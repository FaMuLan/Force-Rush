#ifndef FORCE_RUSH_MESSAGE_BOX_H
#define FORCE_RUSH_MESSAGE_BOX_H

#include <string>
#include <vector>

namespace fr
{
	class Sprite;

	class MessageBox
	{
		public:
			static MessageBox *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new MessageBox;
					return m_instance;
				}
				return m_instance;
			}
			void init();
			void clear();
			void update();
			void render();
			void SetText(std::string text, int time = 0);
		private:
			MessageBox() {}
			~MessageBox() {}
			static MessageBox *m_instance;
			Sprite *base;
			//Sprite *box;
			std::string text_top;
			std::string text_base;
			bool is_base_text_showing;
			int top_show_duration;
	};
};

#endif