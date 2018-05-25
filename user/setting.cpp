#include "setting.h"
#include <string>
#include <regex>
#include "../file_system.h"
#include "../system.h"
#include "../timer.h"

fr::Setting *fr::Setting::m_instance = 0;

void fr::Setting::init()
{
	user_profile_path = "/sdcard/ForceRush/default.fa";
	is_auto = false;
	is_slide_out = false;
	speed = 3000;
	offset = -200;
	key_code[0] = SDL_SCANCODE_D;
	key_code[1] = SDL_SCANCODE_F;
	key_code[2] = SDL_SCANCODE_J;
	key_code[3] = SDL_SCANCODE_K;
	camera_pos_y_portrait = -160;
	camera_pos_z_portrait = 1020;
	camera_rotate_x_portrait = 18;
	force_angle_portrait = 5;
	camera_pos_y_landscape = -470;
	camera_pos_z_landscape = 120;
	camera_rotate_x_landscape = 35;
	force_angle_landscape = 3;
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
	if (!ReadFile("/sdcard/ForceRush/setting.fa", file))
	{
		return false; 
	}
	song_list.clear();
	tips_text.clear();
	std::regex auto_pattern("auto:(.+?)\\n");
	std::regex slide_out_pattern("slide_out:(.+?)\\n");
	std::regex speed_pattern("speed:(\\d+?)\\n");
	std::regex offset_pattern("offset:(-?\\d+?)\\n");
	std::regex key_code_pattern("key (\\d+?):(\\d+?)\\n");
	std::regex song_list_pattern("song_list:(.+?)\\n");
	std::regex camera_portrait_pattern("camera_portrait:(-?\\d+?),(-?\\d+?),(-?\\d+?),(-?\\d+?)\\n");
	std::regex camera_landscape_pattern("camera_landscape:(-?\\d+?),(-?\\d+?),(-?\\d+?),(-?\\d+?)\\n");
	std::regex tips_pattern("tips:(.+?)\\n");
	std::smatch auto_line;
	std::smatch slide_out_line;
	std::smatch speed_line;
	std::smatch offset_line;
	std::smatch camera_portrait_line;
	std::smatch camera_landscape_line;
	std::regex_search(file, auto_line, auto_pattern);
	std::regex_search(file, slide_out_line, slide_out_pattern);
	std::regex_search(file, speed_line, speed_pattern);
	std::regex_search(file, offset_line, offset_pattern);
	std::regex_search(file, camera_portrait_line, camera_portrait_pattern);
	std::regex_search(file, camera_landscape_line, camera_landscape_pattern);
	is_auto = std::regex_replace(auto_line.str(), auto_pattern, "$1") == "on" ? true : false;
	is_slide_out = std::regex_replace(slide_out_line.str(), slide_out_pattern, "$1") == "on" ? true : false;
	speed = atoi(std::regex_replace(speed_line.str(), speed_pattern, "$1").c_str());
	offset = atoi(std::regex_replace(offset_line.str(), offset_pattern, "$1").c_str());
	camera_pos_y_portrait = atoi(std::regex_replace(camera_portrait_line.str(), camera_portrait_pattern, "$1").c_str());
	camera_pos_z_portrait = atoi(std::regex_replace(camera_portrait_line.str(), camera_portrait_pattern, "$2").c_str());
	camera_rotate_x_portrait = atoi(std::regex_replace(camera_portrait_line.str(), camera_portrait_pattern, "$3").c_str());
	force_angle_portrait = atoi(std::regex_replace(camera_portrait_line.str(), camera_portrait_pattern, "$4").c_str());
	camera_pos_y_landscape = atoi(std::regex_replace(camera_landscape_line.str(), camera_landscape_pattern, "$1").c_str());
	camera_pos_z_landscape = atoi(std::regex_replace(camera_landscape_line.str(), camera_landscape_pattern, "$2").c_str());
	camera_rotate_x_landscape = atoi(std::regex_replace(camera_landscape_line.str(), camera_landscape_pattern, "$3").c_str());
	force_angle_landscape = atoi(std::regex_replace(camera_landscape_line.str(), camera_landscape_pattern, "$4").c_str());
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
	for (std::sregex_iterator i = std::sregex_iterator(file.begin(), file.end(), tips_pattern); i != std::sregex_iterator(); i++)
	{
		std::smatch tips_line = *i;
		tips_text.push_back(std::regex_replace(tips_line.str(), tips_pattern, "$1"));
	}
	return true;
}

void fr::Setting::write()
{
	std::string file;
	char *speed_ch = new char[50];
	char *offset_ch = new char[50];
	char *camera_portrait_ch = new char[50];
	char *camera_landscape_ch = new char[50];
	sprintf(speed_ch, "speed:%d\n", speed);
	sprintf(offset_ch, "offset:%d\n", offset);
	sprintf(camera_portrait_ch, "camera_portrait:%d,%d,%d,%d\n", camera_pos_y_portrait, camera_pos_z_portrait, camera_rotate_x_portrait, force_angle_portrait);
	sprintf(camera_landscape_ch, "camera_landscape:%d,%d,%d,%d\n", camera_pos_y_landscape, camera_pos_z_landscape, camera_rotate_x_landscape, force_angle_landscape);

	file = "Force Rush user setting file\n";
	file += is_auto ? "auto:on\n" : "auto:off\n";
	file += is_slide_out ? "slide_out:on\n" : "slide_out:off\n";
	file += speed_ch;
	file += offset_ch;
	file += camera_portrait_ch;
	file += camera_landscape_ch;
	delete [] speed_ch;
	delete [] offset_ch;
	delete [] camera_portrait_ch;
	delete [] camera_landscape_ch;
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
	WriteFile("/sdcard/ForceRush/setting.fa", file);
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

int fr::Setting::GetSpeed()
{
	return speed;
}

int fr::Setting::GetOffset()
{
	return offset;
}

int fr::Setting::GetCameraPosY()
{
	if (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT)
	{
		return camera_pos_y_portrait;
	}
	return camera_pos_y_landscape;
}

int fr::Setting::GetCameraPosZ()
{
	if (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT)
	{
		return camera_pos_z_portrait;
	}
	return camera_pos_z_landscape;
}

int fr::Setting::GetCameraRotateX()
{
	if (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT)
	{
		return camera_rotate_x_portrait;
	}
	return camera_rotate_x_landscape;
}

int fr::Setting::GetForceAngle()
{
	if (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT)
	{
		return force_angle_portrait;
	}
	return force_angle_landscape;
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
	int index = rand() % tips_text.size();
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

void fr::Setting::SetSpeed(int input)
{
	speed = input;
	speed = speed < 1 ? 1 : speed;
	speed = speed > 100 ? 100 : speed;
	write();
}

void fr::Setting::SetOffset(int input)
{
	offset = input;
	write();
}

void fr::Setting::SetCameraPosY(int input)
{
	if (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT)
	{
		camera_pos_y_portrait = input;
	}
	else
	{
		camera_pos_y_landscape = input;
	}
	write();
}

void fr::Setting::SetCameraPosZ(int input)
{
	if (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT)
	{
		camera_pos_z_portrait = input;
	}
	else
	{
		camera_pos_z_landscape = input;
	}
	write();
}

void fr::Setting::SetCameraRotateX(int input)
{
	if (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT)
	{
		camera_rotate_x_portrait = input;
	}
	else
	{
		camera_rotate_x_landscape = input;
	}
	write();
}

void fr::Setting::SetForceAngle(int input)
{
	if (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT)
	{
		force_angle_portrait = input;
	}
	else
	{
		force_angle_landscape = input;
	}
	write();
}

void fr::Setting::SetKeycode(SDL_Scancode input, int index)
{
	key_code[index] = input;
	write();
}

void fr::Setting::AddSongList(std::string path)
{
	song_list.push_back(path);
	write();
}

void fr::Setting::DeleteSongList(int index)
{
	song_list.erase(song_list.begin() + index);
	write();
}