#ifndef FORCE_RUSH_USER_SETTING_H
#define LUNATIC_MEL0DY_USER_SETTING_H

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

			bool IsAuto();
			int GetDuration();
			int GetOffset();
			float GetDrawScale();
			int GetDrawOffset();
			SDL_Scancode GetKeycode(int index);
			void GetSongList(std::vector<std::string> &output);
			void SwitchAuto();
			void SetDuration(int input);
			void SetOffset(int input);
			void SetKeycode(SDL_Scancode input, int index);
		private:
			Setting() {}
			~Setting() {}
			static Setting *m_instance;
			bool is_auto;
			int duration;
			int offset;
			int draw_scale_portrait;
			int draw_offset_portrait;
			int draw_scale_landscape;
			int draw_offset_landscape;
			SDL_Scancode key_code[4];
			std::vector<std::string> song_list;
	};
};

#endif	//FORCE_RUSH_USER_SETTING_H