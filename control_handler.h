#ifndef LUNATIC_MELODY_CONTROL_HANDLER_H
#define LUNATIC_MELODY_CONTROL_HANDLER_H

#include <SDL2/SDL.h>

#include <map>
#include <vector>

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

	struct Finger
	{
		int x;
		int y;
		int dx;
		int dy;
		SDL_EventType state;
	};
	//觸控有點特殊，考慮到是多點觸控，就把單個手指的數據打包成結構體

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

			bool GetTouch(Finger *output);

			bool IsQuit();
			void update();
		private:
			ControlHandler();
			~ControlHandler() {}
			static ControlHandler *m_instance;
			SDL_Event e;
			//基本
			bool quit;
			//是否退出
			const Uint8 *key_state;
			//鍵盤狀態
			bool mouse_state[MOUSEBUTTON_TOTAL];
			SDL_Point mouse_pos;
			//鼠標狀態
			std::vector<Finger*> touch_state;
			int finger_count;
			//觸屏狀態
	};	//class ControlHandler
};	//namespace lm

#endif	//LUNATIC_MELODY_CONTROL_HANDLER_H