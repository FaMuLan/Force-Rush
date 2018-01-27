#include "control_handler.h"
#include "system.h"
#include "texture_manager.h"

fr::ControlHandler *fr::ControlHandler::m_instance = 0;

void fr::ControlHandler::init()
{
	quit = false;
	has_keyboard = false;
	has_touch = false;
	mouse_pos.x = 0;
	mouse_pos.y = 0;
	input_text = "";
	is_text_input = false;
}

void fr::ControlHandler::clear()
{
	
}

bool fr::ControlHandler::IsKeyDown(SDL_Scancode k)
{
	if (has_keyboard)
	{
		return key_state[k];
	}
	return false;
}

std::string fr::ControlHandler::HearInputText()
{
	return input_text;
}

bool fr::ControlHandler::IsTextInput()
{
	return is_text_input;
}

bool fr::ControlHandler::IsBackspaceDown()
{
	return is_backspace_down;
}

void fr::ControlHandler::SwitchTextInput()
{
	if (is_text_input)
	{
		SDL_StopTextInput();
		is_text_input = false;
	}
	else
	{
		SDL_StartTextInput();
		is_text_input = true;
	}
}

bool fr::ControlHandler::IsMouseButtonDown(MouseButton k)
{
	return mouse_state[k];
}

void fr::ControlHandler::GetMousePos(int &x, int &y)
{
	x = mouse_pos.x / System::instance()->GetScale();
	y = mouse_pos.y / System::instance()->GetScale();
}

int fr::ControlHandler::GetFingerCount()
{
	return finger_state.size();
}

fr::Finger fr::ControlHandler::GetFinger(int index)
{
	return finger_state[index];
}

bool fr::ControlHandler::IsQuit()
{
	return quit;
}

void fr::ControlHandler::update()
{
	is_backspace_down = false;
	input_text = "";
	for (std::vector<Finger>::iterator iter = finger_state.begin(); iter != finger_state.end();)
	{
		if (iter->released)
		{
			finger_state.erase(iter);
			iter = finger_state.begin();
		}
		else
		{
			iter++;
		}
	}

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
				has_keyboard = true;
				key_state = SDL_GetKeyboardState(0);
				if (key_state[SDL_SCANCODE_ESCAPE])
				{
					quit = true;
				}
				if( e.key.keysym.sym == SDLK_BACKSPACE)
				{
					is_backspace_down = true;
				}
			break;
			case SDL_TEXTINPUT:
				input_text += e.text.text;
			break;
			case SDL_FINGERDOWN:
			{
				has_touch = true;
				Finger new_finger;
				new_finger.x = e.tfinger.x * System::instance()->GetWindowWidth();
				new_finger.y = e.tfinger.y * System::instance()->GetWindowHeigh();
				new_finger.dx = e.tfinger.dx * System::instance()->GetWindowWidth();
				new_finger.dy = e.tfinger.dy * System::instance()->GetWindowHeigh();
				new_finger.moved = false;
				new_finger.released = false;
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
						finger_state[i].x = e.tfinger.x * System::instance()->GetWindowWidth();
						finger_state[i].y = e.tfinger.y * System::instance()->GetWindowHeigh();
						finger_state[i].dx = e.tfinger.dx * System::instance()->GetWindowWidth();
						finger_state[i].dy = e.tfinger.dy * System::instance()->GetWindowHeigh();
						if (finger_state[i].dx != 0 || finger_state[i].dy != 0)
						//檢測手指是否移動
						{
							finger_state[i].moved = true;
						}
					}
				}
			}
			break;
			case SDL_FINGERUP:
			{
				FingerID load_id = e.tfinger.fingerId;
				for (int i = 0; i < finger_state.size(); i++)
				{
					if (load_id == finger_state[i].id)
					{
						finger_state[i].released = true;
					}
				}
			}
			break;
			case SDL_WINDOWEVENT:
				System::instance()->RefreshWindowSize();
			break;
		}	//switch (e.type)
	}	//while (SDL_PollEvent(&e))
}	//void fr::ControlHandler::update()