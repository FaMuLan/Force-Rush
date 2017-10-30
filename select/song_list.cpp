#include "song_list.h"
#include <vector>
#include <regex>
#include <cstdlib>
#include <dirent.h>
#include "../button.h"
#include "../control_handler.h"
#include "../sprite.h"
#include "../system.h"
#include "../texture_manager.h"
#include "../file_system.h"
#include "../message_box.h"
#include "../loading/loading_state.h"
#include "../main/main_state.h"
#include "../game/game_state.h"
#include "../game/beatmap.h"
#include "select_state.h"
#include "song_header.h"

lm::SongList *lm::SongList::m_instance = 0;
std::vector<lm::Button*> lm::SongList::m_cell;
std::vector<lm::SongInformation*> lm::SongList::m_information;
lm::SongInformation *lm::SongList::null_information = 0;
int lm::SongList::list_length = 0;
int lm::SongList::list_process = 0;
int lm::SongList::selected_index = 0;
int lm::SongList::cell_heigh = 0;
bool lm::SongList::is_list_moved = false;
bool lm::SongList::is_refreshing = false;

void lm::SongList::init()
{
	cell_heigh = 60;
	TextureManager::instance()->loadfont("assets/Ubuntu-M.ttf", 28);

	null_information = new SongInformation;
	null_information->title = "null";
	null_information->artist = "null";
	null_information->noter = "null";
	null_information->bpm = "null";

	if (!is_refreshing)
	{
		LoadList();
	}
	//一定要在計算列表長度之前加載好信息
	list_length = cell_heigh * m_information.size();
	list_process = 0;
	selected_index = 0;

	RefreshListSize();
	if (m_information.size() == 0)
	{
		list_length = cell_heigh;
		SongHeader::instance()->SetInformation(null_information);
	}
	else
	{
		list_length = cell_heigh * m_information.size();
		SongHeader::instance()->SetInformation(m_information[selected_index]);
	}
}	//void lm::SongList::inìt()

void lm::SongList::clear()
{
	m_cell.clear();
	if (!is_refreshing)
	{
		m_information.clear();
	}
}

void lm::SongList::update()
{
	static int roll_speed;
	for (int i = 0; i < ControlHandler::instance()->GetFingerCount(); i++)
	{
		Finger load_finger = ControlHandler::instance()->GetFinger(i);
		if (load_finger.x >= System::instance()->GetWindowWidth() - 654)
		{
			list_process -= load_finger.dy;
			roll_speed = load_finger.dy;

			while (list_process < 0)
			{
				list_process += list_length;
			}
			while (list_process >= list_length)
			{
				list_process -= list_length;
			}
			//把list_process盡量控制在[0,list_length)區間內
			//艸有必要用一個笨方法來控制變量在範圍內了
		}
	}

	if (ControlHandler::instance()->GetFingerCount() == 0)
	{
		if (roll_speed < 0)
		{
			roll_speed += 2;
			roll_speed = roll_speed > 0 ? 0 : roll_speed;
		}
		else if (roll_speed > 0)
		{
			roll_speed -= 2;
			roll_speed = roll_speed < 0 ? 0 : roll_speed;
		}
		//不包含 roll_speed == 0 這個情況
		list_process -= roll_speed;
		while (list_process < 0)
		{
			list_process += list_length;
		}
		while (list_process >= list_length)
		{
			list_process -= list_length;
		}
	}

	is_list_moved = roll_speed != 0;
	//检测列表是否移动。

	if (System::instance()->IsWindowModified())
	{
		RefreshListSize();
	}

	int current_index = list_process / cell_heigh;
	for (int i = 0; i < m_cell.size(); i++)
	{
		int x = System::instance()->GetWindowWidth() - (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT ? 654 : 560);
		int y = -(list_process % cell_heigh) + i * cell_heigh + (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT ? 360 : 60);
		m_cell[i]->SetPos(x, y);
		if (m_information.size() != 0)
		{
			if (current_index >= m_information.size())
			{
				current_index = 0;
			}
				m_cell[i]->SetText(m_information[current_index]->title, "assets/Ubuntu-M.ttf", 28, 0x00, 0x00, 0x00);

			if ((m_cell[i]->GetY() < System::instance()->GetWindowHeigh() - 140 && System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT) ||
			(m_cell[i]->GetY() < System::instance()->GetWindowHeigh() - 60 && System::instance()->GetWindowRotation() == WINDOWROTATION_LANDSCAPE) ||
			!is_list_moved)
			//這個條件運算吃棗藥丸……
			{
				m_cell[i]->update();
			}
			if (m_cell[i]->IsReleased())
			{
				if (current_index == selected_index)
				{
					LoadingState::instance()->init(GameState::instance(), SelectState::instance());
					GameState::instance()->SetFile(m_information[selected_index]->file_path);
				}
				else
				{
					selected_index = current_index;
					SongHeader::instance()->SetInformation(m_information[selected_index]);
				}
			}
			current_index++;
		}
		else
		{
			m_cell[i]->SetText(null_information->title, "assets/Ubuntu-M.ttf", 28, 0x00, 0x00, 0x00);
				SongHeader::instance()->SetInformation(null_information);
			list_length = cell_heigh;
		}
	}

	if (list_process < 0 || list_process > list_length)
	{
		exit(0);
	}
}	//void lm::SongList::update()

void lm::SongList::render()
{
	for (int i = 0; i < m_cell.size(); i++)
	{
		if ((m_cell[i]->GetY() < System::instance()->GetWindowHeigh() - 140 && System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT) ||
		(m_cell[i]->GetY() < System::instance()->GetWindowHeigh() - 60 && System::instance()->GetWindowRotation() == WINDOWROTATION_LANDSCAPE))
		{
			m_cell[i]->render();
		}
	}
}

bool lm::SongList::LoadList()
{
	m_information.clear();
	bool exist = false;

	std::string text;
	std::regex pattern("(.*?),(.*?),(.*?),(.*?),(.*?)\\n");
	if (!ReadFile("/sdcard/data/song_list.fa", text))
	{
		return false;
	}
	for (std::sregex_iterator i = std::sregex_iterator(text.begin(), text.end(), pattern); i != std::sregex_iterator(); ++i)
	{
		exist = true;
		std::smatch line = *i;
		SongInformation *new_information = new SongInformation;
		new_information->title = std::regex_replace(line.str(), pattern, "$1");
		new_information->artist = std::regex_replace(line.str(), pattern, "$2");
		new_information->noter = std::regex_replace(line.str(), pattern, "$3");
		new_information->bpm = std::regex_replace(line.str(), pattern, "$4");
		new_information->file_path = std::regex_replace(line.str(), pattern, "$5");
		m_information.push_back(new_information);
	}
	char *output_ch = new char[50];
	sprintf(output_ch, "Match %d files", m_information.size());
	MessageBox::instance()->SetText(output_ch);
	return exist;
}

void lm::SongList::RefreshList()
{
	is_refreshing = true;
	MessageBox::instance()->SetText("Start Refresh!");
	char *output_ch = new char[50];
	m_information.clear();

	std::vector<File*> file;
	std::regex title_pattern("Title:(.*)");
	std::regex artist_pattern("Artist:(.*)");
	std::regex noter_pattern("Creator:(.*)");
	std::regex bpm_pattern("\\d+,([\\d.-]+),\\d+,\\d+,\\d+,\\d+,1,\\d+");
	FindFile("/sdcard/data/malody/beatmap", ".*\\.osu", file);
	sprintf(output_ch, "Match %d files", file.size());
	MessageBox::instance()->SetText(output_ch);
	for (int i = 0; i < file.size(); i++)
//	for (int i = 0; i < 3; i++)
	{
		SongInformation *new_song_information = new SongInformation;
		new_song_information->file_path = file[i]->name;
		std::string text;
		ReadFile(file[i]->name, text);
		std::smatch title_line;
		std::smatch artist_line;
		std::smatch noter_line;

		std::regex_search(text, title_line, title_pattern);
		std::regex_search(text, artist_line, artist_pattern);
		std::regex_search(text, noter_line, noter_pattern);

		new_song_information->title = std::regex_replace(title_line.str(), title_pattern, "$1");
		new_song_information->artist = std::regex_replace(artist_line.str(), artist_pattern, "$1");
		new_song_information->noter = std::regex_replace(noter_line.str(), noter_pattern, "$1");
		for (std::sregex_iterator i = std::sregex_iterator(text.begin(), text.end(), bpm_pattern); i != std::sregex_iterator(); ++i)
		{
			std::smatch bpm_line = *i;
			char *bpm_ch = new char[5];
			int bpm_integer = int(60000 / atof(std::regex_replace(bpm_line.str(), bpm_pattern, "$1").c_str()));
			sprintf(bpm_ch, "%d", bpm_integer);
			new_song_information->bpm += bpm_ch;
			new_song_information->bpm += " - ";
		}
		m_information.push_back(new_song_information);

		list_length = cell_heigh * m_information.size();
		SongHeader::instance()->SetInformation(m_information[selected_index]);
		sprintf(output_ch, "Loaded %d%%", int(float(i) / float(file.size()) * 100));
		MessageBox::instance()->SetText(output_ch);
	}

	std::string output_text;
	for (int i = 0; i < m_information.size(); i++)
	{
		output_text += m_information[i]->title + ",";
		output_text += m_information[i]->artist + ",";
		output_text += m_information[i]->noter + ",";
		output_text += m_information[i]->bpm + ",";
		output_text += m_information[i]->file_path + "\n";
	}
	WriteFile("/sdcard/data/song_list.fa", output_text);
	MessageBox::instance()->SetText("Refresh Completed!");
	is_refreshing = false;
}	//void lm::SongList::RefreshList()

bool lm::SongList::IsRefreshing()
{
	return is_refreshing;
}

void lm::SongList::RefreshListSize()
{
	m_cell.clear();
	int cell_count;

	if (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT)
	{
		cell_count = ((System::instance()->GetWindowHeigh() - 500) / cell_heigh + 2);
	}
	else
	{
		cell_count = ((System::instance()->GetWindowHeigh() - 120) / cell_heigh + 2);
	}

	for (int i = 0; i < cell_count; i++)
	{
		Button *new_song_cell = new Button;
		new_song_cell->load("assets/select_song_cell.png", "assets/select_song_cell_pressed.png", 0, 0, 654, 60);
		new_song_cell->SetTextPos(16, 20, TEXTFORMAT_LEFT, 640);
		m_cell.push_back(new_song_cell);
	}
}