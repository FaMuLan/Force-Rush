#ifndef LUNATIC_MELODY_CONTROL_HANDLER_H
#define LUNATIC_MELODY_CONTROL_HANDLER_H

#include <SDL2/SDL.h>
#include <map>

#include "system.h"

namespace lm
{
	enum MouseButton
	{
		MOUSEBUTTON_LEFT = 0,
		MOUSEBUTTON_MIDDLE = 1,
		MOUSEBUTTON_RIGHT = 2,
		MOUSEBUTTON_TOTAL = 3
	};

	class ControlHandler
	{
		public:
			static ControlHandler *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new ControlHandler;
					return m_instance;
				}
				return m_instance;
			}
			bool IsKeyDown(SDL_Scancode k);
			bool IsMouseButtonDown(MouseButton k);
			void GetMousePos(int &x, int &y);
			bool IsQuit();
			void update();
		private:
			ControlHandler();
			~ControlHandler() {}
			static ControlHandler *m_instance;
			SDL_Event e;
			bool quit;
			const Uint8 *key_state;
			bool mouse_state[MOUSEBUTTON_TOTAL];
			SDL_Point mouse_pos;
	};	//class ControlHandler
};	//namespace lm

#endif	//LUNATIC_MELODY_CONTROL_HANDLER_H