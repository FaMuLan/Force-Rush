#include "file_system.h"
#include <dirent.h>
#include <fstream>
#include <algorithm>
#include <SDL2/SDL.h>
#include <map>
#include <regex>
#include "song_data.h"

bool fr::ReadFile(std::string path, std::string &output)
{
	std::ifstream file;
	file.open(path);
	if (!file)
	{
		return false;
	}
	while (!file.eof())
	{
		output += file.get();
	}
	file.close();
	return true;
}

bool fr::WriteFile(std::string path, std::string &output)
{
	std::ofstream file;
	file.open(path);
	file << output;
	file.close();
	return true;
}

bool fr::ListDir(std::string path, std::vector<fr::File*> &output)
{
	DIR *dir = opendir(path.c_str());
	if (dir == NULL)
	{
		return false;
	}
	dirent *file = NULL;
	while ((file = readdir(dir)) != NULL)
	{
		if (file->d_name[0] != '.')
		{
			File *new_file = new File;
			new_file->name = path + "/" + std::string(file->d_name);
			new_file->type = file->d_type == 8 ? FILETYPE_FILE : (file->d_type == 4 ? FILETYPE_DIR : FILETYPE_LINK);
			output.push_back(new_file);
		}
	}
	return true;
}

void fr::FindFile(std::string path, std::string pattern_str, std::vector<File*> &output)
{
	std::vector<File*> current_list;
	ListDir(path, current_list);
	std::regex pattern(pattern_str); 
	for (int i = 0; i < current_list.size(); i++)
	{
		if (std::regex_match(current_list[i]->name, pattern))
		{
			output.push_back(current_list[i]);
		}
		else if (current_list[i]->type == FILETYPE_DIR)
		{
			FindFile(current_list[i]->name, pattern_str, output);
			//使用遞歸讀取子目錄下的文件
		}
	}
}

std::string fr::GetParentDir(std::string path)
{
	std::regex parent_dir_pattern("(.*/)[^/]*");
	return regex_replace(path, parent_dir_pattern, "$1");
}

bool fr::LoadBeatmapFile(std::string path, SongInformation *output_information, std::vector<Note*> *output_note_list1, std::vector<Note*> *output_note_list2, std::vector<Note*> *output_note_list3, std::vector<Note*> *output_note_list4)
{
	std::regex path_pattern(".*\\.(.*)");
	std::string format = std::regex_replace(path, path_pattern, "$1");
	bool success = true;
	if (format == "imd")
	{
		success = LoadIMDFile(path, output_information, output_note_list1, output_note_list2, output_note_list3, output_note_list4);
	}
	else
	{
		success = LoadOSUFile(path, output_information, output_note_list1, output_note_list2, output_note_list3, output_note_list4);
	}
	return success;
}

bool fr::LoadOSUFile(std::string path, fr::SongInformation *output_information, std::vector<Note*> *output_note_list1, std::vector<Note*> *output_note_list2, std::vector<Note*> *output_note_list3, std::vector<Note*> *output_note_list4)
{

	static std::regex id_pattern("BeatmapID:(.*?)[\\n\\r]");
	static std::regex title_pattern("Title:(.*?)[\\n\\r]");
	static std::regex artist_pattern("Artist:(.*?)[\\n\\r]");
	static std::regex noter_pattern("Creator:(.*?)[\\n\\r]");
	static std::regex version_pattern("Version:(.*?)[\\n\\r]");
	static std::regex mode_pattern("Mode: (\\d)");
	static std::regex key_count_pattern("CircleSize:(\\d)");
	static std::regex audio_path_pattern("AudioFilename: (.*?)[\\n\\r]");
	static std::regex preview_time_pattern("PreviewTime: (\\d*)");
	static std::regex note_pattern("(\\d+?),\\d+?,(\\d+?),(\\d+?),\\d+?,(\\d+?):\\d+?:\\d+?:\\d+?:(\\d+?:)?");

	bool success = true;

	bool load_information = output_information != NULL;
	bool load_note = output_note_list1 != NULL && output_note_list2 != NULL && output_note_list3 != NULL && output_note_list4 != NULL;

	int note_count = 0;
	int key_count = 0;
	int mode = 0;
	std::vector<float> note_piece_count;
	int note_piece_index = 0;
	note_piece_count.push_back(0);

	std::string text;
	ReadFile(path, text);

	if (load_information)
	{
		output_information->file_path = path;
		std::smatch id_line;
		std::smatch title_line;
		std::smatch artist_line;
		std::smatch noter_line;
		std::smatch version_line;
		std::smatch mode_line;
		std::smatch key_count_line;
		std::smatch audio_path_line;
		std::smatch preview_time_line;

		std::regex_search(text, id_line, id_pattern);
		std::regex_search(text, title_line, title_pattern);
		std::regex_search(text, artist_line, artist_pattern);
		std::regex_search(text, noter_line, noter_pattern);
		std::regex_search(text, version_line, version_pattern);
		std::regex_search(text, mode_line, mode_pattern);
		std::regex_search(text, key_count_line, key_count_pattern);
		std::regex_search(text, audio_path_line, audio_path_pattern);
		std::regex_search(text, preview_time_line, preview_time_pattern);

		output_information->id = "osu" +  std::regex_replace(id_line.str(), id_pattern, "$1");
		output_information->title = std::regex_replace(title_line.str(), title_pattern, "$1");
		output_information->artist = std::regex_replace(artist_line.str(), artist_pattern, "$1");
		output_information->noter = std::regex_replace(noter_line.str(), noter_pattern, "$1");
		output_information->version = std::regex_replace(version_line.str(), version_pattern, "$1");
		output_information->audio_path = GetParentDir(output_information->file_path) + std::regex_replace(audio_path_line.str(), audio_path_pattern, "$1");
		output_information->preview_time = atoi(std::regex_replace(preview_time_line.str(), preview_time_pattern, "$1").c_str());
		output_information->full_score = 0;
		success = success && (atoi(std::regex_replace(mode_line.str(), mode_pattern, "$1").c_str()) == 3);
		success = success && (atoi(std::regex_replace(key_count_line.str(), key_count_pattern, "$1").c_str()) == 4);

		if (!success)
		{
			return false;
		}
	}

	for (std::sregex_iterator i = std::sregex_iterator(text.begin(), text.end(), note_pattern); i != std::sregex_iterator(); i++)
	{
		std::smatch note_line = *i;
		int time = atoi(std::regex_replace(note_line.str(), note_pattern, "$2").c_str());
		int type = atoi(std::regex_replace(note_line.str(), note_pattern, "$3").c_str());
		int time_end = (type % 16 == 0) ? atoi(std::regex_replace(note_line.str(), note_pattern, "$4").c_str()) : time;
		int column_index = atoi(std::regex_replace(note_line.str(), note_pattern, "$1").c_str());
		if (load_note)
		{
			Note *new_note = new Note;
			new_note->time = time + 2000;
			new_note->time_end = time_end + 2000;
			new_note->type = NOTETYPE_NORMAL;
			new_note->type_end = NOTETYPE_NORMAL;
			switch (column_index)
			{
				case 64:
					output_note_list1->push_back(new_note);
				break;
				case 192:
					output_note_list2->push_back(new_note);
				break;
				case 320:
					output_note_list3->push_back(new_note);
				break;
				case 448:
					output_note_list4->push_back(new_note);
				break;
			}
		}
		if (load_information)
		{
			output_information->duration = time;
			while (note_piece_index * 1000 < time)
			{
				note_piece_index++;
				note_piece_count.push_back(0);
			}
			note_piece_count[note_piece_index]++;
			output_information->full_score += (type % 16 == 0) ? 4 : 2;
		}
	}
	if (load_information)
	{
		std::sort(note_piece_count.begin(), note_piece_count.end());
		output_information->difficulty = note_piece_count[int(note_piece_index * 0.6f)];
		Score *new_null_score = new Score;
		new_null_score->rank = RANK_NONE;
		new_null_score->score = 0;
		new_null_score->pure = 0;
		new_null_score->safe = 0;
		new_null_score->warning = 0;
		new_null_score->error = 0;
		new_null_score->chain = 0;
		output_information->high_score = new_null_score;
	}
	if (load_note)
	{
		std::sort(output_note_list1->begin(), output_note_list1->end(), CompareNote);
		std::sort(output_note_list2->begin(), output_note_list2->end(), CompareNote);
		std::sort(output_note_list3->begin(), output_note_list3->end(), CompareNote);
		std::sort(output_note_list4->begin(), output_note_list4->end(), CompareNote);
		//信不過來排序
	}
	return true;
}

bool fr::LoadIMDFile(std::string path, SongInformation *output_information, std::vector<Note*> *output_note_list1, std::vector<Note*> *output_note_list2, std::vector<Note*> *output_note_list3, std::vector<Note*> *output_note_list4)
{
	static std::regex path_pattern(".*/(.*?)_(\\d)k_(.*?)\\.imd");
	bool success = true;
	bool load_information = output_information != NULL;
	bool load_note = output_note_list1 != NULL && output_note_list2 != NULL && output_note_list3 != NULL && output_note_list4 != NULL;
	SDL_RWops *file = SDL_RWFromFile(path.c_str(), "r+b");
	//死因:不會用fstream讀取二進制文件
	int note_count = 0;
	int beat_count = 0;
	int temp = 0;

	std::vector<float> note_piece_count;
	int note_piece_index = 0;
	note_piece_count.push_back(0);

	if (load_information)
	{
		output_information->file_path = path;
		output_information->title = std::regex_replace(path, path_pattern, "$1");
		std::string track_count = std::regex_replace(path, path_pattern, "$2");
		output_information->version = std::regex_replace(path, path_pattern, "IMD $3");
		output_information->audio_path = GetParentDir(output_information->file_path) + output_information->title + ".mp3";
		output_information->preview_time = 0;
		if (track_count != "4")
		{
			success = false;
		}

		SDL_RWread(file, &output_information->duration, 4, 1);

		SDL_RWread(file, &beat_count, 4, 1);
		for (int i = 0; i < beat_count; i++)
		{
			SDL_RWread(file, &temp, 4, 1);
			SDL_RWread(file, &temp, 4, 1);
			SDL_RWread(file, &temp, 4, 1);
		}
		SDL_RWread(file, &temp, 2, 1);
		SDL_RWread(file, &note_count, 4, 1);
		output_information->artist = "Unknown";
		output_information->noter = "Unknown";
		output_information->full_score = 0;

		Score *new_null_score = new Score;
		new_null_score->rank = RANK_NONE;
		new_null_score->score = 0;
		new_null_score->pure = 0;
		new_null_score->safe = 0;
		new_null_score->warning = 0;
		new_null_score->error = 0;
		new_null_score->chain = 0;
		output_information->high_score = new_null_score;
	}
	else
	{
		SDL_RWread(file, &temp, 4, 1);

		SDL_RWread(file, &beat_count, 4, 1);
		for (int i = 0; i < beat_count; i++)
		{
			SDL_RWread(file, &temp, 4, 1);
			SDL_RWread(file, &temp, 4, 1);
			SDL_RWread(file, &temp, 4, 1);
		}
		SDL_RWread(file, &temp, 2, 1);
		SDL_RWread(file, &note_count, 4, 1);
	}
	Note *last_note1;
	Note *last_note2;
	Note *last_note3;
	Note *last_note4;
	for (int i = 0; i < note_count; i++)
	{
		short action;
		int time_stamp;
		char track;
		int time_span;
		SDL_RWread(file, &action, 2, 1);
		SDL_RWread(file, &time_stamp, 4, 1);
		SDL_RWread(file, &track, 1, 1);
		SDL_RWread(file, &time_span, 4, 1);

		if (load_information)
		{
			while (note_piece_index * 1000 < time_stamp)
			{
				note_piece_index++;
				note_piece_count.push_back(0);
			}
			note_piece_count[note_piece_index] += 1;
			switch (action)
			{
				case 0x0000:
				case 0x0001:
				case 0x0061:
				case 0x0062:
				case 0x0021:
				case 0x00A1:
				case 0x00A2:
					output_information->full_score += 2;
				break;
				case 0x0002:
					output_information->full_score += 4;
				break;
			}
		}

		if (load_note)
		{
			switch (action)
			{
				case 0x0000:	//單鍵
				{
					Note *new_note = new Note;
					new_note->type = NOTETYPE_NORMAL;
					new_note->type_end = NOTETYPE_NORMAL;
					new_note->time = time_stamp + 2000;
					new_note->time_end = new_note->time;
					switch (track)
					{
						case 0x00:
							output_note_list1->push_back(new_note);
						break;
						case 0x01:
							output_note_list2->push_back(new_note);
						break;
						case 0x02:
							output_note_list3->push_back(new_note);
						break;
						case 0x03:
							output_note_list4->push_back(new_note);
						break;
					}
				}
				break;
				case 0x0001:	//滑鍵
				{
					Note *new_start_note = new Note;
					new_start_note->type = NOTETYPE_NORMAL;
					new_start_note->type_end = NOTETYPE_NORMAL;
					new_start_note->time = time_stamp + 2000;
					new_start_note->time_end = new_start_note->time;
					switch (track)
					{
						case 0x00:
							output_note_list1->push_back(new_start_note);
						break;
						case 0x01:
							output_note_list2->push_back(new_start_note);
						break;
						case 0x02:
							output_note_list3->push_back(new_start_note);
						break;
						case 0x03:
							output_note_list4->push_back(new_start_note);
						break;
					}
					for (char i = track; i != track + time_span;)
					{
						Note *new_slide_note = new Note;
						i += time_span > 0 ? 1 : -1;
						if (i == track + time_span)
						{
							new_slide_note->type = time_span > 0 ? NOTETYPE_SLIDE_END_RIGHT : NOTETYPE_SLIDE_END_LEFT;
						}
						else
						{
							new_slide_note->type = NOTETYPE_SLIDE_THROUGH;
						}
						new_slide_note->time = time_stamp + 2000;
						new_slide_note->time_end = new_start_note->time;
						switch (i)
						{
							case 0x00:
								output_note_list1->push_back(new_slide_note);
							break;
							case 0x01:
								output_note_list2->push_back(new_slide_note);
							break;
							case 0x02:
								output_note_list3->push_back(new_slide_note);
							break;
							case 0x03:
								output_note_list4->push_back(new_slide_note);
							break;
						}
					}
				}
				break;
				case 0x0002:	//長條
				{
					Note *new_note = new Note;
					new_note->type = NOTETYPE_NORMAL;
					new_note->type_end = NOTETYPE_NORMAL;
					new_note->time = time_stamp + 2000;
					new_note->time_end = new_note->time + time_span;
					switch (track)
					{
						case 0x00:
							output_note_list1->push_back(new_note);
						break;
						case 0x01:
							output_note_list2->push_back(new_note);
						break;
						case 0x02:
							output_note_list3->push_back(new_note);
						break;
						case 0x03:
							output_note_list4->push_back(new_note);
						break;
					}
				}
				break;
				case 0x0061:	//長滑條開頭(橫滑)
				{
					Note *new_start_note = new Note;
					new_start_note->type = NOTETYPE_NORMAL;
					new_start_note->type_end = NOTETYPE_NORMAL;
					new_start_note->time = time_stamp + 2000;
					new_start_note->time_end = new_start_note->time;
					switch (track)
					{
						case 0x00:
							output_note_list1->push_back(new_start_note);
						break;
						case 0x01:
							output_note_list2->push_back(new_start_note);
						break;
						case 0x02:
							output_note_list3->push_back(new_start_note);
						break;
						case 0x03:
							output_note_list4->push_back(new_start_note);
						break;
					}
					for (char i = track; i != track + time_span;)
					{
						Note *new_slide_note = new Note;
						i += time_span > 0 ? 1 : -1;
						if (i == track + time_span)
						{
							new_slide_note->type = time_span > 0 ? NOTETYPE_SLIDE_IN_RIGHT : NOTETYPE_SLIDE_IN_LEFT;
						}
						else
						{
							new_slide_note->type = NOTETYPE_SLIDE_THROUGH;
						}
						new_slide_note->type_end = NOTETYPE_NORMAL;
						new_slide_note->time = time_stamp + 2000;
						new_slide_note->time_end = new_start_note->time;
						switch (i)
						{
							case 0x00:
								output_note_list1->push_back(new_slide_note);
								last_note1 = new_slide_note;
							break;
							case 0x01:
								output_note_list2->push_back(new_slide_note);
								last_note2 = new_slide_note;
							break;
							case 0x02:
								output_note_list3->push_back(new_slide_note);
								last_note3 = new_slide_note;
							break;
							case 0x03:
								output_note_list4->push_back(new_slide_note);
								last_note4 = new_slide_note;
							break;
						}
					}
				}
				break;
				case 0x0062:	//長滑條開頭(直行)
				{
					Note *new_note = new Note;
					new_note->type = NOTETYPE_NORMAL;
					new_note->type_end = NOTETYPE_NORMAL;
					new_note->time = time_stamp + 2000;
					new_note->time_end = new_note->time + time_span;
					switch (track)
					{
						case 0x00:
							output_note_list1->push_back(new_note);
							last_note1 = new_note;
						break;
						case 0x01:
							output_note_list2->push_back(new_note);
							last_note2 = new_note;
						break;
						case 0x02:
							output_note_list3->push_back(new_note);
							last_note3 = new_note;
						break;
						case 0x03:
							output_note_list4->push_back(new_note);
							last_note4 = new_note;
						break;
					}
				}
				break;
				case 0x0021:	//長滑條中繼(橫滑)
				{
					switch (track)
					{
						case 0x00:
							last_note1->type_end = time_span > 0 ? NOTETYPE_SLIDE_OUT_RIGHT : NOTETYPE_SLIDE_OUT_LEFT;
						break;
						case 0x01:
							last_note2->type_end = time_span > 0 ? NOTETYPE_SLIDE_OUT_RIGHT : NOTETYPE_SLIDE_OUT_LEFT;
						break;
						case 0x02:
							last_note3->type_end = time_span > 0 ? NOTETYPE_SLIDE_OUT_RIGHT : NOTETYPE_SLIDE_OUT_LEFT;
						break;
						case 0x03:
							last_note4->type_end = time_span > 0 ? NOTETYPE_SLIDE_OUT_RIGHT : NOTETYPE_SLIDE_OUT_LEFT;
						break;
					}
					for (char i = track; i != track + time_span;)
					{
						Note *new_slide_note = new Note;
						i += time_span > 0 ? 1 : -1;
						if (i == track + time_span)
						{
							new_slide_note->type = time_span > 0 ? NOTETYPE_SLIDE_IN_RIGHT : NOTETYPE_SLIDE_IN_LEFT;
						}
						else
						{
							new_slide_note->type = NOTETYPE_SLIDE_THROUGH;
						}
						new_slide_note->type_end = NOTETYPE_NORMAL;
						new_slide_note->time = time_stamp + 2000;
						new_slide_note->time_end = new_slide_note->time;
						switch (i)
						{
							case 0x00:
								output_note_list1->push_back(new_slide_note);
								last_note1 = new_slide_note;
							break;
							case 0x01:
								output_note_list2->push_back(new_slide_note);
								last_note2 = new_slide_note;
							break;
							case 0x02:
								output_note_list3->push_back(new_slide_note);
								last_note3 = new_slide_note;
							break;
							case 0x03:
								output_note_list4->push_back(new_slide_note);
								last_note4 = new_slide_note;
							break;
						}
					}
				}
				break;
				case 0x0022:	//長滑條中繼(直行)
					switch (track)
					{
						case 0x00:
							last_note1->time_end = last_note1->time + time_span;
						break;
						case 0x01:
							last_note2->time_end = last_note2->time + time_span;
						break;
						case 0x02:
							last_note3->time_end = last_note3->time + time_span;
						break;
						case 0x03:
							last_note4->time_end = last_note4->time + time_span;
						break;
					}
				break;
				case 0x00A1:	//長滑條結束(橫滑)
					switch (track)
					{
						case 0x00:
							last_note1->type_end = time_span > 0 ? NOTETYPE_SLIDE_OUT_RIGHT : NOTETYPE_SLIDE_OUT_LEFT;
						break;
						case 0x01:
							last_note2->type_end = time_span > 0 ? NOTETYPE_SLIDE_OUT_RIGHT : NOTETYPE_SLIDE_OUT_LEFT;
						break;
						case 0x02:
							last_note3->type_end = time_span > 0 ? NOTETYPE_SLIDE_OUT_RIGHT : NOTETYPE_SLIDE_OUT_LEFT;
						break;
						case 0x03:
							last_note4->type_end = time_span > 0 ? NOTETYPE_SLIDE_OUT_RIGHT : NOTETYPE_SLIDE_OUT_LEFT;
						break;
					}
					for (char i = track; i != track + time_span;)
					{
						Note *new_slide_note = new Note;
						i += time_span > 0 ? 1 : -1;
						if (i == track + time_span)
						{
							new_slide_note->type = time_span > 0 ? NOTETYPE_SLIDE_END_RIGHT : NOTETYPE_SLIDE_END_LEFT;
						}
						else
						{
							new_slide_note->type = NOTETYPE_SLIDE_THROUGH;
						}
						new_slide_note->type_end = NOTETYPE_NORMAL;
						new_slide_note->time = time_stamp + 2000;
						new_slide_note->time_end = new_slide_note->time;
						switch (i)
						{
							case 0x00:
								output_note_list1->push_back(new_slide_note);
							break;
							case 0x01:
								output_note_list2->push_back(new_slide_note);
							break;
							case 0x02:
								output_note_list3->push_back(new_slide_note);
							break;
							case 0x03:
								output_note_list4->push_back(new_slide_note);
							break;
						}
					}
				break;
				case 0x00A2:	//長滑條結束(直行)
				{
					switch (track)
					{
						case 0x00:
							last_note1->time_end = last_note1->time + time_span;
						break;
						case 0x01:
							last_note2->time_end = last_note2->time + time_span;
						break;
						case 0x02:
							last_note3->time_end = last_note3->time + time_span;
						break;
						case 0x03:
							last_note4->time_end = last_note4->time + time_span;
						break;
					}
				}
				break;
			}	//switch (action)
		}	//if (load_note)
	}

	if (load_note)
	{
		std::sort(output_note_list1->begin(), output_note_list1->end(), CompareNote);
		std::sort(output_note_list2->begin(), output_note_list2->end(), CompareNote);
		std::sort(output_note_list3->begin(), output_note_list3->end(), CompareNote);
		std::sort(output_note_list4->begin(), output_note_list4->end(), CompareNote);
		//對於IMD格式的時間排列……不手動排序的話遊戲中某些配置連auto都救不了
	}

	if (load_information)
	{
		std::sort(note_piece_count.begin(), note_piece_count.end());
		output_information->difficulty = note_piece_count[int(note_piece_index * 0.6f)];
	}

	SDL_FreeRW(file);
	return success;
}