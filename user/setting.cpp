#include "setting.h"
#include <string>
#include <boost/regex.hpp>
#include "../file_system.h"
#include "../system.h"
#include "../timer.h"

fr::Setting *fr::Setting::m_instance = 0;

void fr::Setting::init()
{
	user_profile_path = "/sdcard/data/default.fru";
	is_auto = false;
	is_slide_out = false;
	duration = 800;
	offset = -200;
	key_code[0] = SDL_SCANCODE_D;
	key_code[1] = SDL_SCANCODE_F;
	key_code[2] = SDL_SCANCODE_J;
	key_code[3] = SDL_SCANCODE_K;
	draw_scale_portrait = 1000;
	draw_offset_portrait = 0;
	draw_scale_landscape = 1450;
	draw_offset_landscape = -320;
	song_list.push_back("assets/songs");
	tips_text.push_back("适当休息。在这里我更建议您的是对手部进行放松");
	tips_text.push_back("在游戏时，可以在屏幕上半部分以竖直方向滑动手指，来进行快速调节音符下落速度");
	tips_text.push_back("在游戏时若发现音画不同步的现象，可以尝试调节一下偏移，本游戏内置偏移调节向导");
	tips_text.push_back("如果您是音乐游戏新手，那么请最好不要一下子尝试高难度谱面，也最好不要尝试无法接受的音符下落速度");
	tips_text.push_back("Performance数值不同于传统游戏的经验值，这个数值能大致反映您在这个游戏的实力");
	tips_text.push_back("要想提高Performance数值，您需要选择您能接受的适中难度的谱面(不能过于简单)，并打出较好的成绩");

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
	tips_text.clear();
	boost::regex auto_pattern("auto:(.+?)\\n");
	boost::regex slide_out_pattern("slide_out:(.+?)\\n");
	boost::regex duration_pattern("duration:(\\d+?)\\n");
	boost::regex offset_pattern("offset:(-?\\d+?)\\n");
	boost::regex key_code_pattern("key (\\d+?):(\\d+?)\\n");
	boost::regex song_list_pattern("song_list:(.+?)\\n");
	boost::regex column_portrait_pattern("column_portrait:(\\d+?),(-?\\d+?)\\n");
	boost::regex column_landscape_pattern("column_landscape:(\\d+?),(-?\\d+?)\\n");
	boost::regex tips_pattern("tips:(.+?)\\n");
	boost::smatch auto_line;
	boost::smatch slide_out_line;
	boost::smatch duration_line;
	boost::smatch offset_line;
	boost::smatch column_portrait_line;
	boost::smatch column_landscape_line;
	boost::regex_search(file, auto_line, auto_pattern);
	boost::regex_search(file, slide_out_line, slide_out_pattern);
	boost::regex_search(file, duration_line, duration_pattern);
	boost::regex_search(file, offset_line, offset_pattern);
	boost::regex_search(file, column_portrait_line, column_portrait_pattern);
	boost::regex_search(file, column_landscape_line, column_landscape_pattern);
	is_auto = boost::regex_replace(auto_line.str(), auto_pattern, "$1") == "on" ? true : false;
	is_slide_out = boost::regex_replace(slide_out_line.str(), slide_out_pattern, "$1") == "on" ? true : false;
	duration = atoi(boost::regex_replace(duration_line.str(), duration_pattern, "$1").c_str());
	offset = atoi(boost::regex_replace(offset_line.str(), offset_pattern, "$1").c_str());
	draw_scale_portrait = atoi(boost::regex_replace(column_portrait_line.str(), column_portrait_pattern, "$1").c_str());
	draw_offset_portrait = atoi(boost::regex_replace(column_portrait_line.str(), column_portrait_pattern, "$2").c_str());
	draw_scale_landscape = atoi(boost::regex_replace(column_landscape_line.str(), column_landscape_pattern, "$1").c_str());
	draw_offset_landscape = atoi(boost::regex_replace(column_landscape_line.str(), column_landscape_pattern, "$2").c_str());
	for (boost::sregex_iterator i = boost::sregex_iterator(file.begin(), file.end(), key_code_pattern); i != boost::sregex_iterator(); i++)
	{
		boost::smatch key_code_line = *i;
		int column_index = atoi(boost::regex_replace(key_code_line.str(), key_code_pattern, "$1").c_str());
		key_code[column_index] = SDL_Scancode( atoi(boost::regex_replace(key_code_line.str(), key_code_pattern, "$2").c_str()));
	}
	for (boost::sregex_iterator i = boost::sregex_iterator(file.begin(), file.end(), song_list_pattern); i != boost::sregex_iterator(); i++)
	{
		boost::smatch song_list_line = *i;
		song_list.push_back(boost::regex_replace(song_list_line.str(), song_list_pattern, "$1"));
	}
	for (boost::sregex_iterator i = boost::sregex_iterator(file.begin(), file.end(), tips_pattern); i != boost::sregex_iterator(); i++)
	{
		boost::smatch tips_line = *i;
		tips_text.push_back(boost::regex_replace(tips_line.str(), tips_pattern, "$1"));
	}
	return true;
}

void fr::Setting::write()
{
	std::string file;
	char *duration_ch = new char[50];
	char *offset_ch = new char[50];
	char *column_portrait_ch = new char[50];
	char *column_landscape_ch = new char[50];
	sprintf(duration_ch, "duration:%d\n", duration);
	sprintf(offset_ch, "offset:%d\n", offset);
	sprintf(column_portrait_ch, "column_portrait:%d,%d\n", draw_scale_portrait, draw_offset_portrait);
	sprintf(column_landscape_ch, "column_landscape:%d,%d\n", draw_scale_landscape, draw_offset_landscape);

	file = "Force Rush user setting file\n";
	file += is_auto ? "auto:on\n" : "auto:off\n";
	file += is_slide_out ? "slide_out:on\n" : "slide_out:off\n";
	file += duration_ch;
	file += offset_ch;
	file += column_portrait_ch;
	file += column_landscape_ch;
	delete [] duration_ch;
	delete [] offset_ch;
	delete [] column_portrait_ch;
	delete [] column_landscape_ch;
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
	for (int i = 0; i < tips_text.size(); i++)
	{
		file += "tips:";
		file += tips_text[i];
		file += "\n";
	}
	file += "End of file\n";
	WriteFile("/sdcard/data/user_setting.fa", file);
}

std::string fr::Setting::GetUserProfilePath()
{
	return user_profile_path;
}

bool fr::Setting::IsAuto()
{
	return is_auto;
}

bool fr::Setting::IsSlideOut()
{
	return is_slide_out;
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

std::string fr::Setting::GetRandomTips()
{
	srand(Timer::instance()->GetSystemTime());
	int index = rand() / tips_text.size();
	return tips_text[index];
}

void fr::Setting::SetUserProfilePath(std::string input)
{
	user_profile_path = input;
}

void fr::Setting::SwitchAuto()
{
	is_auto = !is_auto;
	write();
}

void fr::Setting::SwitchSlideOut()
{
	is_slide_out = !is_slide_out;
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