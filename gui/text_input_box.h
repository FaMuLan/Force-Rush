#ifndef FORCE_RUSH_TEXT_INPUT_BOX_H
#define FORCE_RUSH_TEXT_INPUT_BOX_H

#include <string>
#include "button.h"

namespace fr
{
	class TextArea;

	class TextInputBox : public Button
	{
		public:
			virtual void clear();
			virtual void update();
			virtual void render();

			virtual void InitText(int x, int y, std::string font_path, int font_size, char r, char g, char b, TextFormat format = TEXTFORMAT_MIDDLE, int limited_w = 0);
			virtual std::string GetText();
			virtual void SetText(std::string input_text);
			virtual void SetFont(std::string font_path, int font_size);
		protected:
			TextArea *current_text;
	};
};

#endif