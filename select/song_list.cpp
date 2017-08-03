#include "song_list.h"

lm::SongList *lm::SongList::m_instance = 0;

void lm::SongList::init()
{
	TextureManager::instance()->loadfont("assets/Exo-Light.ttf", 30);
	select_cover = new Sprite;
	std::string text;
	std::ifstream file;
	file.open("assets/songs/song_list.fml");
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
		new_song_information->m_cover_path = std::regex_replace(result_str, pattern, std::string("$5"));
		new_cell->load("assets/song_cell_right.png", "assets/song_cell_right_selected.png", 0, 0, 549, 123);
		new_cell->SetText(new_song_information->m_title, "assets/Exo-Light.ttf", 0x00, 0x00, 0x00);
		m_information.push_back(new_song_information);
		m_cell.push_back(new_cell);
	}
	list_length = 123 * m_cell.size() - System::instance()->GetWindowHeigh();
	list_process = 0;
	selected_index = 0;
	select_cover->load(m_information[selected_index]->m_cover_path, 135, 120, 512, 512);
}	//void lm::SongList::inìt()

void lm::SongList::clear()
{
	m_cell.clear();
	m_information.clear();
}

void lm::SongList::update()
{
	static int roll_speed;
	for (int i = 0; i < ControlHandler::instance()->GetFingerCount(); i++)
	{
		Finger load_finger = ControlHandler::instance()->GetFinger(i);
		if (load_finger.x >= System::instance()->GetWindowWidth() - 549)
		{
			list_process -= load_finger.dy;
			roll_speed = load_finger.dy;

			if (list_process < 0)
			{
				list_process = 0;
			}
			else if (list_process > list_length && list_length > System::instance()->GetWindowHeigh())
			{
				list_process = list_length;
			}
			//把list_process盡量控制在[0,list_length]區間內
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
		if (list_process < 0)
		{
			list_process = 0;
		}
		else if (list_process > list_length && list_length > System::instance()->GetWindowHeigh())
		//順便檢測列表長度是否超過屏幕寬度，免得列表顯得抽搐……
		{
			list_process = list_length;
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
				std::string output = "Enter song ";
				output += m_information[i]->m_title;
				DebugWidget::instance()->PushLog(output);
			}
			else
			{
				selected_index = i;
				select_cover->clear();
				select_cover->load(m_information[i]->m_cover_path, 135, 120, 512, 512);
				std::string output = "Selected song ";
				output += m_information[i]->m_title;
				DebugWidget::instance()->PushLog(output);
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
	select_cover->render();
	TextureManager::instance()->render(m_information[selected_index]->m_title, 0, 0, "assets/Exo-Light.ttf", 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);
	TextureManager::instance()->render(m_information[selected_index]->m_artist, 0, 40, "assets/Exo-Light.ttf", 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);
	TextureManager::instance()->render(m_information[selected_index]->m_noter, 0, 80, "assets/Exo-Light.ttf", 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);
	TextureManager::instance()->render(m_information[selected_index]->m_bpm, 0, 120, "assets/Exo-Light.ttf", 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);
}