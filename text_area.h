#ifndef LUNATIC_MELODY_TEXT_AREA_H
#define LUNATIC_MELODY_TEXT_AREA_H

#include <string>

namespace lm
{
	class TextArea
	{
		public:
			virtual void load(std::string text, std::string font_path, int font_size, int r, int g, int b);
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
	};	//class TextArea
};	//namespace lm

#endif	//LUNATIC_MELODY_TEXTAREA_H