#include "button.h"

void lm::Button::load(std::string path, std::string path_pressed, int x, int y, int w, int h)
{
	TextureManager::instance()->load(path);
	TextureManager::instance()->load(path_pressed);
	m_path = path;
	m_path_pressed = path_pressed;
	m_x = x;
	m_y = y;
	m_w = w;
	m_h = h;
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
}	//void lm::Button::update()

void lm::Button::render()
{
	if (is_pressed)
	{
		TextureManager::instance()->render(m_path_pressed, m_x, m_y, m_w, m_h);
	}
	else
	{
		TextureManager::instance()->render(m_path, m_x, m_y, m_w, m_h);
	}
	int text_x = m_x + m_w / 2;
	int text_y = m_y + m_h / 2;
	TextureManager::instance()->render(text, text_x, text_y, font_path, r, g, b);
}

void lm::Button::clear()
{
	
}

bool lm::Button::IsPressed()
{
	return is_pressed;
}

bool lm::Button::IsReleased()
{
	return is_released;
}

void lm::Button::SetPos(int x, int y)
{
	if (x != m_x || y != m_y)
	{
		Sprite::SetPos(x, y);
		is_pressed = false;
	}
}

void lm::Button::SetText(std::string load_text, std::string load_font_path, Uint8 load_r, Uint8 load_g, Uint8 load_b)
{
	text = load_text;
	font_path = load_font_path;
	r = load_r;
	g = load_g;
	b = load_b;
}