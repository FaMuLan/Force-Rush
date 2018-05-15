#include "song_list.h"
#include <vector>
#include <regex>
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
		SoundManager::instance()->load(m_information[selected_index]->audio_path, SOUNDTYPE_MUSIC);
		SoundManager::instance()->play(m_information[selected_index]->audio_path, m_information[selected_index]->preview_time);
	}
	RefreshListSize();
}	//void fr::SongList::inìt()

void fr::SongList::clear()
{
	cell.clear();
	if (SoundManager::instance()->IsPlayingMusic())
	{
		SoundManager::instance()->stop();
	SoundManager::instance()->clear(m_information[selected_index]->audio_path, SOUNDTYPE_MUSIC);
	}
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

			if (IsConfirmed())
			{
				LoadingState::instance()->init(STATE_GAME);
				GameState::instance()->SetFile(shown_information[selected_index]);
				SoundManager::instance()->stop();
			}
			else 
			{
				if (shown_information[last_selected_index]->audio_path != shown_information[selected_index]->audio_path)
				{
					if (SoundManager::instance()->IsPlayingMusic())
					{
						SoundManager::instance()->stop();
						SoundManager::instance()->clear(shown_information[last_selected_index]->audio_path, SOUNDTYPE_MUSIC);
					}
					PrepareHeader::instance()->SetInformation(shown_information[selected_index]);
					SoundManager::instance()->load(shown_information[selected_index]->audio_path, SOUNDTYPE_MUSIC);
					SoundManager::instance()->play(shown_information[selected_index]->audio_path, shown_information[selected_index]->preview_time);
				}
				PrepareHeader::instance()->SetInformation(shown_information[selected_index]);
			}
			last_selected_index = selected_index;
		}
		else
		{
			cell[i]->GetText(0)->SetText("??");
			cell[i]->GetText(0)->SetColor(0x00, 0x00, 0x00);
			cell[i]->GetText(1)->SetText(null_information->title);
			cell[i]->GetText(1)->SetColor(0x00, 0x00, 0x00);
			PrepareHeader::instance()->SetInformation(null_information);
			list_length = cell_h;
		}
	}
	List::SetValueCount(shown_information.size());
}	//void fr::SongList::update()

void fr::SongList::render()
{
	List::render();
	search_bar->render();
}

bool fr::SongList::LoadList()
{
	m_information.clear();

	std::string text;
//	std::regex pattern("\\[(.*?)\\]\\s*\\{\\s*title:(.*)\\s*artist:(.*)\\s*noter:(.*)\\s*version:(.*)\\s*difficulty:(\\d*)\\s*duration:(\\d*)\\s*audio_path:(.*)\\s*preview_time:(\\d*)\\s*file_path:(.*)\\s*score:(\\d+)\\s*rank:(\\d)\\s*\\}");
	std::regex song_pattern("\\[(.*?)\\]\\s*?\\{[\\S\\s]*?\\}");
	//包含ID信息在內，先截取其中一段再單獨進行提取
	std::regex title_pattern("\\ttitle:(.*?)\\n");
	std::regex artist_pattern("\\tartist:(.*?)\\n");
	std::regex noter_pattern("\\tnoter:(.*?)\\n");
	std::regex version_pattern("\\tversion:(.*?)\\n");
	std::regex difficulty_pattern("\\tdifficulty:(\\d*?)\\n");
	std::regex duration_pattern("\\tduration:(\\d*?)\\n");
	std::regex audio_path_pattern("\\taudio_path:(.*?)\\n");
	std::regex preview_time_pattern("\\tpreview_time:(\\d*?)\\n");
	std::regex file_path_pattern("\\tfile_path:(.*?)\\n");
	std::regex full_score_pattern("\\tfull_score:(\\d*?)\\n");

	std::regex score_pattern("\\tscore:(\\d*?)\\n");
	std::regex pure_pattern("\\tpure:(\\d*?)\\n");
	std::regex safe_pattern("\\tsafe:(\\d*?)\\n");
	std::regex warning_pattern("\\twarning:(\\d*?)\\n");
	std::regex error_pattern("\\terror:(\\d*?)\\n");
	std::regex chain_pattern("\\tchain:(\\d*?)\\n");
	std::regex rank_pattern("\\trank:(\\d)");

	if (!ReadFile("/sdcard/ForceRush/song_list.fa", text))
	{
		return false;
	}
//	if (!std::regex_search(text, pattern))
//	{
//		return false;
//	}
	for (std::sregex_iterator i = std::sregex_iterator(text.begin(), text.end(), song_pattern); i != std::sregex_iterator(); i++)
	{
		std::smatch paragraph_match = *i;
		std::string paragraph = paragraph_match.str();
		std::smatch title_line;
		std::smatch artist_line;
		std::smatch noter_line;
		std::smatch version_line;
		std::smatch difficulty_line;
		std::smatch duration_line;
		std::smatch audio_path_line;
		std::smatch preview_time_line;
		std::smatch file_path_line;
		std::smatch full_score_line;
		std::smatch score_line;
		std::smatch pure_line;
		std::smatch safe_line;
		std::smatch warning_line;
		std::smatch error_line;
		std::smatch chain_line;
		std::smatch rank_line;

		std::regex_search(paragraph, title_line, title_pattern);
		std::regex_search(paragraph, artist_line, artist_pattern);
		std::regex_search(paragraph, noter_line, noter_pattern);
		std::regex_search(paragraph, version_line, version_pattern);
		std::regex_search(paragraph, difficulty_line, difficulty_pattern);
		std::regex_search(paragraph, duration_line, duration_pattern);
		std::regex_search(paragraph, audio_path_line, audio_path_pattern);
		std::regex_search(paragraph, preview_time_line, preview_time_pattern);
		std::regex_search(paragraph, file_path_line, file_path_pattern);
		std::regex_search(paragraph, full_score_line, full_score_pattern);
		std::regex_search(paragraph, score_line, score_pattern);
		std::regex_search(paragraph, pure_line, pure_pattern);
		std::regex_search(paragraph, safe_line, safe_pattern);
		std::regex_search(paragraph, warning_line, warning_pattern);
		std::regex_search(paragraph, error_line, error_pattern);
		std::regex_search(paragraph, chain_line, chain_pattern);
		std::regex_search(paragraph, rank_line, rank_pattern);
		SongInformation *new_information = new SongInformation;
		Score *new_score = new Score;
		new_information->id = std::regex_replace(paragraph, song_pattern, "$1");
		new_information->title = std::regex_replace(title_line.str(), title_pattern, "$1");
		new_information->artist = std::regex_replace(artist_line.str(), artist_pattern, "$1");
		new_information->noter = std::regex_replace(noter_line.str(), noter_pattern, "$1");
		new_information->version = std::regex_replace(version_line.str(), version_pattern, "$1");
		new_information->difficulty = atoi(std::regex_replace(difficulty_line.str(), difficulty_pattern, "$1").c_str());
		new_information->duration = atoi(std::regex_replace(duration_line.str(), duration_pattern, "$1").c_str());
		new_information->audio_path = std::regex_replace(audio_path_line.str(), audio_path_pattern, "$1");
		new_information->preview_time = atoi(std::regex_replace(preview_time_line.str(), preview_time_pattern, "$1").c_str());
		new_information->file_path = std::regex_replace(file_path_line.str(), file_path_pattern, "$1");
		new_information->full_score = atoi(std::regex_replace(full_score_line.str(), full_score_pattern, "$1").c_str());
		new_score->score = atoi(std::regex_replace(score_line.str(), score_pattern, "$1").c_str());
		new_score->pure = atoi(std::regex_replace(pure_line.str(), pure_pattern, "$1").c_str());
		new_score->safe = atoi(std::regex_replace(safe_line.str(), safe_pattern, "$1").c_str());
		new_score->warning = atoi(std::regex_replace(warning_line.str(), warning_pattern, "$1").c_str());
		new_score->error = atoi(std::regex_replace(error_line.str(), error_pattern, "$1").c_str());
		new_score->chain = atoi(std::regex_replace(chain_line.str(), chain_pattern, "$1").c_str());
		new_score->rank = Rank(atoi(std::regex_replace(rank_line.str(), rank_pattern, "$1").c_str()));
		new_information->high_score = new_score;
		m_information.push_back(new_information);
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

		if (LoadBeatmapFile(file[i]->name, new_song_information, NULL))
		{
			m_information.push_back(new_song_information);
			shown_information.push_back(new_song_information);
			SwitchSort(sort_type, is_reverse);
			//就当是刷新列表了

//			PrepareHeader::instance()->SetInformation(m_information[selected_index]);
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