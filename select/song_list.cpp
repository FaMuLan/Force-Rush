#include "song_list.h"

lm::SongList *lm::SongList::m_instance = 0;

void lm::SongList::init()
{
	TextureManager::instance()->loadfont("assets/Exo-Light.ttf", 30);
	std::string text;
	std::ifstream file;
	file.open("songs/song_list.fml");
	while (!file.eof())
	{
		text += file.get();
	}
	file.close();
	std::regex pattern("\"(.+?)\",\"(.+?)\",\"(.+?)\",\"(.+?)\",\"(.+?)\"");
	//"title","artist","noter","bpm","cover_path"
	for (std::sregex_iterator i = std::sregex_iterator(text.begin(), text.end(), pattern); i != std::sregex_iterator(); ++i)
	{
		std::smatch result = *i;
		std::string result_str = result.str();
		Button *new_cell = new Button;
		SongInformation *new_song_information = new SongInformation;
		new_song_information->m_title = std::regex_replace(result_str, pattern, std::string("$1"));
		new_song_information->m_artist = std::regex_replace(result_str, pattern, std::string("$2"));
		new_song_information->m_noter = std::regex_replace(result_str, pattern, std::string("$3"));
		new_song_information->m_bpm = std::regex_replace(result_str, pattern, std::string("$4"));
		new_song_information->m_cover_path = std::regex_replace(result_str, pattern, std::string("$4"));
		new_cell->load("assets/song_cell_right.png", "assets/song_cell_right_selected.png", 0, 0, 549, 123);
		new_cell->SetText(new_song_information->m_title, "assets/Exo-Light.ttf", 0x00, 0x00, 0x00);
		m_information.push_back(new_song_information);
		m_cell.push_back(new_cell);
	}
	list_length = 123 * m_cell.size() - System::instance()->GetWindowHeigh();
	list_process = 0;
	selected_index = 0;
}	//void lm::SongList::inìt()

void lm::SongList::clear()
{
	m_cell.clear();
	m_information.clear();
}

void lm::SongList::update()
{
	for (int i = 0; i < ControlHandler::instance()->GetFingerCount(); i++)
	{
		Finger load_finger = ControlHandler::instance()->GetFinger(i);
		if (load_finger.x >= System::instance()->GetWindowWidth() - 549)
		{
			list_process -= load_finger.dy;

			if (list_process > list_length)
			{
				list_process = list_length;
			}
			else if (list_process < 0)
			{
				list_process = 0;
			}
			//把list_process盡量控制在[0,list_length]區間內
		}
	}
	for (int i = 0; i < m_cell.size(); i++)
	{
		m_cell[i]->SetPos(System::instance()->GetWindowWidth() - 549, 123 * i - list_process);
		m_cell[i]->update();
		if (m_cell[i]->IsReleased())
		{
			if (i == selected_index)
			{
				LoadingState::instance()->init(CraftingState::instance(), SelectState::instance());
			}
			else
			{
				selected_index = i;
			}
		}
	}
}

void lm::SongList::render()
{
	for (int i = 0; i < m_cell.size(); i++)
	{
		m_cell[i]->render();
	}
}