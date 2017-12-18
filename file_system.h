#ifndef FORCE_RUSH_FILE_SYSTEM_H
#define FORCE_RUSH_FILE_SYSTEM_H

#include <string>
#include <vector>

namespace fr
{
	enum FileType
	{
		FILETYPE_FILE = 0,
		FILETYPE_DIR,
		FILETYPE_LINK
	};

	struct File
	{
		std::string name;
		FileType type;
	};

	bool ReadFile(std::string path, std::string &output);
	bool WriteFile(std::string path, std::string &output);
	
	bool ListDir(std::string path, std::vector<File*> &output);
	void FindFile(std::string path, std::string pattern_str, std::vector<File*> &output);
	std::string GetParentDir(std::string path);
};

#endif