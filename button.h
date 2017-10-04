#ifndef LUNATIC_MELODY_BUTTON_H
#define LUNATIC_MELODY_BUTTON_H

#include <SDL2/SDL.h>
#include <string>

#include "sprite.h"
#include "texture_manager.h"

namespace lm
{
	typedef SDL_FingerID FingerID;

	class Button : public Sprite
	{
		public:
			virtual void load(std::string path, std::string path_pressed, int x, int y, int w, int h);
			virtual void clear();
			virtual void update();
			virtual void render();

			virtual bool IsPressed();
			virtual bool IsReleased();
			void SetText(std::string load_text, std::string load_font_path, int load_font_size, Uint8 load_r, Uint8 load_g, Uint8 load_b);
			void SetTextPos(int x, int y, TextFormat load_format, int load_limited_w);
		protected:
			bool is_pressed;
			bool is_released;
			FingerID has_pressed_id;
			//for touch
			std::string m_path_pressed;
			//adden
			std::string text;
			std::string font_path;
			int font_size;
			char r, g, b;
			int text_x, text_y, limited_w;
			TextFormat format;
			//text & font
	};	//class Button : public Sprite
};	//namespace lm

#endif	//LUNATIC_MELODY_BUTTON_H