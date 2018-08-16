#include "song_list.h"
#include <vector>
#include <cstdlib>
#include <thread>
#include <dirent.h>
#include "../animator.h"
#include "../gui/button.h"
#include "../sprite.h"
#include "../gui/text_input_box.h"
#include "../control_handler.h"
#include "../sprite.h"
#include "../system.h"
#include "../texture_manager.h"
#include "../timer.h"
#include "../file_system.h"
#include "../sound_manager.h"
#include "../song_data.h"
#include "../loading/loading_state.h"
#include "../game/game_state.h"
#include "../user/setting.h"
#include "prepare_header.h"
#include "mod_widget.h"
#include "song_path_manager.h"

fr::SongList *fr::SongList::m_instance = 0;
std::vector<fr::SongInformation*> fr::SongList::m_information;
std::vector<fr::SongInformation*> fr::SongList::shown_information;
fr::SongInformation *fr::SongList::null_information = 0;
bool fr::SongList::is_refreshing = false;
bool fr::SongList::is_loaded = false;
bool fr::SongList::is_reverse = false;
fr::SortType fr::SongList::sort_type = fr::SORTTYPE_DEFAULT;

void fr::SongList::init()
{
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

	cell_base_path = "assets/prepare/song_cell.png";
	cell_base_pressed_path = "assets/prepare/song_cell_pressed.png";
	cell_base_selected_path = "assets/prepare/song_cell_selected.png";

	if (!is_refreshing && !is_loaded)
	{
		if (!LoadList())
		{
			std::thread refresh_thread(&fr::SongList::RefreshList);
			refresh_thread.detach();
		}
	}
	//一定要在計算列表長度之前加載好信息

	if (m_information.size() == 0)
	{
		List::init(Rect((System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT ? 0 : 280), (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT ? 352 : 64), 720, System::instance()->GetWindowHeigh() - (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT ? 352 : 64)), 1, true, true);
		PrepareHeader::instance()->SetInformation(null_information);
	}
	else
	{
		List::init(Rect((System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT ? 0 : 280), (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT ? 352 : 64), 720, System::instance()->GetWindowHeigh() - (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT ? 352 : 64)), m_information.size(), true, true);
		PrepareHeader::instance()->SetInformation(m_information[selected_index]);
		SoundManager::instance()->load(m_information[selected_index]->audio_path);
		SoundManager::instance()->GetProcess("default_music")->path = m_information[selected_index]->audio_path;
		SoundManager::instance()->play("default_music");
		SoundManager::instance()->seek("default_music", m_information[selected_index]->preview_time);
	}
	RefreshListSize();
	search_bar->SetPos(dest_rect.x, dest_rect.y - cell_h);
}	//void fr::SongList::inìt()

void fr::SongList::clear()
{
	cell.clear();
	SoundManager::instance()->GetProcess("default_music")->is_playing = false;
	SoundManager::instance()->clear(m_information[selected_index]->audio_path);
}

void fr::SongList::update()
{
	if (System::instance()->IsWindowModified())
	{
		List::SetPos(System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT ? 0 : 280, System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT ? 352 : 64);
		List::SetSize(720, System::instance()->GetWindowHeigh() - (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT ? 352 : 64));
		search_bar->SetPos(dest_rect.x, dest_rect.y - cell_h);
		RefreshListSize();
	}

	search_bar->update();
	List::update();

	if (shown_information.size() != 0)
	{
		if (IsConfirmed())
		{
			LoadingState::instance()->init(STATE_GAME);
			GameState::instance()->SetFile(shown_information[selected_index]);
			SoundManager::instance()->GetProcess("default_music")->is_playing = false;
		}
		else 
		{
			if (shown_information[last_selected_index]->audio_path != shown_information[selected_index]->audio_path)
			{
				SoundManager::instance()->GetProcess("default_music")->is_playing = false;
				SoundManager::instance()->clear(shown_information[last_selected_index]->audio_path);
				PrepareHeader::instance()->SetInformation(shown_information[selected_index]);
				SoundManager::instance()->load(shown_information[selected_index]->audio_path);
				SoundManager::instance()->GetProcess("default_music")->path = shown_information[selected_index]->audio_path;
				SoundManager::instance()->play("default_music");
				SoundManager::instance()->seek("default_music", shown_information[selected_index]->preview_time);
			}
			PrepareHeader::instance()->SetInformation(shown_information[selected_index]);
		}
		last_selected_index = selected_index;
	}
	List::SetValueCount(shown_information.size());
}	//void fr::SongList::update()

void fr::SongList::render()
{
	List::render();
	search_bar->render();
}

void fr::SongList::PushText()
{
	int current_index = current_list_process / cell_h;
	for (int i = 0; i < cell.size(); i++)
	{
		if (shown_information.size() != 0)
		{
			if (current_index >= value_count)
			{
				current_index = 0;
			}
			char *difficulty_ch = new char[3];
			sprintf(difficulty_ch, "%d", shown_information[current_index]->difficulty);

			if (current_index == selected_index)
			{
				cell[i]->GetText(0)->SetText(difficulty_ch);
				cell[i]->GetText(0)->SetColor(0xFF, 0xFF, 0xFF);
				cell[i]->GetText(1)->SetText(shown_information[current_index]->title);
				cell[i]->GetText(1)->SetColor(0xFF, 0xFF, 0xFF);
			}
			else
			{
				cell[i]->GetText(0)->SetText(difficulty_ch);
				cell[i]->GetText(0)->SetColor(0x00, 0x00, 0x00);
				cell[i]->GetText(1)->SetText(shown_information[current_index]->title);
				cell[i]->GetText(1)->SetColor(0x00, 0x00, 0x00);
			}

			delete [] difficulty_ch;
			current_index++;
		}
		else
		{
			cell[i]->GetText(0)->SetText("??");
			cell[i]->GetText(0)->SetColor(0xFF, 0xFF, 0xFF);
			cell[i]->GetText(1)->SetText(null_information->title);
			cell[i]->GetText(1)->SetColor(0xFF, 0xFF, 0xFF);
			PrepareHeader::instance()->SetInformation(null_information);
			list_length = cell_h;
		}
	}
}

bool fr::SongList::LoadList()
{
	m_information.clear();

	std::string text;
	int file_index = 0;
	if (!ReadFile("/sdcard/ForceRush/song_list.fa", text))
	{
		return false;
	}

	while (file_index < text.size())
	{
		std::string line;
		int char_index;
		for (char_index = 0; text[file_index + char_index] != 10; char_index++)
		{
			if (text[file_index + char_index] != 13 && text[file_index + char_index] != '\t')
			{
				line += text[file_index + char_index];
			}
			if (file_index + char_index + 1 == text.size())
			{
				break;
			}
		}
		file_index += char_index + 1;
		if (line.compare(0, 1, "[") == 0)
		{
			std::string brace_line;
			SongInformation *new_information = new SongInformation;
			Score *new_score = new Score;
			while (brace_line.compare(0, 1, "}") != 0)
			{
				brace_line = "";
				for (char_index = 0; text[file_index + char_index] != 10; char_index++)
				{
					if (text[file_index + char_index] != 13 && text[file_index + char_index] != '\t')
					{
						brace_line += text[file_index + char_index];
					}
					if (file_index + char_index + 1 == text.size())
					{
						break;
					}
				}
				file_index += char_index + 1;
				if (brace_line.compare(0, 1, "[") == 0)
				{
					int mark_index = brace_line.find(']');
					new_information->id = brace_line.substr(1, mark_index - 1);
				}
				if (brace_line.compare(0, 6, "title:") == 0)
				{
					new_information->title = brace_line.substr(6);
				}
				if (brace_line.compare(0, 7, "artist:") == 0)
				{
					new_information->artist = brace_line.substr(7);
				}
				if (brace_line.compare(0, 6, "noter:") == 0)
				{
					new_information->noter = brace_line.substr(6);
				}
				if (brace_line.compare(0, 8, "version:") == 0)
				{
					new_information->version = brace_line.substr(8);
				}
				if (brace_line.compare(0, 11, "difficulty:") == 0)
				{
					new_information->difficulty = atoi(brace_line.substr(11).c_str());
				}
				if (brace_line.compare(0, 9, "duration:") == 0)
				{
					new_information->duration = atoi(brace_line.substr(9).c_str());
				}
				if (brace_line.compare(0, 11, "audio_path:") == 0)
				{
					new_information->audio_path = brace_line.substr(11);
				}
				if (brace_line.compare(0, 13, "preview_time:") == 0)
				{
					new_information->preview_time = atoi(brace_line.substr(13).c_str());
				}
				if (brace_line.compare(0, 10, "file_path:") == 0)
				{
					new_information->file_path = brace_line.substr(10);
				}
				if (brace_line.compare(0, 11, "full_score:") == 0)
				{
					new_information->full_score = atoi(brace_line.substr(11).c_str());
				}
				if (brace_line.compare(0, 6, "score:") == 0)
				{
					new_score->score = atoi(brace_line.substr(5).c_str());
				}
				if (brace_line.compare(0, 5, "pure:") == 0)
				{
					new_score->pure = atoi(brace_line.substr(5).c_str());
				}
				if (brace_line.compare(0, 5, "safe:") == 0)
				{
					new_score->safe = atoi(brace_line.substr(5).c_str());
				}
				if (brace_line.compare(0, 8, "warning:") == 0)
				{
					new_score->warning = atoi(brace_line.substr(8).c_str());
				}
				if (brace_line.compare(0, 6, "error:") == 0)
				{
					new_score->error = atoi(brace_line.substr(6).c_str());
				}
				if (brace_line.compare(0, 6, "chain:") == 0)
				{
					new_score->chain = atoi(brace_line.substr(6).c_str());
				}
				if (brace_line.compare(0, 5, "rank:") == 0)
				{
					new_score->rank = Rank(atoi(brace_line.substr(5).c_str()));
				}
			}
			new_information->high_score = new_score;
			m_information.push_back(new_information);
		}
	}
	shown_information = m_information;
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
	WriteFile("/sdcard/ForceRush/song_list.fa", output_text);
}

void fr::SongList::RefreshList()
{
	is_refreshing = true;

	std::vector<File*> file;
	std::vector<std::string> list_path;
	Setting::instance()->GetSongList(list_path);
	for (int i = 0; i < list_path.size(); i++)
	{
		FindFile(list_path[i], ".osu", file);
		FindFile(list_path[i], ".mc", file);
		FindFile(list_path[i], ".imd", file);
	}

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

		if (LoadBeatmapFile(file[i]->name, new_song_information, NULL))
		{
			m_information.push_back(new_song_information);
			SwitchSort(sort_type, is_reverse);
			//就当是刷新列表了

//			PrepareHeader::instance()->SetInformation(m_information[selected_index]);
			WriteList();
			//實時寫入到緩存文件，中途退出回來刷新的時可以繼續進度
		}
		else
		{
			delete new_song_information;
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
	List::RefreshListSize();
	for (int i = 0; i < cell.size(); i++)
	{
		cell[i]->AddText("??", 24, 16, "assets/fonts/Ubuntu-M.ttf", 32, 0xFF, 0xFF, 0xFF, TEXTFORMAT_LEFT, 40);
		cell[i]->AddText("NULL", 96, 16, "assets/fonts/Ubuntu-M.ttf", 32, 0xFF, 0xFF, 0xFF, TEXTFORMAT_LEFT, 616);
	}
}

void fr::SongList::SwitchSort(SortType type, bool reverse)
{
	is_reverse = reverse;
	sort_type = type;
	switch (type)
	{
		case SORTTYPE_DEFAULT:
			shown_information = m_information;
		break;
		case SORTTYPE_ARTIST:
			shown_information = m_information;
			std::sort(shown_information.begin(), shown_information.end(), CompareArtist);
		break;
		case SORTTYPE_NOTER:
			shown_information = m_information;
			std::sort(shown_information.begin(), shown_information.end(), CompareNoter);
		break;
		case SORTTYPE_TITLE:
			shown_information = m_information;
			std::sort(shown_information.begin(), shown_information.end(), CompareTitle);
		break;
		case SORTTYPE_DIFFICULTY:
			shown_information = m_information;
			std::sort(shown_information.begin(), shown_information.end(), CompareDifficulty);
		break;
		case SORTTYPE_DURATION:
			shown_information = m_information;
			std::sort(shown_information.begin(), shown_information.end(), CompareDuration);
		break;
	}
}

void fr::SongList::RollList()
{
	
}

void fr::SongList::Search(std::string pattern_str)
{
	
}

bool fr::SongList::IsReverse()
{
	return is_reverse;
}

fr::SortType fr::SongList::GetSortType()
{
	return sort_type;
}