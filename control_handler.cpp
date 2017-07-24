#include "control_handler.h"

lm::ControlHandler *lm::ControlHandler::m_instance = 0;

void lm::ControlHandler::init()
{
	quit = false;
	mouse_pos.x = 0;
	mouse_pos.y = 0;
	TextureManager::instance()->loadfont("assets/Kazesawa-Light.ttf", 30);
//	touch_state.reserve(12);
}

void lm::ControlHandler::clear()
{
	
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

int lm::ControlHandler::GetFingerCount()
{
	return finger_state.size();
}

lm::Finger lm::ControlHandler::GetFinger(int index)
{
	return finger_state[index];
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
			{
				Finger new_finger;
				new_finger.x = e.tfinger.x * lm::System::instance()->GetWindowWidth();
				new_finger.y = e.tfinger.y * lm::System::instance()->GetWindowHeigh();
				new_finger.dx = e.tfinger.dx * lm::System::instance()->GetWindowWidth();
				new_finger.dy = e.tfinger.dy * lm::System::instance()->GetWindowHeigh();
				new_finger.id = e.tfinger.fingerId;
				finger_state.push_back(new_finger);
			}
			case SDL_FINGERMOTION:
			{
				FingerID load_id = e.tfinger.fingerId;
				for (int i = 0; i < finger_state.size(); i++)
				{
					if (load_id == finger_state[i].id)
					{
						finger_state[i].x = e.tfinger.x * lm::System::instance()->GetWindowWidth();
						finger_state[i].y = e.tfinger.y * lm::System::instance()->GetWindowHeigh();
						finger_state[i].dx = e.tfinger.dx * lm::System::instance()->GetWindowWidth();
						finger_state[i].dy = e.tfinger.dy * lm::System::instance()->GetWindowHeigh();
					}
				}
			}
			break;
			case SDL_FINGERUP:
			{
				FingerID load_id = e.tfinger.fingerId;
				std::vector<Finger>::iterator iter;
				for (iter = finger_state.begin(); iter != finger_state.end();)
				{
					if (iter->id == load_id)
					{
						finger_state.erase(iter);
						iter = finger_state.begin();
					}
					else
					{
						iter++;
					}
				}
			}
			break;
		}	//switch (e.type)
	}	//while (SDL_PollEvent(&e))
}	//void lm::ControlHandler::update()

void lm::ControlHandler::render()
{
	for (int i = 0; i < finger_state.size(); i++)
	{
		char output[50];
		sprintf(output, "id:%d x:%d y:%d dx:%d dy:%d", finger_state[i].id, finger_state[i].x, finger_state[i].y, finger_state[i].dx, finger_state[i].dy);
		std::string output_str(output);
		TextureManager::instance()->render(output_str, finger_state[i].x, finger_state[i].y, "assets/Kazesawa-Light.ttf", 0x00, 0x00, 0x00);
	}
}