#ifndef LUNATIC_ΜELODY_SELECT_SELECT_STATE_H
#define LUNATIC_MELODY_SELECT_SELECT_STATE_H

#include <vector>
#include <string>
#include "state.h"
#include "button.h"

namespace lm
{
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
	};
};

#endif	//LUNATIC_MELODY_SELECT_SELECT_STATE_H