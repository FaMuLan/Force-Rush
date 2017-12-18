#ifndef FORCE_RUSH_SELECT_SELECT_STATE_H
#define FORCE_RUSH_SELECT_SELECT_STATE_H

#include <string>
#include "../state.h"

namespace fr
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
			Button *select_back;
			Button *select_mod;
			Button *select_refresh;
	};
};

#endif	//FORCE_RUSH_SELECT_SELECT_STATE_H