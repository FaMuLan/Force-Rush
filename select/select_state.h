#ifndef LUNATIC_MELODY_SELECT_SELECT_STATE_H
#define LUNATIC_MELODY_SELECT_SELECT_STATE_H

#include <string>
#include "../state.h"

namespace lm
{
	class Sprite;
	class Button;

	class SelectState : public State
	{
		public:
			static SelectState *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new SelectState;
					return m_instance;
				}
				return m_instance;
			}
			void init();
			void clear();
			void update();
		private:
			SelectState() {}
			~SelectState() {}
			static SelectState *m_instance;
			Sprite *background;
			Sprite *search_bar;
			Sprite *random_bar;
			Button *select_back;
			Button *select_mod;
			Sprite *select_footer;
	};
};

#endif	//LUNATIC_MELODY_SELECT_SELECT_STATE_H