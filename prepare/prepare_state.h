#ifndef FORCE_RUSH_PREPARE_PREPARE_STATE_H
#define FORCE_RUSH_PREPARE_PREPARE_STATE_H

#include <string>
#include "../state.h"

namespace fr
{
	class Sprite;
	class Button;

	class PrepareState : public State
	{
		public:
			static PrepareState *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new PrepareState;
					return m_instance;
				}
				return m_instance;
			}
			void init();
			void clear();
			void update();
		private:
			PrepareState() {}
			~PrepareState() {}
			static PrepareState *m_instance;
			Sprite *background;
			Button *prepare_back;
			Button *prepare_mod;
			Button *prepare_list;
			Button *prepare_refresh;
	};
};

#endif	//FORCE_RUSH_PREPARE_PREPARE_STATE_H