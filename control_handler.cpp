#include "control_handler.h"
#include "system.h"
#include "texture_manager.h"

lm::ControlHandler *lm::ControlHandler::m_instance = 0;

void lm::ControlHandler::init()
{
	quit = false;
	mouse_pos.x = 0;
	mouse_pos.y = 0;
	TextureManager::instance()->loadfont("assets/Kazesawa-Light.ttf", 30);
	//for debug rendering
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
	x = mouse_pos.x / System::instance()->GetScale();
	y = mouse_pos.y / System::instance()->GetScale();
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
				key_state = SDL_GetKeyboardState(0);
				if (key_state[SDL_SCANCODE_ESCAPE])
				{
					quit = true;
				}
			break;
			case SDL_FINGERDOWN:
			{
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
}	//void lm::ControlHandler::update()

void lm::ControlHandler::render()
{
	for (int i = 0; i < finger_state.size(); i++)
	{
		char output[50];
		sprintf(output, "id:%d x:%d y:%d dx:%d dy:%d count:%d", finger_state[i].id, finger_state[i].x, finger_state[i].y, finger_state[i].dx, finger_state[i].dy, i);

		std::string output_str(output);
		output_str += finger_state[i].moved ? std::string(" moved:true ") : std::string(" moved:false ");
		output_str += finger_state[i].released ? std::string("released:true") : std::string("released:false");
	
		TextureManager::instance()->render(output_str, finger_state[i].x, finger_state[i].y, "assets/Kazesawa-Light.ttf", 0x00, 0x00, 0x00);
	}
}