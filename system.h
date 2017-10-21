#ifndef LUNATIC_MELODY_SYSTEM_H
#define LUNATIC_MELODY_SYSTEM_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <map>
#include <string>

namespace lm
{
	class State;

	enum WindowRotation
	{
		WINDOWROTATION_PORTRAIT = 0,
		WINDOWROTATION_LANDSCAPE
	};

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
			void init();
			void run();
			void clear();
			void RefreshWindowSize();
			bool IsWindowModified();
			void SetBackgroundColor(char r, char g, char b);
			int GetWindowWidth();
			int GetWindowHeigh();
			int GetScreenWidth();
			int GetScreenHeigh();
			float GetScale();
			WindowRotation GetWindowRotation();
			State *current_state;
		private:
			System() {}
			~System() {}
			static System *m_instance;
			SDL_Window *system_window;
			SDL_Renderer *system_renderer;
			int window_width;
			int window_heigh;
			int screen_width;
			int screen_heigh;
			char bg_r, bg_g, bg_b;
//			std::map<std::string, State*> m_state;
			bool m_loading;
			bool is_window_modified;
			float scale;
			WindowRotation rotation;
			//用於縮放
	};	//class System
};	//namespace lm

#endif	//LUNATIC_MELODY_SYSTEM_H