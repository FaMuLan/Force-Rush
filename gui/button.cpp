#include "button.h"
#include "../control_handler.h"
#include "../texture_manager.h"
#include "../loading/loading_state.h"

void fr::Button::init(std::string input_path, Rect input_dest_rect, Rect input_source_rect)
{
	Sprite::init(input_path, input_dest_rect, input_source_rect);
	is_pressed = false;
}

void fr::Button::update()
{
	is_released = false;
	is_outside_pressed = false;
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
			Finger input_finger = ControlHandler::instance()->GetFinger(i);
			if (input_finger.x >= dest_rect.x && input_finger.y >= dest_rect.y && input_finger.x <= (dest_rect.x + dest_rect.w) && input_finger.y <= (dest_rect.y + dest_rect.h))
			{
				if (!input_finger.moved)
				//從外側劃進來的是不會被檢測到的。
				{
					is_pressed = true;
					has_pressed_id = input_finger.id;
					//防止干擾記錄下標
				}
			}
			else
			{
				is_outside_pressed = true;
			}
		}
	}
	else
	{
		for (int i = 0; i < ControlHandler::instance()->GetFingerCount(); i++)
		{
			Finger input_finger = ControlHandler::instance()->GetFinger(i);
			if (input_finger.id == has_pressed_id)
			{
//				if (input_finger.x < dest_rect.x || input_finger.y < dest_rect.y || input_finger.x > (dest_rect.x + dest_rect.w) || input_finger.y > (dest_rect.y + dest_rect.h))
				if (input_finger.moved)
				{
					is_pressed = false;
				}
				if (input_finger.released)
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
}	//void fr::Button::update()

void fr::Button::render()
{
	Sprite::render();
	for (int i = 0; i < text.size(); i++)
	{
		int x, y;
		x = source_rect.x > text[i]->GetX() ? dest_rect.x : text[i]->GetX() + dest_rect.x - source_rect.x;
		y = source_rect.y > text[i]->GetY() ? dest_rect.y : text[i]->GetY() + dest_rect.y - source_rect.y;
		text[i]->render(x, y, TEXTFORMAT_LEFT);
	}
}

void fr::Button::clear()
{
	Sprite::clear();
	for (int i = 0; i < text.size(); i++)
	{
		text[i]->clear();
		delete text[i];
	}
}

void fr::Button::AddPressedFrame(std::string input_path)
{
	Sprite::AddFrame(input_path);
	pressed_index = frame.size() - 1;
}

bool fr::Button::IsPressed()
{
	return is_pressed && !LoadingState::instance()->IsSwitching();
}

bool fr::Button::IsOutsidePressed()
{
	return is_outside_pressed && !LoadingState::instance()->IsSwitching();
}

bool fr::Button::IsReleased()
{
	return is_released && !LoadingState::instance()->IsSwitching();
}

void fr::Button::AddText(std::string input_text, Point2Di input_position, std::string input_font_path, int input_font_size, Color input_color, TextFormat input_format, int input_limited_w)
{
	TextArea *new_text = new TextArea;
	new_text->init(input_text, input_position, input_font_path, input_font_size, input_color, input_format, input_limited_w);
	text.push_back(new_text);
}

void fr::Button::AddText(TextArea *input_text)
{
	text.push_back(input_text);
}

void fr::Button::ClearText()
{
	text.clear();
}

fr::TextArea *fr::Button::GetText(int index)
{
	return text[index];
}

void fr::Button::SetSize(int w, int h)
{
	Sprite::SetSize(w, h);
}

void fr::Button::SetSrcRect(fr::Rect input_source_rect)
{
	Sprite::SetSrcRect(input_source_rect);
	for (int i = 0; i < text.size(); i++)
	{
		Rect text_source_rect = Rect(0, 0, 0, 0);
		text_source_rect.x = source_rect.x < text[i]->GetX() ? 0 : source_rect.x - text[i]->GetX();
		text_source_rect.y = source_rect.y < text[i]->GetY() ? 0 : source_rect.y - text[i]->GetY();
		text_source_rect.w = (source_rect.x + source_rect.w > text[i]->GetX() + text[i]->GetTextureW()) ? text[i]->GetTextureW() - text_source_rect.x : source_rect.x + source_rect.w - text[i]->GetX() - text_source_rect.x;
		text_source_rect.w = text_source_rect.x < text[i]->GetTextureW() ? text_source_rect.w : 0;
		text_source_rect.w = text_source_rect.w > 0 ? text_source_rect.w : 0;
		text_source_rect.h = (source_rect.y + source_rect.h > text[i]->GetY() + text[i]->GetTextureH()) ? text[i]->GetTextureH() - text_source_rect.y : source_rect.y + source_rect.h - text[i]->GetY() - text_source_rect.y;
		text_source_rect.h = text_source_rect.y < text[i]->GetTextureH() ? text_source_rect.h : 0;
		text_source_rect.h = text_source_rect.h > 0 ? text_source_rect.h : 0;
		text[i]->SetSrcRect(text_source_rect);
		text[i]->SetSize(text_source_rect.w, text_source_rect.h);
	}
}