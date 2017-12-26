#include "setting.h"
#include <string>
#include <regex>
#include "../file_system.h"
#include "../system.h"

fr::Setting *fr::Setting::m_instance = 0;

void fr::Setting::init()
{
	is_auto = false;
	duration = 800;
	offset = -200;
	key_code[0] = SDL_SCANCODE_D;
	key_code[1] = SDL_SCANCODE_F;
	key_code[2] = SDL_SCANCODE_J;
	key_code[3] = SDL_SCANCODE_K;
	draw_scale_portrait = 1000;
	draw_offset_portrait = 0;
	draw_scale_landscape = 1500;
	draw_offset_landscape = -350;
	song_list.push_back("/sdcard/data/malody/beatmap");
	if (!read())
	{
		write();
	}
}

bool fr::Setting::read()
{
	std::string file;
	if (!ReadFile("/sdcard/data/user_setting.fa", file))
	{
		return false; 
	}
	song_list.clear();
	std::regex auto_pattern("auto:(.+)");
	std::regex duration_pattern("duration:(\\d+)");
	std::regex offset_pattern("offset:(-?\\d+)");
	std::regex key_code_pattern("key (\\d+):(\\d+)");
	std::regex song_list_pattern("song_list:(.+)");
	std::regex column_portrait_pattern("column_portrait:(\\d+),(-?\\d+)");
	std::regex column_landscape_pattern("column_landscape:(\\d+),(-?\\d+)");
	std::smatch auto_line;
	std::smatch duration_line;
	std::smatch offset_line;
	std::smatch column_portrait_line;
	std::smatch column_landscape_line;
	std::regex_search(file, auto_line, auto_pattern);
	std::regex_search(file, duration_line, duration_pattern);
	std::regex_search(file, offset_line, offset_pattern);
	std::regex_search(file, column_portrait_line, column_portrait_pattern);
	std::regex_search(file, column_landscape_line, column_landscape_pattern);
	is_auto = std::regex_replace(auto_line.str(), auto_pattern, "$1") == "on" ? true : false;
	duration = atoi(std::regex_replace(duration_line.str(), duration_pattern, "$1").c_str());
	offset = atoi(std::regex_replace(offset_line.str(), offset_pattern, "$1").c_str());
	draw_scale_portrait = atoi(std::regex_replace(column_portrait_line.str(), column_portrait_pattern, "$1").c_str());
	draw_offset_portrait = atoi(std::regex_replace(column_portrait_line.str(), column_portrait_pattern, "$2").c_str());
	draw_scale_landscape = atoi(std::regex_replace(column_landscape_line.str(), column_landscape_pattern, "$1").c_str());
	draw_offset_landscape = atoi(std::regex_replace(column_landscape_line.str(), column_landscape_pattern, "$2").c_str());
	for (std::sregex_iterator i = std::sregex_iterator(file.begin(), file.end(), key_code_pattern); i != std::sregex_iterator(); i++)
	{
		std::smatch key_code_line = *i;
		int column_index = atoi(std::regex_replace(key_code_line.str(), key_code_pattern, "$1").c_str());
		key_code[column_index] = SDL_Scancode( atoi(std::regex_replace(key_code_line.str(), key_code_pattern, "$2").c_str()));
	}
	for (std::sregex_iterator i = std::sregex_iterator(file.begin(), file.end(), song_list_pattern); i != std::sregex_iterator(); i++)
	{
		std::smatch song_list_line = *i;
		song_list.push_back(std::regex_replace(song_list_line.str(), song_list_pattern, "$1"));
	}
	return true;
}

void fr::Setting::write()
{
	std::string file;
	char *duration_ch = new char;
	char *offset_ch = new char;
	char *column_portrait_ch = new char;
	char *column_landscape_ch = new char;
	sprintf(duration_ch, "duration:%d\n", duration);
	sprintf(offset_ch, "offset:%d\n", offset);
	sprintf(column_portrait_ch, "column_portrait:%d,%d\n", draw_scale_portrait, draw_offset_portrait);
	sprintf(column_landscape_ch, "column_landscape:%d,%d\n", draw_scale_landscape, draw_offset_landscape);

	file = "Force Rush user setting file\n";
	file += is_auto ? "auto:on\n" : "auto:off\n";
	file += duration_ch;
	file += offset_ch;
	file += column_portrait_ch;
	file += column_landscape_ch;
	delete duration_ch;
	delete offset_ch;
	delete column_portrait_ch;
	delete column_landscape_ch;
	for (int i = 0; i < 4; i++)
	{
		char *key_code_ch = new char;
		sprintf(key_code_ch, "Key %d:%d\n", i, key_code[i]);
		file += key_code_ch;
		delete key_code_ch;
	}
	for (int i = 0; i < song_list.size(); i++)
	{
		file += "song_list:";
		file += song_list[i];
		file += "\n";
	}
	file += "End of file\n";
	WriteFile("/sdcard/data/user_setting.fa", file);
}

bool fr::Setting::IsAuto()
{
	return is_auto;
}

int fr::Setting::GetDuration()
{
	return duration;
}

int fr::Setting::GetOffset()
{
	return offset;
}

float fr::Setting::GetDrawScale()
{
	if (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT)
	{
		return float(draw_scale_portrait) / 1000.f;
	}
	return float(draw_scale_landscape) / 1000.f;
}

int fr::Setting::GetDrawOffset()
{
	if (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT)
	{
		return float(draw_offset_portrait) / 1000.f * System::instance()->GetWindowHeigh();
	}
	return float(draw_offset_landscape) / 1000.f * System::instance()->GetWindowHeigh();
}

SDL_Scancode fr::Setting::GetKeycode(int index)
{
	return key_code[index];
}

void fr::Setting::GetSongList(std::vector<std::string> &output)
{
	output = song_list;
}

void fr::Setting::SwitchAuto()
{
	is_auto = !is_auto;
	write();
}

void fr::Setting::SetDuration(int input)
{
	duration = input;
	write();
}

void fr::Setting::SetOffset(int input)
{
	offset = input;
	write();
}

void fr::Setting::SetDrawScale(float input)
{
	if (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT)
	{
		draw_scale_portrait = input * 1000;
	}
	else
	{
		draw_scale_landscape = input * 1000;
	}
	write();
}

void fr::Setting::SetDrawOffset(int input)
{
	if (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT)
	{
		draw_offset_portrait = input * 1000.f / System::instance()->GetWindowHeigh();
	}
	else
	{
		draw_offset_landscape = input * 1000.f / System::instance()->GetWindowHeigh();
	}
	write();
}

void fr::Setting::SetKeycode(SDL_Scancode input, int index)
{
	key_code[index] = input;
	write();
}