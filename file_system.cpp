#include "file_system.h"
#include <dirent.h>
#include <fstream>
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

bool fr::LoadOSUFile(std::string path, fr::SongInformation *output_information, std::vector<Note*> *output_note_list1, std::vector<Note*> *output_note_list2, std::vector<Note*> *output_note_list3, std::vector<Note*> *output_note_list4)
{

	static std::regex id_pattern("BeatmapID:(.*)");
	static std::regex title_pattern("Title:(.*)");
	static std::regex artist_pattern("Artist:(.*)");
	static std::regex noter_pattern("Creator:(.*)");
	static std::regex version_pattern("Version:(.*)");
	static std::regex mode_pattern("Mode: (\\d)");
	static std::regex key_count_pattern("CircleSize:(\\d)");
	static std::regex audio_path_pattern("AudioFilename: (.*)");
	static std::regex preview_time_pattern("PreviewTime: (\\d*)");
	static std::regex note_pattern("(\\d+),\\d+,(\\d+),(\\d+),\\d+,(\\d+):\\d+:\\d+:\\d+:(\\d+:)?");

	bool success = true;

	bool load_information = output_information != NULL;
	bool load_note = output_note_list1 != NULL && output_note_list2 != NULL && output_note_list3 != NULL && output_note_list4 != NULL;

	int note_count = 0;
	int key_count = 0;
	int mode = 0;

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

		output_information->id = std::regex_replace(id_line.str(), id_pattern, "$1");
		output_information->title = std::regex_replace(title_line.str(), title_pattern, "$1");
		output_information->artist = std::regex_replace(artist_line.str(), artist_pattern, "$1");
		output_information->noter = std::regex_replace(noter_line.str(), noter_pattern, "$1");
		output_information->version = std::regex_replace(version_line.str(), version_pattern, "$1");
		output_information->audio_path = GetParentDir(output_information->file_path) + std::regex_replace(audio_path_line.str(), audio_path_pattern, "$1");
		output_information->preview_time = atoi(std::regex_replace(preview_time_line.str(), preview_time_pattern, "$1").c_str());
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
		if (load_note)
		{
			Note *new_note = new Note;
			new_note->time = atoi(std::regex_replace(note_line.str(), note_pattern, "$2").c_str()) + 2000;
			int load_type = atoi(std::regex_replace(note_line.str(), note_pattern, "$3").c_str()) ;
			new_note->time_end = (load_type % 16 == 0) ? atoi(std::regex_replace(note_line.str(), note_pattern, "$4").c_str()) + 2000 : new_note->time;
			new_note->type = NOTETYPE_NORMAL;
			new_note->type_end = NOTETYPE_NORMAL;
			int column_index = atoi(std::regex_replace(note_line.str(), note_pattern, "$1").c_str());
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
			output_information->duration = atoi(std::regex_replace(note_line.str(), note_pattern, "$2").c_str());
			note_count++;
		}
	}
	if (load_information)
	{
		output_information->difficulty = note_count * 1000 / output_information->duration;
		Score *new_null_score = new Score;
		new_null_score->rank = RANK_NONE;
		new_null_score->score = 0;
		new_null_score->pure = 0;
		new_null_score->great = 0;
		new_null_score->good = 0;
		new_null_score->error = 0;
		output_information->high_score = new_null_score;
	}
	return true;
}