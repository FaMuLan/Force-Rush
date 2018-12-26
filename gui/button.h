#ifndef FORCE_RUSH_GUI_BUTTON_H
#define FORCE_RUSH_GUI_BUTTON_H

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include "../data.h"
#include "../sprite.h"
#include "text_area.h"

namespace fr
{
	typedef SDL_FingerID FingerID;
	class TextArea;

	class Button : public Sprite
	{
		public:
			virtual void init(std::string input_path, Rect input_dest_rect = Rect(0, 0, 0, 0), Rect input_source_rect = Rect(0, 0, 0, 0));
			virtual void clear();
			virtual void update();
			virtual void render();

			virtual void AddPressedFrame(std::string input_path);
			virtual bool IsPressed();
			virtual bool IsReleased();
			virtual bool IsOutsidePressed();
			void AddText(std::string input_text, Point2Di input_position, std::string input_font_path, int input_font_size, Color input_color, TextFormat input_format = TEXTFORMAT_MIDDLE, int input_limited_w = 0);
			void AddText(TextArea *input_text);
			void ClearText();
			TextArea *GetText(int index);

			virtual void SetSize(int w, int h);
			virtual void SetSrcRect(Rect input_source_rect);
		protected:
			bool is_pressed;
			bool is_released;
			bool is_outside_pressed;
			FingerID has_pressed_id;
			//for touch
			int pressed_index;
			//adden
			std::vector<TextArea*> text;
			//text & font
	};	//class Button : public Sprite
};	//namespace fr

#endif	//FORCE_RUSH_GUI_BUTTON_H