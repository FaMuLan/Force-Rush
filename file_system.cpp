#include "file_system.h"
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <algorithm>
#include <SDL2/SDL.h>
#include <map>
#include <regex>
#include <ratio>
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
	mkdir(GetParentDir(path).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
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
	closedir(dir);
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
	current_list.clear();
	current_list.shrink_to_fit();
}

std::string fr::GetParentDir(std::string path)
{
	std::regex parent_dir_pattern("(.*/)[^/]*");
	return regex_replace(path, parent_dir_pattern, "$1");
}

bool fr::LoadBeatmapFile(std::string path, SongInformation *output_information, std::vector<fr::NoteSet*> *output_note_set)
{
	static std::regex path_pattern(".*\\.(.*?)");
	std::string format = std::regex_replace(path, path_pattern, "$1");
	bool success = true;
	if (format == "imd")
	{
		success = LoadIMDFile(path, output_information, output_note_set);
	}
	if (format == "mc")
	{
		success = LoadMalodyFile(path, output_information, output_note_set);
	}
	if (format == "osu")
	{
		success = LoadOSUFile(path, output_information, output_note_set);
	}
	return success;
}

bool fr::LoadOSUFile(std::string path, fr::SongInformation *output_information, std::vector<fr::NoteSet*> *output_note_set)
{
	bool success = true;

	bool load_information = output_information != NULL;
	bool load_note = output_note_set != NULL;

	std::vector<float> note_piece_count;
	int note_piece_index = 0;
	note_piece_count.push_back(0);

	std::string text;
	int file_index = 0;
	ReadFile(path, text);

	bool is_id_read = false;
	bool is_title_read = false;
	bool is_artist_read = false;
	bool is_noter_read = false;
	bool is_version_read = false;
	bool is_mode_read = false;
	bool is_key_count_read = false;
	bool is_audio_path_read = false;
	bool is_preview_time_read = false;
	int mode = 0;
	int key_count;
	int note_count = 0;
	if (load_note)
	{
		for (int i = 0; i < 4; i++)
		{
			NoteSet *new_note_set = new NoteSet;
			output_note_set->push_back(new_note_set);
		}
	}
	Timeline *last_timeline = NULL;
	int base_bpm = 0;
	while (file_index < text.size())
	{
		std::string line;
		int char_index;
		for (char_index = 0; text[file_index + char_index] != 10; char_index++)
		{
			if (text[file_index + char_index] != 13)
			{
				line += text[file_index + char_index];
			}
			if (file_index + char_index + 1 == text.size())
			{
				break;
			}
		}
		file_index += char_index + 1;
		if (load_information)
		{
			if (!is_id_read)
			{
				if (line.compare(0, 10, "BeatmapID:") == 0)
				{
					output_information->id = "osu" + line.substr(10);
					is_id_read = true;
				}
			}
			if (!is_title_read)
			{
				if (line.compare(0, 6, "Title:") == 0)
				{
					output_information->title = line.substr(6);
					is_title_read = true;
				}
			}
			if (!is_artist_read)
			{
				if (line.compare(0, 7, "Artist:") == 0)
				{
					output_information->artist = line.substr(7);
					is_artist_read = true;
				}
			}
			if (!is_noter_read)
			{
				if (line.compare(0, 8, "Creator:") == 0)
				{
					output_information->noter = line.substr(8);
					is_noter_read = true;
				}
			}
			if (!is_version_read)
			{
				if (line.compare(0, 8, "Version:") == 0)
				{
					output_information->version = line.substr(8);
					is_version_read = true;
				}
			}
			if (!is_mode_read)
			{
				if (line.compare(0, 6, "Mode: ") == 0)
				{
					mode = atoi(line.substr(6).c_str());
					is_mode_read = true;
				}
			}
			if (!is_key_count_read)
			{
				if (line.compare(0, 11, "CircleSize:") == 0)
				{
					key_count = atoi(line.substr(11).c_str());
					is_key_count_read = true;
				}
			}
			if (!is_audio_path_read)
			{
				if (line.compare(0, 15, "AudioFilename: ") == 0)
				{
					output_information->audio_path = line.substr(15);
					is_audio_path_read = true;
				}
			}
			if (!is_preview_time_read)
			{
				if (line.compare(0, 13, "PreviewTime: ") == 0)
				{
					output_information->preview_time = atoi(line.substr(13).c_str());
					is_preview_time_read = true;
				}
			}
			success = success && mode == 3;
			success = success && key_count == 4;
			if (!success)
			{
				return false;
			}
		}

		int comma_count = 0;
		for (int i = 0; i < line.size(); i++)
		{
			if (line[i] == ',')
			{
				comma_count++;
			}
			if (comma_count == 5)
			{
				if (line[i] == ':')	//Notes
				{
					int comma_index;
					int last_comma_index;
					int column_index;
					unsigned int time;
					unsigned int time_end;
					int type;

					comma_index = line.find(',', 0);
					column_index = atoi(line.substr(0, comma_index).c_str());
					//column index
					comma_index = line.find(',', comma_index + 1);
					last_comma_index = comma_index;
					//ignored
					comma_index = line.find(',', comma_index + 1);
					time = atoi(line.substr(last_comma_index + 1, comma_index - last_comma_index - 1).c_str());
					last_comma_index = comma_index;
					//time
					comma_index = line.find(',', comma_index + 1);
					type = atoi(line.substr(last_comma_index + 1, comma_index - last_comma_index - 1).c_str());
					//note type
					comma_index = line.find(',', comma_index + 1);
					last_comma_index = comma_index;
					//ignored
					comma_index = line.find(':', comma_index + 1);
					time_end = atoi(line.substr(last_comma_index + 1, comma_index - last_comma_index - 1).c_str());
					time_end = (type % 16 == 0) ? time_end : time;
					//time end

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
								(*output_note_set)[0]->note.push_back(new_note);
							break;
							case 192:
								(*output_note_set)[1]->note.push_back(new_note);
							break;
							case 320:
								(*output_note_set)[2]->note.push_back(new_note);
							break;
							case 448:
								(*output_note_set)[3]->note.push_back(new_note);
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

					comma_count = 0;
					break;
				}
			}
			if (comma_count == 7)	//Timelines
			{
				int comma_index;
				int last_comma_index;
				int start_time;
				float milliseconds;
				int type;

				comma_index = line.find(',', 0);
				start_time = atoi(line.substr(0, comma_index).c_str());
				last_comma_index = comma_index;
				//start time
				comma_index = line.find(',', comma_index + 1);
				milliseconds = atof(line.substr(last_comma_index + 1, comma_index - last_comma_index - 1).c_str());
				//milliseconds
				comma_index = line.find(',', comma_index + 1);
				comma_index = line.find(',', comma_index + 1);
				comma_index = line.find(',', comma_index + 1);
				comma_index = line.find(',', comma_index + 1);
				last_comma_index = comma_index;
				//ignored
				comma_index = line.find(',', comma_index + 1);
				type = atoi(line.substr(last_comma_index + 1, comma_index - last_comma_index - 1).c_str());
				if (type == 0)
				{
					if (load_note)
					{
						Timeline *new_timeline = new Timeline;
						new_timeline->start_time = start_time + 2000;
						new_timeline->bpm = last_timeline->bpm;
						new_timeline->speed = new_timeline->bpm / base_bpm / (-milliseconds / 100.f);
						new_timeline->end_time = 0;
						last_timeline->end_time = start_time + 2000;
						for (int i = 0; i < output_note_set->size(); i++)
						{
							(*output_note_set)[i]->timeline.push_back(new_timeline);
						}
						last_timeline = new_timeline;
					}
				}
				else if (type == 1)
				{
					if (load_note)
					{
						Timeline *new_timeline = new Timeline;
						new_timeline->start_time = start_time + 2000;
						new_timeline->bpm = 60000.f / milliseconds;
						if ((*output_note_set)[0]->timeline.size() == 0)
						{
							base_bpm = new_timeline->bpm;
						}
						new_timeline->speed = new_timeline->bpm / base_bpm;
						new_timeline->end_time = 0;
						if (last_timeline)
						{
							last_timeline->end_time = start_time + 2000;
						}
						for (int i = 0; i < output_note_set->size(); i++)
						{
							(*output_note_set)[i]->timeline.push_back(new_timeline);
						}
						last_timeline = new_timeline;
					}
				}

				comma_count = 0;
				break;
			}
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
		std::sort((*output_note_set)[0]->note.begin(), (*output_note_set)[0]->note.end(), CompareNote);
		std::sort((*output_note_set)[1]->note.begin(), (*output_note_set)[1]->note.end(), CompareNote);
		std::sort((*output_note_set)[2]->note.begin(), (*output_note_set)[2]->note.end(), CompareNote);
		std::sort((*output_note_set)[3]->note.begin(), (*output_note_set)[3]->note.end(), CompareNote);
		//信不過來排序
	}
	return true;
}

bool fr::LoadIMDFile(std::string path, SongInformation *output_information, std::vector<fr::NoteSet*> *output_note_set)
{
	static std::regex path_pattern(".*/(.*?)_(\\d)k_(.*?)\\.imd");
	bool success = true;
	bool load_information = output_information != NULL;
	bool load_note = output_note_set != NULL;
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
	if (load_note)
	{
		for (int i = 0; i < 4; i++)
		{
			NoteSet *new_note_set = new NoteSet;
			output_note_set->push_back(new_note_set);
		}
	}

	if (load_note)
	{
		Timeline *first_timeline = new Timeline;
		first_timeline->start_time = 0;
		first_timeline->speed = 1;
		first_timeline->end_time = 0;
		for (int i = 0; i < output_note_set->size(); i++)
		{
			(*output_note_set)[i]->timeline.push_back(first_timeline);
		}
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
					(*output_note_set)[track]->note.push_back(new_note);
				}
				break;
				case 0x0001:	//滑鍵
				{
					Note *new_start_note = new Note;
					new_start_note->type = NOTETYPE_NORMAL;
					new_start_note->type_end = NOTETYPE_NORMAL;
					new_start_note->time = time_stamp + 2000;
					new_start_note->time_end = new_start_note->time;
					(*output_note_set)[track]->note.push_back(new_start_note);
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
						(*output_note_set)[i]->note.push_back(new_slide_note);
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
					(*output_note_set)[track]->note.push_back(new_note);
				}
				break;
				case 0x0061:	//長滑條開頭(橫滑)
				{
					Note *new_start_note = new Note;
					new_start_note->type = NOTETYPE_NORMAL;
					new_start_note->type_end = NOTETYPE_NORMAL;
					new_start_note->time = time_stamp + 2000;
					new_start_note->time_end = new_start_note->time;
					(*output_note_set)[track]->note.push_back(new_start_note);
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
						(*output_note_set)[i]->note.push_back(new_slide_note);
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
					(*output_note_set)[track]->note.push_back(new_note);
				}
				break;
				case 0x0021:	//長滑條中繼(橫滑)
				{
					(*output_note_set)[track]->note[(*output_note_set)[track]->note.size() - 1]->type_end = time_span > 0 ? NOTETYPE_SLIDE_OUT_RIGHT : NOTETYPE_SLIDE_OUT_LEFT;
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
						(*output_note_set)[i]->note.push_back(new_slide_note);
					}
				}
				break;
				case 0x0022:	//長滑條中繼(直行)
					(*output_note_set)[track]->note[(*output_note_set)[track]->note.size() - 1]->time_end =	(*output_note_set)[track]->note[(*output_note_set)[track]->note.size() - 1]->time + time_span;
				break;
				case 0x00A1:	//長滑條結束(橫滑)
					(*output_note_set)[track]->note[(*output_note_set)[track]->note.size() - 1]->type_end = time_span > 0 ? NOTETYPE_SLIDE_OUT_RIGHT : NOTETYPE_SLIDE_OUT_LEFT;
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
						(*output_note_set)[i]->note.push_back(new_slide_note);
					}
				break;
				case 0x00A2:	//長滑條結束(直行)
					(*output_note_set)[track]->note[(*output_note_set)[track]->note.size() - 1]->time_end =	(*output_note_set)[track]->note[(*output_note_set)[track]->note.size() - 1]->time + time_span;
				break;
			}	//switch (action)
		}	//if (load_note)
	}

	if (load_note)
	{
		std::sort((*output_note_set)[0]->note.begin(), (*output_note_set)[0]->note.end(), CompareNote);
		std::sort((*output_note_set)[1]->note.begin(), (*output_note_set)[1]->note.end(), CompareNote);
		std::sort((*output_note_set)[2]->note.begin(), (*output_note_set)[2]->note.end(), CompareNote);
		std::sort((*output_note_set)[3]->note.begin(), (*output_note_set)[3]->note.end(), CompareNote);
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


bool fr::LoadMalodyFile(std::string path, fr::SongInformation *output_information, std::vector<fr::NoteSet*> *output_note_set)
{
	static std::regex id_pattern("\\s*\"id\": (\\d*),");
	static std::regex title_pattern("\\s*\"title\": \"(.*)\"");
	static std::regex artist_pattern("\\s*\"artist\": \"(.*)\"");
	static std::regex noter_pattern("\\s*\"creator\": \"(.*)\"");
	static std::regex version_pattern("\\s*\"version\": \"(.*)\"");
	static std::regex mode_pattern("\\s*\"mode\": (\\d)");
	static std::regex key_count_pattern("\"mode_ext\": \\{[\\S\\s]*?\"column\": (\\d)[\\S\\s]*?\\}");
	static std::regex audio_path_pattern("\\s*\"sound\": \"(.*)\"");
	static std::regex global_offset_pattern("\\s*\"offset\": (\\d*),");
	static std::regex timeline_text_pattern("\"time\"[\\S\\s]*\"note\"");
	static std::regex timeline_bpm_pattern("\"bpm\": ([\\d.]+)");
	static std::regex timeline_beat_pattern("\"beat\": \\[(\\d+),(\\d+),(\\d+)\\]");
	static std::regex note_pattern("\\s*\\{\\s*\"beat\": \\[(\\d+),(\\d+),(\\d+)\\],\\s*(?:\"endbeat\": \\[\\d+,\\d+,\\d+\\],\\s*)?\"column\": (\\d+)\\s*\\}");
	static std::regex long_note_pattern("\\s*\\{\\s*\"beat\": \\[\\d+,\\d+,\\d+\\],\\s*\"endbeat\": \\[(\\d+),(\\d+),(\\d+)\\],\\s*\"column\": \\d+\\s*\\}");

	bool success = true;

	bool load_information = output_information != NULL;
	bool load_note = output_note_set != NULL;

	int note_count = 0;
	int key_count = 0;
	int mode = 0;
	std::vector<float> note_piece_count;
	int note_piece_index = 0;
	note_piece_count.push_back(0);
	int global_offset = 0;

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

		std::regex_search(text, id_line, id_pattern);
		std::regex_search(text, title_line, title_pattern);
		std::regex_search(text, artist_line, artist_pattern);
		std::regex_search(text, noter_line, noter_pattern);
		std::regex_search(text, version_line, version_pattern);
		std::regex_search(text, mode_line, mode_pattern);
		std::regex_search(text, key_count_line, key_count_pattern);
		std::regex_search(text, audio_path_line, audio_path_pattern);

		output_information->id = "ma" +  std::regex_replace(id_line.str(), id_pattern, "$1");
		output_information->title = std::regex_replace(title_line.str(), title_pattern, "$1");
		output_information->artist = std::regex_replace(artist_line.str(), artist_pattern, "$1");
		output_information->noter = std::regex_replace(noter_line.str(), noter_pattern, "$1");
		output_information->version = std::regex_replace(version_line.str(), version_pattern, "$1");
		output_information->audio_path = GetParentDir(output_information->file_path) + std::regex_replace(audio_path_line.str(), audio_path_pattern, "$1");
		output_information->full_score = 0;
		success = success && (atoi(std::regex_replace(mode_line.str(), mode_pattern, "$1").c_str()) == 0);
		success = success && (atoi(std::regex_replace(key_count_line.str(), key_count_pattern, "$1").c_str()) == 4);

		if (!success)
		{
			return false;
		}
	}
	if (load_note)
	{
		std::smatch global_offset_line;
		std::regex_search(text, global_offset_line, global_offset_pattern);
		global_offset = atoi(std::regex_replace(global_offset_line.str(), global_offset_pattern, "$1").c_str());
		for (int i = 0; i < 4; i++)
		{
			NoteSet *new_note_set = new NoteSet;
			output_note_set->push_back(new_note_set);
		}
	}

	std::smatch timeline_text;
	std::string timeline_text_str;
	std::regex_search(text, timeline_text, timeline_text_pattern);
	timeline_text_str = timeline_text.str();

	std::vector<float> timeline_meter;
	std::vector<int> temp_timeline_bar;
	std::vector<int> temp_timeline_beat;
	std::vector<int> temp_timeline_divide;
	std::vector<float> temp_timeline_bpm;
	std::vector<Timeline*> temp_timeline;
	int base_bpm = 0;
	float last_milliseconds = 0;
	int last_timeline_bar = 0;
	int last_timeline_beat = 0;
	int last_timeline_divide = 1;
	Timeline *last_timeline = NULL;
	for (std::sregex_iterator i = std::sregex_iterator(timeline_text_str.begin(), timeline_text_str.end(), timeline_beat_pattern); i != std::sregex_iterator(); i++)
	{
		std::smatch beat_line = *i;
		temp_timeline_bar.push_back(atoi(std::regex_replace(beat_line.str(), timeline_beat_pattern, "$1").c_str()));
		temp_timeline_beat.push_back(atoi(std::regex_replace(beat_line.str(), timeline_beat_pattern, "$2").c_str()));
		temp_timeline_divide.push_back(atoi(std::regex_replace(beat_line.str(), timeline_beat_pattern, "$3").c_str()));
	}
	for (std::sregex_iterator i = std::sregex_iterator(timeline_text_str.begin(), timeline_text_str.end(), timeline_bpm_pattern); i != std::sregex_iterator(); i++)
	{
		std::smatch bpm_line = *i;
		temp_timeline_bpm.push_back(atof(std::regex_replace(bpm_line.str(), timeline_bpm_pattern, "$1").c_str()));
	}
	for (int i = 0; i < temp_timeline_bpm.size(); i++)
	{
		int timeline_bar = temp_timeline_bar[i];
		int timeline_beat = temp_timeline_beat[i];
		int timeline_divide = temp_timeline_divide[i];
		float bpm = temp_timeline_bpm[i];
		float milliseconds = 60000.f / bpm;
		float meter = float(timeline_bar * timeline_divide + timeline_beat) / float(timeline_divide);
		timeline_meter.push_back(meter);
		int start_time = 0;
		if (last_timeline)
		{
			float last_meter = float(last_timeline_bar * last_timeline_divide + last_timeline_beat) / float(last_timeline_divide);
			float sub = meter - last_meter;
			last_timeline->end_time = last_timeline->start_time + last_milliseconds * sub;
			start_time = last_timeline->end_time;
		}
		else
		{
			start_time = 2000 - global_offset;
		}

		Timeline *new_timeline = new Timeline;
		new_timeline->start_time = start_time;
		new_timeline->bpm = bpm;
		if (temp_timeline.size() == 0)
		{
			base_bpm = new_timeline->bpm;
		}
		new_timeline->speed = new_timeline->bpm / base_bpm;
		new_timeline->end_time = 0;
		temp_timeline.push_back(new_timeline);
		last_timeline = new_timeline;
		last_milliseconds = milliseconds;
		last_timeline_bar = timeline_bar;
		last_timeline_beat = timeline_beat;
		last_timeline_divide = timeline_divide;
	}
	if (load_note)
	{
		for (int i = 0; i < 4; i++)
		{
			(*output_note_set)[i]->timeline = temp_timeline;
		}
	}

	int timeline_index = 0;
	for (std::sregex_iterator i = std::sregex_iterator(text.begin(), text.end(), note_pattern); i != std::sregex_iterator(); i++)
	{
		std::smatch note_line = *i;
		int note_bar = atoi(std::regex_replace(note_line.str(), note_pattern, "$1").c_str());
		int note_beat = atoi(std::regex_replace(note_line.str(), note_pattern, "$2").c_str());
		int note_divide = atoi(std::regex_replace(note_line.str(), note_pattern, "$3").c_str());
		float meter = float(note_bar * note_divide + note_beat) / float(note_divide);
		if (timeline_index < temp_timeline.size() - 1)
		{
			while (meter >= timeline_meter[timeline_index + 1])
			{
				timeline_index++;
				if (timeline_index > temp_timeline.size() - 2)
				{
					break;
				}
			}
		}
		float milliseconds = 60000.f / temp_timeline[timeline_index]->bpm;
		float sub_meter = meter - timeline_meter[timeline_index];
		int time = temp_timeline[timeline_index]->start_time + milliseconds * sub_meter;
		int type = 0;
		int time_end = time;
		if (std::regex_match(note_line.str(), long_note_pattern))
		{
			type = 1;
			int note_end_bar = atoi(std::regex_replace(note_line.str(), long_note_pattern, "$1").c_str());
			int note_end_beat = atoi(std::regex_replace(note_line.str(), long_note_pattern, "$2").c_str());
			int note_end_divide = atoi(std::regex_replace(note_line.str(), long_note_pattern, "$3").c_str());
			int timeline_end_index = timeline_index;
			float meter_end = float(note_end_bar * note_end_divide + note_end_beat) / float(note_end_divide);
			if (timeline_end_index < temp_timeline.size() - 1)
			{
				while (meter_end >= timeline_meter[timeline_end_index + 1])
				{
					timeline_end_index++;
					if (timeline_end_index >= temp_timeline.size() - 2)
					{
						break;
					}
				}
			}
			float milliseconds_end = 60000.f / temp_timeline[timeline_end_index]->bpm;
			float sub_meter_end = meter_end - timeline_meter[timeline_end_index];
			time_end = temp_timeline[timeline_end_index]->start_time + milliseconds_end * sub_meter_end;
		}
		int column_index = atoi(std::regex_replace(note_line.str(), note_pattern, "$4").c_str());
		if (load_note)
		{
			Note *new_note = new Note;
			new_note->time = time;
			new_note->time_end = time_end;
			new_note->type = NOTETYPE_NORMAL;
			new_note->type_end = NOTETYPE_NORMAL;
			(*output_note_set)[column_index]->note.push_back(new_note);
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
			output_information->full_score += (type == 1) ? 4 : 2;
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
		if (!load_note)
		{
			temp_timeline.clear();
		}
	}
	if (load_note)
	{
		std::sort((*output_note_set)[0]->note.begin(), (*output_note_set)[0]->note.end(), CompareNote);
		std::sort((*output_note_set)[1]->note.begin(), (*output_note_set)[1]->note.end(), CompareNote);
		std::sort((*output_note_set)[2]->note.begin(), (*output_note_set)[2]->note.end(), CompareNote);
		std::sort((*output_note_set)[3]->note.begin(), (*output_note_set)[3]->note.end(), CompareNote);
	}
	return true;
}