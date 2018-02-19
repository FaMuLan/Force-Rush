#include "text_input_box.h"
#include <SDL2/SDL.h>
#include "text_area.h"
#include "control_handler.h"

void fr::TextInputBox::clear()
{
	Button::clear();
	current_text->clear();
}

void fr::TextInputBox::update()
{
	Button::update();
	std::string current_text_str = current_text->GetText();

	if (is_released)
	{
		ControlHandler::instance()->SwitchTextInput();
	}

	if (ControlHandler::instance()->IsTextInput())
	{
		current_text_str += ControlHandler::instance()->HearInputText();
		if (ControlHandler::instance()->IsBackspaceDown() && current_text_str.length() > 0)
		{
			current_text_str.pop_back();
		}
		current_text->SetText(current_text_str);
		if (is_outside_pressed)
		{
			ControlHandler::instance()->SwitchTextInput();
		}
	}
}

void fr::TextInputBox::render()
{
	Button::render();
	current_text->render(dest_rect.x + current_text->GetX(), dest_rect.y + current_text->GetY());
}

void fr::TextInputBox::InitText(int x, int y, std::string font_path, int font_size, char r, char g, char b, TextFormat format, int limited_w)
{
	current_text = new TextArea;
	current_text->init("", x, y, font_path, font_size, r, g, b, format, limited_w);
}

std::string fr::TextInputBox::GetText()
{
	return current_text->GetText();
}

void fr::TextInputBox::SetText(std::string input_text)
{
	current_text->SetText(input_text);
}

void fr::TextInputBox::SetFont(std::string font_path, int font_size)
{
	current_text->SetFont(font_path, font_size);
}