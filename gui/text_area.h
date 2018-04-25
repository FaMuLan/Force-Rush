#ifndef FORCE_RUSH_GUI_TEXT_AREA_H
#define FORCE_RUSH_GUI_TEXT_AREA_H

#include <string>
#include "../texture_manager.h"

namespace fr
{
	enum TextFormat
	{
		TEXTFORMAT_MIDDLE = 0,
		TEXTFORMAT_LEFT = 1,
		TEXTFORMAT_RIGHT
	};

	struct TextureCache;

	class TextArea
	{
		public:
			virtual void init(std::string text, int x, int y, std::string font_path, int font_size, char r, char g, char b, TextFormat format = TEXTFORMAT_MIDDLE, int limited_w = 0, bool wrapped = false);
			virtual void render();
			virtual void render(int x, int y);
			virtual void clear();
			virtual void SetPos(int x, int y);
			virtual void SetText(std::string text);
			virtual void SetColor(char r, char g, char b);
			virtual void SetFont(std::string font_path, int font_size);
			virtual void SetScale(float scale);
			virtual int GetX();
			virtual int GetY();
			virtual std::string GetText();
			virtual float GetScale();
		protected:
			std::string m_text;
			std::string m_font_path;
			int m_font_size;
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