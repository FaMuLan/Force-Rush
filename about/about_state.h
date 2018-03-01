#ifndef FORCE_RUSH_ABOUT_ABOUT_STATE_H
#define FORCE_RUSH_ABOUT_ABOUT_STATE_H

#include <string>
#include "../state.h"

namespace fr
{
	class Sprite;
	class Button;
	class TextArea;

	class AboutState : public State
	{
		public:
			static AboutState *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new AboutState;
					return m_instance;
				}
				return m_instance;
			}
			void init();
			void clear();
			void update();
		private:
			AboutState() {}
			~AboutState() {}
			static AboutState *m_instance;
			TextArea *text;
			Button *back;
	};
};

#endif	//FORCE_RUSH_ABOUT_ABOUT_STATE_H