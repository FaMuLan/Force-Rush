#ifndef FORCE_RUSH_SYSTEM_H
#define FORCE_RUSH_SYSTEM_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <map>
#include <string>

namespace fr
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
			int GetWindowDepth();
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
			EGLContext gles_context;
			GLuint program_object;
			int window_width;
			int window_heigh;
			int window_depth;
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
};	//namespace fr

#endif	//FORCE_RUSH_SYSTEM_H