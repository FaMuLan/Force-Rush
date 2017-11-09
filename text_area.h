#ifndef LUNATIC_MELODY_TEXT_AREA_H
#define LUNATIC_MELODY_TEXT_AREA_H

#include <string>

namespace lm
{
	enum TextFormat
	{
		TEXTFORMAT_MIDDLE = 0,
		TEXTFORMAT_LEFT = 1,
		TEXTFORMAT_RIGHT
	};

	class TextArea
	{
		public:
			virtual void init(std::string text, int x, int y, std::string font_path, int font_size, char r, char g, char b, TextFormat format = TEXTFORMAT_MIDDLE, int m_limited_w = 0);
			virtual void render();
			virtual void render(int x, int y);
			virtual void clear();
			virtual void SetPos(int x, int y);
			virtual int GetX();
			virtual int GetY();
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
	};	//class TextArea
};	//namespace lm

#endif	//LUNATIC_MELODY_TEXTAREA_H