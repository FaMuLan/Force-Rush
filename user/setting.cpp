#include "setting.h"
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include "../file_system.h"
#include "../system.h"
#include "../timer.h"

fr::Setting *fr::Setting::m_instance = 0;

void fr::Setting::init()
{
	user_profile_path = "/sdcard/ForceRush/default.fa";
	is_auto = false;
	is_slide_out = false;
	speed = 10;
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
	mkdir("/sdcard/ForceRush/songs", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	song_list.push_back("assets/songs");
	song_list.push_back("/sdcard/ForceRush/songs");
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
	int file_index = 0;
	if (!ReadFile("/sdcard/ForceRush/setting.fa", file))
	{
		return false; 
	}
	song_list.clear();
	tips_text.clear();

	bool is_auto_read = false;
	bool is_slide_out_read = false;
	bool is_speed_read = false;
	bool is_offset_read = false;
	bool is_camera_pos_portrait_read = false;
	bool is_camera_pos_landscape_read = false;
	bool is_keycode_0_read = false;
	bool is_keycode_1_read = false;
	bool is_keycode_2_read = false;
	bool is_keycode_3_read = false;

	while (file_index < file.size())
	{
		std::string line;
		int char_index;
		for (char_index = 0; file[file_index + char_index] != 10; char_index++)
		{
			if (file[file_index + char_index] != 13)
			{
				line += file[file_index + char_index];
			}
			if (file_index + char_index + 1 == file.size())
			{
				break;
			}
		}
		file_index += char_index + 1;
		
		if (!is_auto_read)
		{
			if (line.compare(0, 5, "auto:") == 0)
			{
				is_auto = line.substr(5) == "on" ? true : false;
				is_auto_read = true;
			}
		}
		if (!is_slide_out_read)
		{
			if (line.compare(0, 10, "slide_out:") == 0)
			{
				is_slide_out = line.substr(10) == "on" ? true : false;
				is_slide_out_read = true;
			}
		}
		if (!is_speed_read)
		{
			if (line.compare(0, 6, "speed:") == 0)
			{
				speed = atoi(line.substr(6).c_str());
				is_speed_read = true;
			}
		}
		if (!is_offset_read)
		{
			if (line.compare(0, 7, "offset:") == 0)
			{
				offset = atoi(line.substr(7).c_str());
				is_offset_read = true;
			}
		}
		if (!is_camera_pos_portrait_read)
		{
			if (line.compare(0, 16, "camera_portrait:") == 0)
			{
				int mark_index = line.find(',');
				int last_mark_index = mark_index;
				camera_pos_y_portrait = atoi(line.substr(16, mark_index - 16).c_str());
				mark_index = line.find(',', mark_index + 1);
				camera_pos_z_portrait = atoi(line.substr(last_mark_index + 1, mark_index - last_mark_index - 1).c_str());
				last_mark_index = mark_index;
				mark_index = line.find(',', mark_index + 1);
				camera_rotate_x_portrait = atoi(line.substr(last_mark_index + 1, mark_index - last_mark_index - 1).c_str());
				force_angle_portrait = atoi(line.substr(mark_index + 1).c_str());
				is_camera_pos_portrait_read = true;
			}
		}
		if (!is_camera_pos_landscape_read)
		{
			if (line.compare(0, 17, "camera_landscape:") == 0)
			{
				int mark_index = line.find(',');
				int last_mark_index = mark_index;
				camera_pos_y_landscape = atoi(line.substr(17, mark_index - 17).c_str());
				mark_index = line.find(',', mark_index + 1);
				camera_pos_z_landscape = atoi(line.substr(last_mark_index + 1, mark_index - last_mark_index - 1).c_str());
				last_mark_index = mark_index;
				mark_index = line.find(',', mark_index + 1);
				camera_rotate_x_landscape = atoi(line.substr(last_mark_index + 1, mark_index - last_mark_index - 1).c_str());
				force_angle_landscape = atoi(line.substr(mark_index + 1).c_str());
				is_camera_pos_landscape_read = true;
			}
		}
		if (!is_keycode_0_read)
		{
			if (line.compare(0, 6, "Key 0:") == 0)
			{
				key_code[0] = SDL_Scancode(atoi(line.substr(6).c_str()));
				is_keycode_0_read = true;
			}
		}
		if (!is_keycode_1_read)
		{
			if (line.compare(0, 6, "Key 1:") == 0)
			{
				key_code[1] = SDL_Scancode(atoi(line.substr(6).c_str()));
				is_keycode_1_read = true;
			}
		}
		if (!is_keycode_2_read)
		{
			if (line.compare(0, 6, "Key 2:") == 0)
			{
				key_code[2] = SDL_Scancode(atoi(line.substr(6).c_str()));
				is_keycode_2_read = true;
			}
		}
		if (!is_keycode_3_read)
		{
			if (line.compare(0, 6, "Key 3:") == 0)
			{
				key_code[3] = SDL_Scancode(atoi(line.substr(6).c_str()));
				is_keycode_3_read = true;
			}
		}
		if (line.compare(0, 10, "song_list:") == 0)
		{
			song_list.push_back(line.substr(10));
		}
		if (line.compare(0, 5, "tips:") == 0)
		{
			tips_text.push_back(line.substr(5));
		}
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