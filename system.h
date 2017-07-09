#ifndef LUNATIC_MELODY_SYSTEM_H
#define LUNATIC_MELODY_SYSTEM_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <map>
#include <string>

#include "control_handler.h"
#include "state.h"
#include "main/main_state.h"
#include "crafting/crafting_state.h"
#include "loading/loading_state.h"
#include "texture_manager.h"
#include "sound_manager.h"

namespace lm
{
	class System
	{
		public:
			static System *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new System;
					return m_instance;
				}
				return m_instance;
			}
			bool init();
			bool run();
			bool clear();
			int GetWindowWidth()
			{
				return screen_width;
			}
			int GetWindowHeigh()
			{
				return screen_heigh;
			}
			bool PushState(std::string id, State *s);
			bool SwitchState(std::string id);

			State *current_state;
		private:
			System() {}
			~System() {}
			static System *m_instance;
			SDL_Window *system_window;
			SDL_Renderer *system_renderer;
			int screen_width;
			int screen_heigh;

			std::map<std::string, State*> m_state;
			bool m_loading;
	};	//class System
};	//namespace lm

#endif	//LUNATIC_MELODY_SYSTEM_H