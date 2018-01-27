#ifndef FORCE_RUSH_CONTROL_HANDLER_H
#define FORCE_RUSH_CONTROL_HANDLER_H

#include <SDL2/SDL.h>

#include <map>
#include <vector>
#include <string>

namespace fr
{
	typedef SDL_FingerID FingerID;

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
		bool moved;
		bool released;
		FingerID id;
	};
	//觸控有點特殊，考慮到是多點觸控，就把單個手指的數據打包成結構

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
			void init();
			void clear();

			bool IsKeyDown(SDL_Scancode k);

			std::string HearInputText();
			bool IsTextInput();
			bool IsBackspaceDown();
			void SwitchTextInput();

			bool IsMouseButtonDown(MouseButton k);
			void GetMousePos(int &x, int &y);

			int GetFingerCount();
			Finger GetFinger(int index);

			bool IsQuit();
			void update();
		private:
			ControlHandler() {}
			~ControlHandler() {}
			static ControlHandler *m_instance;
			SDL_Event e;
			//基本
			bool quit;
			//是否退出
			const Uint8 *key_state;
			//鍵盤狀態
			std::string input_text;
			bool is_text_input;
			bool is_backspace_down;
			//用於文本輸入
			bool mouse_state[MOUSEBUTTON_TOTAL];
			SDL_Point mouse_pos;
			//鼠標狀態
			std::vector<Finger> finger_state;
			//觸屏狀態
			bool has_keyboard;
			bool has_touch;
			//檢測外設是否有反應
	};	//class ControlHandler
};	//namespace fr

#endif	//FORCE_RUSH_CONTROL_HANDLER_H