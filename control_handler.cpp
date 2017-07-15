#include "control_handler.h"

lm::ControlHandler *lm::ControlHandler::m_instance = 0;

lm::ControlHandler::ControlHandler()
{
	quit = false;
	mouse_pos.x = 0;
	mouse_pos.y = 0;
//	touch_state.reserve(12);
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

bool lm::ControlHandler::GetTouch(lm::Finger *output)
{
	if (finger_count == touch_state.size() - 1)
	{
		finger_count = 0;
		return false;
	}
	output = touch_state[finger_count];
	finger_count++;	
	return true;
}

bool lm::ControlHandler::IsQuit()
{
	return quit;
}

void lm::ControlHandler::update()
{
	for (unsigned int i = 0; i < touch_state.size(); i++)
	{
		delete touch_state[i];
	}
	touch_state.clear();

	while (SDL_PollEvent(&e))
	{
		Finger *newFinger = new Finger;
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
				newFinger->x = e.tfinger.x * lm::System::instance()->GetWindowWidth();
				newFinger->y = e.tfinger.y * lm::System::instance()->GetWindowHeigh();
				newFinger->dx = e.tfinger.dx * lm::System::instance()->GetWindowWidth();
				newFinger->dy = e.tfinger.dy * lm::System::instance()->GetWindowHeigh();
				newFinger->state = SDL_FINGERDOWN;
				touch_state.push_back(newFinger);
			break;
			case SDL_FINGERUP:
				mouse_state[MOUSEBUTTON_LEFT] = false;
				newFinger->x = e.tfinger.x * lm::System::instance()->GetWindowWidth();
				newFinger->y = e.tfinger.y * lm::System::instance()->GetWindowHeigh();
				newFinger->dx = 0;
				newFinger->dy = 0;
				newFinger->state = SDL_FINGERUP;
				touch_state.push_back(newFinger);
			break;
		}	//switch (e.type)
	}	//while (SDL_PollEvent(&e))
}	//void ControlHandler::update()