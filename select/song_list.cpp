#include "song_list.h"
#include <regex>
#include <fstream>
#include <dirent.h>
#include "../button.h"
#include "../control_handler.h"
#include "../sprite.h"
#include "../system.h"
#include "../texture_manager.h"
#include "../loading/loading_state.h"
#include "../main/main_state.h"
#include "select_state.h"
#include "song_header.h"

lm::SongList *lm::SongList::m_instance = 0;

void lm::SongList::init()
{
	cell_heigh = 60;
	TextureManager::instance()->loadfont("assets/Ubuntu-M.ttf", 28);

	SongInformation *new_information = new SongInformation;
	new_information->m_title = "Title";
	new_information->m_artist = "Artist";
	new_information->m_noter = "Noter";
	new_information->m_bpm = "???";
	m_information.push_back(new_information);

	list_length = cell_heigh * m_information.size();
	list_process = 0;
	for (int i = 0; i < ((System::instance()->GetWindowHeigh() - 500) / cell_heigh + 2); i++)
	{
		Button *new_song_cell = new Button;
		new_song_cell->load("assets/select_song_cell.png", "assets/select_song_cell_pressed.png", 0, 0, 654, 60);
		new_song_cell->SetTextPos(16, 20, TEXTFORMAT_LEFT, 640);
		m_cell.push_back(new_song_cell);
	}
	selected_index = 0;
	SongHeader::instance()->SetInformation(m_information[selected_index]);
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

	int current_index = list_process / cell_heigh;
	for (int i = 0; i < m_cell.size(); i++)
	{
		int x = System::instance()->GetWindowWidth() - 654;
		int y = -(list_process % cell_heigh) + i * cell_heigh + 360;
		m_cell[i]->SetPos(x, y);
		if (current_index >= m_information.size())
		{
			current_index = 0;
		}
			m_cell[i]->SetText(m_information[current_index]->m_title, "assets/Ubuntu-M.ttf", 28, 0x00, 0x00, 0x00);

		if (m_cell[i]->GetY() < System::instance()->GetWindowHeigh() - 140 || !is_list_moved)
		{
			m_cell[i]->update();
		}
		if (m_cell[i]->IsReleased())
		{
			if (current_index == selected_index)
			{
				LoadingState::instance()->init(MainState::instance(), SelectState::instance());
			}
			else
			{
				selected_index = current_index;
				SongHeader::instance()->SetInformation(m_information[selected_index]);
			}
		}
		current_index++;
	}

	if (list_process < 0 || list_process > list_length)
	{
		exit(0);
	}
}

void lm::SongList::render()
{
	for (int i = 0; i < m_cell.size(); i++)
	{
		if (m_cell[i]->GetY() < System::instance()->GetWindowHeigh() - 140)
		m_cell[i]->render();
	}
}