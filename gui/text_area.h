#ifndef FORCE_RUSH_GUI_TEXT_AREA_H
#define FORCE_RUSH_GUI_TEXT_AREA_H

#include <string>
#include "../texture_manager.h"
#include "../data.h"

namespace fr
{
	enum TextFormat
	{
		TEXTFORMAT_NONE = 0,
		TEXTFORMAT_MIDDLE = 1,
		TEXTFORMAT_LEFT = 2,
		TEXTFORMAT_RIGHT = 3
	};

	struct TextureCache;

	class TextArea
	{
		public:
			virtual void init(std::string input_text, Point2Di input_position, std::string input_font_path, int input_font_size, Color input_color, TextFormat input_format = TEXTFORMAT_MIDDLE, int input_limited_w = 0, bool input_is_wrapped = false, int input_roll_speed = 0);
			virtual void render();
			virtual void render(int x, int y, TextFormat input_format = TEXTFORMAT_NONE);
			virtual void clear();
			virtual void SetPos(int input_x, int input_y);
			virtual void SetSize(int input_w, int input_h);
			virtual void SetText(std::string text);
			virtual void SetColor(Color input_color);
			virtual void SetAlpha(int input_alpha);
			virtual void SetFont(std::string input_font_path, int input_font_size);
			virtual void SetScale(float input_scale);
			virtual void SetRollSpeed(int input_roll_speed);
			virtual int GetX();
			virtual int GetY();
			virtual int GetUserX();
			virtual int GetUserY();
			virtual int GetW();
			virtual int GetH();
			virtual int GetTextureW();
			virtual int GetTextureH();
			virtual std::string GetText();
			virtual float GetScale();
			virtual void SetSrcRect(Rect input_source_rect);
		protected:
			std::string text;
			std::string font_path;
			int font_size;
			Rect dest_rect;
			Rect source_rect;
			Point2Di position;
			Color color;
			bool is_wrapped;
			int roll_speed;
			TextFormat format;
			int limited_w;
			float scale;
			TextureCache *cache;
			float *vectrices;
			std::string matrix_id;
	};	//class TextArea
};	//namespace fr

#endif	//FORCE_RUSH_GUI_TEXT_AREA_H