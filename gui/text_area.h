#ifndef FORCE_RUSH_GUI_TEXT_AREA_H
#define FORCE_RUSH_GUI_TEXT_AREA_H

#include <string>
#include "../texture_manager.h"
#include "../shape.h"

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
			virtual void init(std::string text, int x, int y, std::string font_path, int font_size, char r, char g, char b, TextFormat format = TEXTFORMAT_MIDDLE, int limited_w = 0, bool wrapped = false);
			virtual void render();
			virtual void render(int x, int y, TextFormat format = TEXTFORMAT_NONE);
			virtual void clear();
			virtual void SetPos(int x, int y);
			virtual void SetSize(int w, int h);
			virtual void SetText(std::string text);
			virtual void SetColor(char r, char g, char b);
			virtual void SetFont(std::string font_path, int font_size);
			virtual void SetScale(float scale);
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
			virtual void SetSrcRect(Rect load_source_rect);
		protected:
			std::string m_text;
			std::string m_font_path;
			int m_font_size;
			Rect dest_rect;
			Rect source_rect;
			int m_x;
			int m_y;
			int m_r;
			int m_g;
			int m_b;
			TextFormat m_format;
			int m_limited_w;
			bool m_wrapped;
			float m_scale;
			TextureCache *cache;
			float *vectrices;
			std::string matrix_id;
	};	//class TextArea
};	//namespace fr

#endif	//FORCE_RUSH_GUI_TEXT_AREA_H