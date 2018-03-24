#ifndef FORCE_RUSH_USER_SETTING_H
#define FORCE_RUSH_USER_SETTING_H

#include <SDL2/SDL.h>
#include <vector>
#include <string>

namespace fr
{
	class Setting
	{
		public:
			static Setting *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new Setting;
					return m_instance;
				}
				return m_instance;
			}
			void init();
			bool read();
			void write();

			std::string GetUserProfilePath();
			bool IsAuto();
			bool IsSlideOut();
			int GetDuration();
			int GetOffset();
			int GetCameraPosY();
			int GetCameraPosZ();
			int GetCameraRotateX();
			int GetForceAngle();
			SDL_Scancode GetKeycode(int index);
			void GetSongList(std::vector<std::string> &output);
			std::string GetRandomTips();
			void SetUserProfilePath(std::string input);
			void SwitchAuto();
			void SwitchSlideOut();
			void SetDuration(int input);
			void SetOffset(int input);
			void SetCameraPosY(int input);
			void SetCameraPosZ(int input);
			void SetCameraRotateX(int input);
			void SetForceAngle(int input);
			void SetKeycode(SDL_Scancode input, int index);
		private:
			Setting() {}
			~Setting() {}
			static Setting *m_instance;
			std::string user_profile_path;
			bool is_auto;
			bool is_slide_out;
			int duration;
			int offset;
			int camera_pos_y_portrait;
			int camera_pos_z_portrait;
			int camera_rotate_x_portrait;
			int force_angle_portrait;
			int camera_pos_y_landscape;
			int camera_pos_z_landscape;
			int camera_rotate_x_landscape;
			int force_angle_landscape;
			SDL_Scancode key_code[4];
			std::vector<std::string> song_list;
			std::vector<std::string> tips_text;
	};
};

#endif	//FORCE_RUSH_USER_SETTING_H