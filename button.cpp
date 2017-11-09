#include "button.h"
#include "control_handler.h"
#include "texture_manager.h"
#include "loading/loading_state.h"

void lm::Button::init(std::string path, int x, int y, int w, int h)
{
	Sprite::init(path, x, y, w, h);
	is_pressed = false;
}

void lm::Button::update()
{
	is_released = false;
	int x, y;
/*
	ControlHandler::instance()->GetMousePos(x, y);
	if (x >= m_x && y >= m_y && x <= (m_x + m_w) && y <= (m_y + m_h))
	{
		if (ControlHandler::instance()->IsMouseButtonDown(MOUSEBUTTON_LEFT))
		{
			is_pressed = true;
		}
		else
		{
			if (is_pressed)
			{
				is_released = true;
			}
			is_pressed = false;
		}
	}
	else
	{
		is_pressed = false;
	}
*/
	if (!is_pressed)
	{
		for (int i = 0; i < ControlHandler::instance()->GetFingerCount(); i++)
		{
			Finger load_finger = ControlHandler::instance()->GetFinger(i);
			if (load_finger.x >= m_x && load_finger.y >= m_y && load_finger.x <= (m_x + m_w) && load_finger.y <= (m_y + m_h))
			{
				if (!load_finger.moved)
				//從外側劃進來的是不會被檢測到的。
				{
					is_pressed = true;
					has_pressed_id = load_finger.id;
					//防止干擾記錄下標
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < ControlHandler::instance()->GetFingerCount(); i++)
		{
			Finger load_finger = ControlHandler::instance()->GetFinger(i);
			if (load_finger.id == has_pressed_id)
			{
				if (load_finger.x < m_x || load_finger.y < m_y || load_finger.x > (m_x + m_w) || load_finger.y > (m_y + m_h))
				{
					is_pressed = false;
				}
				if (load_finger.released)
				{
					is_released = true;
					is_pressed = false;
				}
			}
		}
	}
	Sprite::update();
	if (is_pressed)
	{
		current_index = pressed_index;
	}
	else
	{
		current_index = base_index;
	}
}	//void lm::Button::update()

void lm::Button::render()
{
	Sprite::render();
	for (int i = 0; i < text.size(); i++)
	{
		text[i]->render(m_x + text[i]->GetX(), m_y + text[i]->GetY());
	}
}

void lm::Button::clear()
{
	
}

void lm::Button::AddPressedFrame(std::string path)
{
	Sprite::AddFrame(path);
	pressed_index = frame.size() - 1;
}

bool lm::Button::IsPressed()
{
	return is_pressed;
}

bool lm::Button::IsReleased()
{
	return is_released && !LoadingState::instance()->IsSwitching();
}

void lm::Button::AddText(std::string load_text, int x, int y, std::string font_path, int font_size, char r, char g, char b, TextFormat format, int limited_w)
{
	TextArea *new_text = new TextArea;
	new_text->init(load_text, x, y, font_path, font_size, r, g, b, format, limited_w);
	text.push_back(new_text);
}

void lm::Button::ClearText()
{
	text.clear();
}