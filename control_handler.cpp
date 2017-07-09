#include "control_handler.h"

lm::ControlHandler *lm::ControlHandler::m_instance = 0;

lm::ControlHandler::ControlHandler()
{
	quit = false;
	mouse_pos.x = 0;
	mouse_pos.y = 0;
}

bool lm::ControlHandler::IsKeyDown(SDL_Scancode k)
{
	return key_state[k];
}

bool lm::ControlHandler::IsMouseButtonDown(MouseButton k)
{
	return mouse_state[k];
}

void lm::ControlHandler::GetMousePos(int &x, int &y)
{
	x = mouse_pos.x;
	y = mouse_pos.y;
}

bool lm::ControlHandler::IsQuit()
{
	return quit;
}

void lm::ControlHandler::update()
{
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
			case SDL_QUIT:
				quit = true;
			break;
			case SDL_MOUSEMOTION:
				mouse_pos.x = e.motion.x;
				mouse_pos.y = e.motion.y;
			break;
			case SDL_MOUSEBUTTONDOWN:
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					mouse_state[MOUSEBUTTON_LEFT] = true;
				}
				if (e.button.button == SDL_BUTTON_MIDDLE)
				{
					mouse_state[MOUSEBUTTON_MIDDLE] = true;
				}
				if (e.button.button == SDL_BUTTON_RIGHT)
				{
					mouse_state[MOUSEBUTTON_RIGHT] = true;
				}
			break;
			case SDL_MOUSEBUTTONUP:
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					mouse_state[MOUSEBUTTON_LEFT] = false;
				}
				if (e.button.button == SDL_BUTTON_MIDDLE)
				{
					mouse_state[MOUSEBUTTON_MIDDLE] = false;
				}
				if (e.button.button == SDL_BUTTON_RIGHT)
				{
					mouse_state[MOUSEBUTTON_RIGHT] = false;
				}
			break;
			case SDL_KEYDOWN:
				key_state = SDL_GetKeyboardState(0);
				if (key_state[SDL_SCANCODE_ESCAPE])
				{
					quit = true;
				}
			break;
			case SDL_FINGERDOWN:
			case SDL_FINGERMOTION:
				mouse_pos.x = e.tfinger.x * lm::System::instance()->GetWindowWidth();
				mouse_pos.y = e.tfinger.y * lm::System::instance()->GetWindowHeigh();
				mouse_state[MOUSEBUTTON_LEFT] = true;
			break;
			case SDL_FINGERUP:
				mouse_state[MOUSEBUTTON_LEFT] = false;
			break;
		}	//switch (e.type)
	}	//while (SDL_PollEvent(&e))
}	//void ControlHandler::update()
