#ifndef FORCE_RUSH_BUTTON_H
#define FORCE_RUSH_BUTTON_H

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include "sprite.h"
#include "text_area.h"
#include "shape.h"

namespace fr
{
	typedef SDL_FingerID FingerID;
	class TextArea;

	class Button : public Sprite
	{
		public:
			virtual void init(std::string path, Rect load_dest_rect = Rect(0, 0, 0, 0), Rect load_source_rect = Rect(0, 0, 0, 0));
			virtual void clear();
			virtual void update();
			virtual void render();
			virtual void SetRotation(Point load_centet, double load_angle);

			virtual void AddPressedFrame(std::string path);
			virtual bool IsPressed();
			virtual bool IsReleased();
			void AddText(std::string load_text, int x, int y, std::string font_path, int font_size, char r, char g, char b, TextFormat format = TEXTFORMAT_MIDDLE, int limited_w = 0);
			void AddText(TextArea *load_text);
			void ClearText();
			TextArea *GetText(int index);
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
			Shape press_area;
	};	//class Button : public Sprite
};	//namespace fr

#endif	//FORCE_RUSH_BUTTON_H