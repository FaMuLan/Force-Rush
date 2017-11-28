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
#include "../user/setting.h"
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
	cell_heigh = 64;
	TextureManager::instance()->loadfont("assets/fonts/Ubuntu-M.ttf", 32);

	null_information = new SongInformation;
	null_information->title = "null";
	null_information->artist = "null";
	null_information->noter = "null";
	null_information->bpm = "null";
	null_information->version = "null";
	null_information->difficulty = 0;
	null_information->duration = 0;

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
		int x = System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT ? 0 : 280;
		int y = -(list_process % cell_heigh) + i * cell_heigh + (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT ? 352 : 64);
		m_cell[i]->SetPos(x, y);
		if (m_information.size() != 0)
		{
			if (current_index >= m_information.size())
			{
				current_index = 0;
			}
			m_cell[i]->ClearText();
			char *difficulty_ch = new char[3];
			sprintf(difficulty_ch, "%d", m_information[current_index]->difficulty);
				m_cell[i]->AddText(difficulty_ch, 24, 16, "assets/fonts/Ubuntu-M.ttf", 32, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 40);
				m_cell[i]->AddText(m_information[current_index]->title, 96, 16, "assets/fonts/Ubuntu-M.ttf", 32, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 616);
				delete [] difficulty_ch;

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
					GameState::instance()->SetFile(m_information[selected_index]);
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
			m_cell[i]->ClearText();
			m_cell[i]->AddText(null_information->title, 96, 16, "assets/fonts/Ubuntu-M.ttf", 32, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 640);
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
		if ((m_cell[i]->GetY() < System::instance()->GetWindowHeigh() && System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT) ||
		(m_cell[i]->GetY() < System::instance()->GetWindowHeigh() && System::instance()->GetWindowRotation() == WINDOWROTATION_LANDSCAPE))
		{
			m_cell[i]->render();
		}
	}
}

bool lm::SongList::LoadList()
{
	m_information.clear();

	std::string text;
	std::regex pattern("(.*?),(.*?),(.*?),(.*?),(.*?),(\\d+?),(\\d+?),(.*?)\\n");
	if (!ReadFile("/sdcard/data/song_list.fa", text))
	{
		return false;
	}
	if (!std::regex_search(text, pattern))
	{
		return false;
	}
	for (std::sregex_iterator i = std::sregex_iterator(text.begin(), text.end(), pattern); i != std::sregex_iterator(); ++i)
	{
		std::smatch line = *i;
		SongInformation *new_information = new SongInformation;
		new_information->title = std::regex_replace(line.str(), pattern, "$1");
		new_information->artist = std::regex_replace(line.str(), pattern, "$2");
		new_information->noter = std::regex_replace(line.str(), pattern, "$3");
		new_information->bpm = std::regex_replace(line.str(), pattern, "$4");
		new_information->version = std::regex_replace(line.str(), pattern, "$5");
		new_information->difficulty = atoi(std::regex_replace(line.str(), pattern, "$6").c_str());
		new_information->duration = atoi(std::regex_replace(line.str(), pattern, "$7").c_str());
		new_information->file_path = std::regex_replace(line.str(), pattern, "$8");
		m_information.push_back(new_information);
	}
	char *output_ch = new char[50];
	sprintf(output_ch, "Match %d files", m_information.size());
	MessageBox::instance()->SetText(output_ch);
	return true;
}

void lm::SongList::RefreshList()
{
	is_refreshing = true;
	MessageBox::instance()->SetText("Start Refresh!");
	char *output_ch = new char[50];
	m_information.clear();

	std::vector<File*> file;
	std::vector<std::string> list_path;
	std::regex title_pattern("Title:(.*)");
	std::regex artist_pattern("Artist:(.*)");
	std::regex noter_pattern("Creator:(.*)");
	std::regex version_pattern("Version:(.*)");
	std::regex mode_pattern("Mode: (\\d)");
	std::regex key_count_pattern("CircleSize:(\\d)");
	std::regex bpm_pattern("\\d+,([\\d.-]+),\\d+,\\d+,\\d+,\\d+,1,\\d+");
	std::regex note_pattern("\\d+,\\d+,(\\d+),\\d+,\\d+,\\d+:\\d+:\\d+:\\d+:(\\d+:)?");
	Setting::instance()->GetSongList(list_path);
	for (int i = 0; i < list_path.size(); i++)
	{
		std::vector<File*> new_file;
		FindFile(list_path[i], ".*\\.osu", new_file);
		file.insert(file.end(), new_file.begin(), new_file.end());
	}
	sprintf(output_ch, "Match %d files", file.size());
	MessageBox::instance()->SetText(output_ch);
	for (int i = 0; i < file.size(); i++)
//	for (int i = 0; i < 3; i++)
	{
		SongInformation *new_song_information = new SongInformation;
		bool success = true;
		int note_count = 0;
		int key_count = 0;
		int mode = 0;
		new_song_information->file_path = file[i]->name;
		std::string text;
		ReadFile(file[i]->name, text);
		std::smatch title_line;
		std::smatch artist_line;
		std::smatch noter_line;
		std::smatch version_line;
		std::smatch mode_line;
		std::smatch key_count_line;

		std::regex_search(text, title_line, title_pattern);
		std::regex_search(text, artist_line, artist_pattern);
		std::regex_search(text, noter_line, noter_pattern);
		std::regex_search(text, version_line, version_pattern);
		std::regex_search(text, mode_line, mode_pattern);
		std::regex_search(text, key_count_line, key_count_pattern);

		new_song_information->title = std::regex_replace(title_line.str(), title_pattern, "$1");
		new_song_information->artist = std::regex_replace(artist_line.str(), artist_pattern, "$1");
		new_song_information->noter = std::regex_replace(noter_line.str(), noter_pattern, "$1");
		new_song_information->version = std::regex_replace(version_line.str(), version_pattern, "$1");
		success = success && (atoi(std::regex_replace(mode_line.str(), mode_pattern, "$1").c_str()) == 3);
		success = success && (atoi(std::regex_replace(key_count_line.str(), key_count_pattern, "$1").c_str()) == 4);

		for (std::sregex_iterator i = std::sregex_iterator(text.begin(), text.end(), note_pattern); i != std::sregex_iterator(); i++)
		{
			std::smatch note_line = *i;
			new_song_information->duration = atoi(std::regex_replace(note_line.str(), note_pattern, "$1").c_str()) / 1000;
			note_count++;
		}
		new_song_information->difficulty = note_count / new_song_information->duration;

		for (std::sregex_iterator i = std::sregex_iterator(text.begin(), text.end(), bpm_pattern); i != std::sregex_iterator(); ++i)
		{
			std::smatch bpm_line = *i;
			char *bpm_ch = new char[5];
			int bpm_integer = int(60000 / atof(std::regex_replace(bpm_line.str(), bpm_pattern, "$1").c_str()));
			sprintf(bpm_ch, "%d", bpm_integer);
			new_song_information->bpm += bpm_ch;
			new_song_information->bpm += " - ";
		}

		if (success)
		{
			m_information.push_back(new_song_information);

			list_length = cell_heigh * m_information.size();
			SongHeader::instance()->SetInformation(m_information[selected_index]);
		}

		sprintf(output_ch, "Loaded %d%%", int(float(i) / float(file.size()) * 100));
		MessageBox::instance()->SetText(output_ch);
	}

	std::string output_text;
	for (int i = 0; i < m_information.size(); i++)
	{
		char *difficulty_ch = new char[3];
		char *duration_ch = new char[4];
		sprintf(difficulty_ch, "%d,", m_information[i]->difficulty);
		sprintf(duration_ch, "%d,", m_information[i]->duration);
		output_text += m_information[i]->title + ",";
		output_text += m_information[i]->artist + ",";
		output_text += m_information[i]->noter + ",";
		output_text += m_information[i]->bpm + ",";
		output_text += m_information[i]->version + ",";
		output_text += difficulty_ch;
		output_text += duration_ch;
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
		cell_count = ((System::instance()->GetWindowHeigh() - 352) / cell_heigh + 2);
	}
	else
	{
		cell_count = ((System::instance()->GetWindowHeigh() - 64) / cell_heigh + 2);
	}

	for (int i = 0; i < cell_count; i++)
	{
		Button *new_song_cell = new Button;
		new_song_cell->init("assets/select/song_cell.png", 0, 0, 720, 64);
		new_song_cell->AddPressedFrame( "assets/select/song_cell_pressed.png");
		new_song_cell->AddFrame("assets/select/song_cell_selected.png");
		m_cell.push_back(new_song_cell);
	}
}