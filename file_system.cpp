#include "file_system.h"
#include <dirent.h>
#include <fstream>
#include <regex>

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