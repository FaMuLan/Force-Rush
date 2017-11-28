#ifndef LUNATIC_MELODY_USER_SETTING_H
#define LUNATIC_MEL0DY_USER_SETTING_H

#include <SDL2/SDL.h>
#include <vector>
#include <string>

namespace lm
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
			SDL_Scancode GetKeycode(int index);
			void GetSongList(std::vector<std::string> &output);
		private:
			Setting() {}
			~Setting() {}
			static Setting *m_instance;
			bool is_auto;
			int duration;
			int offset;
			SDL_Scancode key_code[4];
			std::vector<std::string> song_list;
	};
};

#endif	//LUNATIC_MELODY_USER_SETTING_H