#include "song_list.h"
#include <vector>
#include <boost/regex.hpp>
#include <cstdlib>
#include <thread>
#include <dirent.h>
#include "../animator.h"
#include "../button.h"
#include "../sprite.h"
#include "../text_input_box.h"
#include "../control_handler.h"
#include "../sprite.h"
#include "../system.h"
#include "../texture_manager.h"
#include "../file_system.h"
#include "../sound_manager.h"
#include "../song_data.h"
#include "../loading/loading_state.h"
#include "../game/game_state.h"
#include "../user/setting.h"
#include "prepare_header.h"
#include "mod_widget.h"

fr::SongList *fr::SongList::m_instance = 0;
std::vector<fr::Button*> fr::SongList::m_cell;
fr::TextInputBox *fr::SongList::search_bar;
std::vector<fr::SongInformation*> fr::SongList::m_information;
fr::SongInformation *fr::SongList::null_information = 0;
int fr::SongList::list_length = 0;
int fr::SongList::list_process = 0;
int fr::SongList::selected_index = 0;
int fr::SongList::cell_heigh = 0;
int fr::SongList::cell_pos_offset_y = 0;
bool fr::SongList::is_list_moved = false;
bool fr::SongList::is_refreshing = false;
bool fr::SongList::is_loaded = false;

bool fr::SongList::is_shown = true;
bool fr::SongList::is_entered = true;
bool fr::SongList::is_exited = false;

void fr::SongList::init()
{
	cell_heigh = 64;
	TextureManager::instance()->loadfont("assets/fonts/Ubuntu-M.ttf", 32);
	search_bar = new TextInputBox;
	search_bar->init("assets/prepare/search_bar.png", Rect(0, 288, 0, 0));
	search_bar->AddPressedFrame( "assets/prepare/search_bar.png");
	search_bar->InitText(24, 16, "assets/fonts/Ubuntu-M.ttf", 32, 0xFF, 0xFF, 0xFF, TEXTFORMAT_LEFT, 688);

	null_information = new SongInformation;
	Score *null_score = new Score;
	null_information->title = "null";
	null_information->artist = "null";
	null_information->noter = "null";
	null_information->version = "null";
	null_information->difficulty = 0;
	null_information->duration = 0;
	null_score->score = 0;
	null_score->rank = RANK_NONE;
	null_score->pure = 0;
	null_score->safe = 0;
	null_score->warning = 0;
	null_score->error = 0;
	null_information->high_score = null_score;

	Animator::instance()->AddAnimation("song_list_enter", ANIMATIONTYPE_UNIFORMLY_DECELERATED, 300);
	Animator::instance()->AddAnimation("song_list_exit", ANIMATIONTYPE_UNIFORMLY_ACCELERATED, 300);

	if (!is_refreshing && !is_loaded)
	{
		if (!LoadList())
		{
			std::thread refresh_thread(&fr::SongList::RefreshList);
			refresh_thread.detach();
		}
	}
	//一定要在計算列表長度之前加載好信息
	list_length = cell_heigh * m_information.size();
	list_process = 0;
	selected_index = 0;

	RefreshListSize();
	if (m_information.size() == 0)
	{
		list_length = cell_heigh;
		PrepareHeader::instance()->SetInformation(null_information);
	}
	else
	{
		list_length = cell_heigh * m_information.size();
		PrepareHeader::instance()->SetInformation(m_information[selected_index]);
		SoundManager::instance()->load(m_information[selected_index]->audio_path, SOUNDTYPE_MUSIC);
		SoundManager::instance()->play(m_information[selected_index]->audio_path, m_information[selected_index]->preview_time);
	}

}	//void fr::SongList::inìt()

void fr::SongList::clear()
{
	m_cell.clear();
	if (SoundManager::instance()->IsPlayingMusic())
	{
		SoundManager::instance()->stop();
	SoundManager::instance()->clear(m_information[selected_index]->audio_path, SOUNDTYPE_MUSIC);
	}
}

void fr::SongList::update()
{
	if (is_entered)
	{
		static int roll_speed;
		for (int i = 0; i < ControlHandler::instance()->GetFingerCount(); i++)
		{
			Finger load_finger = ControlHandler::instance()->GetFinger(i);
			if ((load_finger.x >= (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT ? 0 : 280)) && (load_finger.x <= (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT ? 720 : 1000)) && (load_finger.y >= (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT ? 352 : 64)) && !ModWidget::instance()->IsShown())
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
	}	//if (is_entered)

	if (System::instance()->IsWindowModified())
	{
		RefreshListSize();
	}

	if (is_shown && !is_entered)
	{
		OnEnter();
	}
	else if (!is_shown && !is_exited)
	{
		OnExit();
	}

	if (!is_exited)
	{
		search_bar->update();

		search_bar->SetPos(System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT ? 0 : 280, (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT ? 288 : 0) + cell_pos_offset_y);
		int current_index = list_process / cell_heigh;
		for (int i = 0; i < m_cell.size(); i++)
		{
			int x = System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT ? 0 : 280;
			int y = -(list_process % cell_heigh) + i * cell_heigh + (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT ? 352 : 64) + cell_pos_offset_y;
			m_cell[i]->SetPos(x, y);
			if (m_information.size() != 0)
			{
				if (current_index >= m_information.size())
				{
					current_index = 0;
				}
				char *difficulty_ch = new char[3];
				sprintf(difficulty_ch, "%d", m_information[current_index]->difficulty);

				if (current_index == selected_index)
				{
					m_cell[i]->SetBaseFrame(2);
					m_cell[i]->GetText(0)->SetText(difficulty_ch);	
					m_cell[i]->GetText(0)->SetColor(0xFF, 0xFF, 0xFF);
					m_cell[i]->GetText(1)->SetText(m_information[current_index]->title);
					m_cell[i]->GetText(1)->SetColor(0xFF, 0xFF, 0xFF);
				}
				else
				{
					m_cell[i]->SetBaseFrame(0);
					m_cell[i]->GetText(0)->SetText(difficulty_ch);
					m_cell[i]->GetText(0)->SetColor(0x00, 0x00, 0x00);
					m_cell[i]->GetText(1)->SetText(m_information[current_index]->title);
					m_cell[i]->GetText(1)->SetColor(0x00, 0x00, 0x00);
				}

				delete [] difficulty_ch;

				if (((m_cell[i]->GetY() < System::instance()->GetWindowHeigh() - 140 && System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT) ||
				(m_cell[i]->GetY() < System::instance()->GetWindowHeigh() - 60 && System::instance()->GetWindowRotation() == WINDOWROTATION_LANDSCAPE) ||
				!is_list_moved) && !ModWidget::instance()->IsShown())
			//這個條件運算吃棗藥丸……
				{
					m_cell[i]->update();
				}
				if (m_cell[i]->IsReleased() && !ModWidget::instance()->IsShown())
				{
					if (current_index == selected_index)
					{
						LoadingState::instance()->init(STATE_GAME);
						GameState::instance()->SetFile(m_information[selected_index]);
						SoundManager::instance()->stop();
					}
					else
					{
						if (m_information[selected_index]->audio_path != m_information[current_index]->audio_path)
						{
							if (SoundManager::instance()->IsPlayingMusic())
							{
								SoundManager::instance()->stop();
								SoundManager::instance()->clear(m_information[selected_index]->audio_path, SOUNDTYPE_MUSIC);
							}
							selected_index = current_index;
							PrepareHeader::instance()->SetInformation(m_information[selected_index]);
							SoundManager::instance()->load(m_information[selected_index]->audio_path, SOUNDTYPE_MUSIC);
							SoundManager::instance()->play(m_information[selected_index]->audio_path, m_information[selected_index]->preview_time);
						}
						else
						{
							selected_index = current_index;									PrepareHeader::instance()->SetInformation(m_information[selected_index]);
						}
					}
				}
				current_index++;
			}
			else
			{
				m_cell[i]->GetText(0)->SetText("??");
				m_cell[i]->GetText(0)->SetColor(0x00, 0x00, 0x00);
				m_cell[i]->GetText(1)->SetText(null_information->title);
				m_cell[i]->GetText(1)->SetColor(0x00, 0x00, 0x00);
					PrepareHeader::instance()->SetInformation(null_information);
				list_length = cell_heigh;
			}
		}
	}

	if (list_process < 0 || list_process > list_length)
	{
		exit(0);
	}
}	//void fr::SongList::update()

void fr::SongList::render()
{
	if (!is_exited)
	{
		for (int i = 0; i < m_cell.size(); i++)
		{
			if (((m_cell[i]->GetY() < System::instance()->GetWindowHeigh() && System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT) ||
			(m_cell[i]->GetY() < System::instance()->GetWindowHeigh() && System::instance()->GetWindowRotation() == WINDOWROTATION_LANDSCAPE)))
			{
				m_cell[i]->render();
			}
		}
		search_bar->render();
	}
}

bool fr::SongList::LoadList()
{
	m_information.clear();

	std::string text;
//	boost::regex pattern("\\[(.*?)\\]\\s*\\{\\s*title:(.*)\\s*artist:(.*)\\s*noter:(.*)\\s*version:(.*)\\s*difficulty:(\\d*)\\s*duration:(\\d*)\\s*audio_path:(.*)\\s*preview_time:(\\d*)\\s*file_path:(.*)\\s*score:(\\d+)\\s*rank:(\\d)\\s*\\}");
	boost::regex song_pattern("\\[(.*?)\\]\\s*?\\{[\\S\\s]*?\\}");
	//包含ID信息在內，先截取其中一段再單獨進行提取
	boost::regex title_pattern("\\ttitle:(.*?)\\n");
	boost::regex artist_pattern("\\tartist:(.*?)\\n");
	boost::regex noter_pattern("\\tnoter:(.*?)\\n");
	boost::regex version_pattern("\\tversion:(.*?)\\n");
	boost::regex difficulty_pattern("\\tdifficulty:(\\d*?)\\n");
	boost::regex duration_pattern("\\tduration:(\\d*?)\\n");
	boost::regex audio_path_pattern("\\taudio_path:(.*?)\\n");
	boost::regex preview_time_pattern("\\tpreview_time:(\\d*?)\\n");
	boost::regex file_path_pattern("\\tfile_path:(.*?)\\n");
	boost::regex full_score_pattern("\\tfull_score:(\\d*?)\\n");

	boost::regex score_pattern("\\tscore:(\\d*?)\\n");
	boost::regex pure_pattern("\\tpure:(\\d*?)\\n");
	boost::regex safe_pattern("\\tsafe:(\\d*?)\\n");
	boost::regex warning_pattern("\\twarning:(\\d*?)\\n");
	boost::regex error_pattern("\\terror:(\\d*?)\\n");
	boost::regex chain_pattern("\\tchain:(\\d*?)\\n");
	boost::regex rank_pattern("\\trank:(\\d)");

	if (!ReadFile("/sdcard/data/song_list.fa", text))
	{
		return false;
	}
//	if (!boost::regex_search(text, pattern))
//	{
//		return false;
//	}
	for (boost::sregex_iterator i = boost::sregex_iterator(text.begin(), text.end(), song_pattern); i != boost::sregex_iterator(); i++)
	{
		boost::smatch paragraph_match = *i;
		std::string paragraph = paragraph_match.str();
		boost::smatch title_line;
		boost::smatch artist_line;
		boost::smatch noter_line;
		boost::smatch version_line;
		boost::smatch difficulty_line;
		boost::smatch duration_line;
		boost::smatch audio_path_line;
		boost::smatch preview_time_line;
		boost::smatch file_path_line;
		boost::smatch full_score_line;
		boost::smatch score_line;
		boost::smatch pure_line;
		boost::smatch safe_line;
		boost::smatch warning_line;
		boost::smatch error_line;
		boost::smatch chain_line;
		boost::smatch rank_line;

		boost::regex_search(paragraph, title_line, title_pattern);
		boost::regex_search(paragraph, artist_line, artist_pattern);
		boost::regex_search(paragraph, noter_line, noter_pattern);
		boost::regex_search(paragraph, version_line, version_pattern);
		boost::regex_search(paragraph, difficulty_line, difficulty_pattern);
		boost::regex_search(paragraph, duration_line, duration_pattern);
		boost::regex_search(paragraph, audio_path_line, audio_path_pattern);
		boost::regex_search(paragraph, preview_time_line, preview_time_pattern);
		boost::regex_search(paragraph, file_path_line, file_path_pattern);
		boost::regex_search(paragraph, full_score_line, full_score_pattern);
		boost::regex_search(paragraph, score_line, score_pattern);
		boost::regex_search(paragraph, pure_line, pure_pattern);
		boost::regex_search(paragraph, safe_line, safe_pattern);
		boost::regex_search(paragraph, warning_line, warning_pattern);
		boost::regex_search(paragraph, error_line, error_pattern);
		boost::regex_search(paragraph, chain_line, chain_pattern);
		boost::regex_search(paragraph, rank_line, rank_pattern);
		SongInformation *new_information = new SongInformation;
		Score *new_score = new Score;
		new_information->id = boost::regex_replace(paragraph, song_pattern, "$1");
		new_information->title = boost::regex_replace(title_line.str(), title_pattern, "$1");
		new_information->artist = boost::regex_replace(artist_line.str(), artist_pattern, "$1");
		new_information->noter = boost::regex_replace(noter_line.str(), noter_pattern, "$1");
		new_information->version = boost::regex_replace(version_line.str(), version_pattern, "$1");
		new_information->difficulty = atoi(boost::regex_replace(difficulty_line.str(), difficulty_pattern, "$1").c_str());
		new_information->duration = atoi(boost::regex_replace(duration_line.str(), duration_pattern, "$1").c_str());
		new_information->audio_path = boost::regex_replace(audio_path_line.str(), audio_path_pattern, "$1");
		new_information->preview_time = atoi(boost::regex_replace(preview_time_line.str(), preview_time_pattern, "$1").c_str());
		new_information->file_path = boost::regex_replace(file_path_line.str(), file_path_pattern, "$1");
		new_information->full_score = atoi(boost::regex_replace(full_score_line.str(), full_score_pattern, "$1").c_str());
		new_score->score = atoi(boost::regex_replace(score_line.str(), score_pattern, "$1").c_str());
		new_score->pure = atoi(boost::regex_replace(pure_line.str(), pure_pattern, "$1").c_str());
		new_score->safe = atoi(boost::regex_replace(safe_line.str(), safe_pattern, "$1").c_str());
		new_score->warning = atoi(boost::regex_replace(warning_line.str(), warning_pattern, "$1").c_str());
		new_score->error = atoi(boost::regex_replace(error_line.str(), error_pattern, "$1").c_str());
		new_score->chain = atoi(boost::regex_replace(chain_line.str(), chain_pattern, "$1").c_str());
		new_score->rank = Rank(atoi(boost::regex_replace(rank_line.str(), rank_pattern, "$1").c_str()));
		new_information->high_score = new_score;
		m_information.push_back(new_information);
	}
	is_loaded = true;
	return true;
}

void fr::SongList::WriteList()
{
	std::string output_text;
	for (int i = 0; i < m_information.size(); i++)
	{
		char *difficulty_ch = new char[3];
		char *duration_ch = new char[4];
		char *preview_time_ch = new char[10];
		char *full_score_ch = new char[6];
		char *score_ch = new char[6];
		char *pure_ch = new char[6];
		char *safe_ch = new char[6];
		char *warning_ch = new char[6];
		char *error_ch = new char[6];
		char *chain_ch = new char[6];
		char *rank_ch = new char;
		sprintf(difficulty_ch, "%d", m_information[i]->difficulty);
		sprintf(duration_ch, "%d", m_information[i]->duration);
		sprintf(preview_time_ch, "%d", m_information[i]->preview_time);
		sprintf(full_score_ch, "%d", m_information[i]->full_score);
		sprintf(score_ch, "%d", m_information[i]->high_score->score);
		sprintf(pure_ch, "%d", m_information[i]->high_score->pure);
		sprintf(safe_ch, "%d", m_information[i]->high_score->safe);
		sprintf(warning_ch, "%d", m_information[i]->high_score->warning);
		sprintf(error_ch, "%d", m_information[i]->high_score->error);
		sprintf(chain_ch, "%d", m_information[i]->high_score->chain);
		sprintf(rank_ch, "%d", m_information[i]->high_score->rank);
		output_text += "[";
		output_text += m_information[i]->id;
		output_text += "]\n{\n";
		output_text += "\ttitle:" + m_information[i]->title + "\n";
		output_text += "\tartist:" +  m_information[i]->artist + "\n";
		output_text += "\tnoter:" + m_information[i]->noter + "\n";
		output_text += "\tversion:" + m_information[i]->version + "\n";
		output_text += "\tdifficulty:";
		output_text += difficulty_ch;
		output_text += "\n";
		output_text += "\tduration:";
		output_text += duration_ch;
		output_text += "\n";
		output_text += "\taudio_path:" + m_information[i]->audio_path + "\n";
		output_text += "\tpreview_time:";
		output_text += preview_time_ch;
		output_text += "\n";
		output_text += "\tfile_path:" + m_information[i]->file_path + "\n";
		output_text += "\tfull_score:";
		output_text += full_score_ch;
		output_text += "\n";
		output_text += "\tscore:";
		output_text += score_ch;
		output_text += "\n";
		output_text += "\tpure:";
		output_text += pure_ch;
		output_text += "\n";
		output_text += "\tsafe:";
		output_text += safe_ch;
		output_text += "\n";
		output_text += "\twarning:";
		output_text += warning_ch;
		output_text += "\n";
		output_text += "\terror:";
		output_text += error_ch;
		output_text += "\n";
		output_text += "\tchain:";
		output_text += chain_ch;
		output_text += "\n";
		output_text += "\trank:";
		output_text += rank_ch;
		output_text += "\n";
		output_text += "}\n";
		delete [] difficulty_ch;
		delete [] duration_ch;
		delete [] preview_time_ch;
		delete [] full_score_ch;
		delete [] score_ch;
		delete [] pure_ch;
		delete [] safe_ch;
		delete [] warning_ch;
		delete [] error_ch;
		delete [] chain_ch;
		delete [] rank_ch;
	}
	WriteFile("/sdcard/data/song_list.fa", output_text);
}

void fr::SongList::RefreshList()
{
	is_refreshing = true;

	std::vector<File*> file;
	std::vector<std::string> list_path;
	Setting::instance()->GetSongList(list_path);
	for (int i = 0; i < list_path.size(); i++)
	{
		FindFile(list_path[i], ".*\\.osu", file);
		FindFile(list_path[i], ".*\\.imd", file);
	}
	Setting::instance()->write();

	for (int i = 0; i < m_information.size(); i++)
	{
		for (std::vector<File*>::iterator iter = file.begin(); iter != file.end();)
		{
			File *check_file = *iter;
			if (m_information[i]->file_path == check_file->name)
			{
				file.erase(iter);
				iter = file.begin();
			}
			else
			{
				iter++;
			}
		}
	}
	for (int i = 0; i < file.size(); i++)
	{
		SongInformation *new_song_information = new SongInformation;

		if (LoadBeatmapFile(file[i]->name, new_song_information, NULL, NULL, NULL, NULL))
		{
			m_information.push_back(new_song_information);

			list_length = cell_heigh * m_information.size();
			PrepareHeader::instance()->SetInformation(m_information[selected_index]);
			WriteList();
			//實時寫入到緩存文件，中途退出回來刷新的時可以繼續進度
		}
	}

	is_refreshing = false;
	is_loaded = true;
}	//void fr::SongList::RefreshList()

bool fr::SongList::IsRefreshing()
{
	return is_refreshing;
}

void fr::SongList::RefreshListSize()
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
		new_song_cell->init("assets/prepare/song_cell.png", Rect(0, 0, 720, 64));
		new_song_cell->AddPressedFrame( "assets/prepare/song_cell_pressed.png");
		new_song_cell->AddFrame("assets/prepare/song_cell_selected.png");		new_song_cell->AddText("??", 24, 16, "assets/fonts/Ubuntu-M.ttf", 32, 0xFF, 0xFF, 0xFF, TEXTFORMAT_LEFT, 40);
		new_song_cell->AddText("NULL", 96, 16, "assets/fonts/Ubuntu-M.ttf", 32, 0xFF, 0xFF, 0xFF, TEXTFORMAT_LEFT, 616);
		m_cell.push_back(new_song_cell);
	}
}

void fr::SongList::OnEnter()
{
	cell_pos_offset_y = (System::instance()->GetWindowHeigh() - (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT ? 352 : 64)) * (1.f - Animator::instance()->GetProcess("song_list_enter"));
	if (Animator::instance()->IsTimeUp("song_list_enter"))
	{
		Animator::instance()->ResetAnimation("song_list_enter");
		is_entered = true;
	}
}

void fr::SongList::OnExit()
{
	cell_pos_offset_y = (System::instance()->GetWindowHeigh() - (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT ? 352 : 64)) * Animator::instance()->GetProcess("song_list_exit");
	if (Animator::instance()->IsTimeUp("song_list_exit"))
	{
		Animator::instance()->ResetAnimation("song_list_exit");
		is_exited = true;
		is_entered = false;
	}
}

void fr::SongList::SwitchShown()
{
	is_shown = !is_shown;
	if (is_shown)
	{
		Animator::instance()->Animate("song_list_enter");
		is_exited = false;
	}
	else
	{
		Animator::instance()->Animate("song_list_exit");
	}
}

bool fr::SongList::IsShown()
{
	return is_shown || !is_exited;
}