#include "setting.h"
#include <string>
#include <regex>
#include "../file_system.h"

lm::Setting *lm::Setting::m_instance = 0;

void lm::Setting::init()
{
	is_auto = false;
	duration = 800;
	offset = -200;
	key_code[0] = SDL_SCANCODE_D;
	key_code[1] = SDL_SCANCODE_F;
	key_code[2] = SDL_SCANCODE_J;
	key_code[3] = SDL_SCANCODE_K;
	if (!read())
	{
		write();
	}
}

bool lm::Setting::read()
{
	std::string file;
	if (!ReadFile("/sdcard/data/user_setting.fa", file))
	{
		return false; 
	}
	std::regex auto_pattern("auto: (.+)");
	std::regex duration_pattern("duration: (\\d+)");
	std::regex offset_pattern("offset: (-?\\d+)");
	std::regex key_code_pattern("key (\\d+): (\\d+)");
	std::smatch auto_line;
	std::smatch duration_line;
	std::smatch offset_line;
	std::regex_search(file, auto_line, auto_pattern);
	std::regex_search(file, duration_line, duration_pattern);
	std::regex_search(file, offset_line, offset_pattern);
	is_auto = std::regex_replace(auto_line.str(), auto_pattern, "$1") == "on" ? true : false;
	duration = atoi(std::regex_replace(duration_line.str(), duration_pattern, "$1").c_str());
	offset = atoi(std::regex_replace(offset_line.str(), offset_pattern, "$1").c_str());
	for (std::sregex_iterator i = std::sregex_iterator(file.begin(), file.end(), key_code_pattern); i != std::sregex_iterator(); i++)
	{
		std::smatch key_code_line = *i;
		int column_index = atoi(std::regex_replace(key_code_line.str(), key_code_pattern, "$1").c_str());
		key_code[column_index] = SDL_Scancode( atoi(std::regex_replace(key_code_line.str(), key_code_pattern, "$2").c_str()));
	}
	return true;
}

void lm::Setting::write()
{
	std::string file;
	char *duration_ch = new char[20];
	char *offset_ch = new char[20];
	sprintf(duration_ch, "duration: %d\n", duration);
	sprintf(offset_ch, "offset: %d\n", offset);

	file = "Lunatic Melody user setting file\n";
	file += is_auto ? "auto: on\n" : "auto: off\n";
	file += duration_ch;
	file += offset_ch;
	delete [] duration_ch;
	for (int i = 0; i < 4; i++)
	{
		char *key_code_ch = new char[20];
		sprintf(key_code_ch, "Key %d: %d\n", i, key_code[i]);
		file += key_code_ch;
		delete [] key_code_ch;
	}
	WriteFile("/sdcard/data/user_setting.fa", file);
}

bool lm::Setting::IsAuto()
{
	return is_auto;
}

int lm::Setting::GetDuration()
{
	return duration;
}

int lm::Setting::GetOffset()
{
	return offset;
}

SDL_Scancode lm::Setting::GetKeycode(int index)
{
	return key_code[index];
}