#ifndef LUNATIC_MELODY_LOADING_LOADING_STATE_H
#define LUNATIC_MELODY_LOADING_LOADING_STATE_H

#include <string>
#include "../state.h"
#include "../system.h"
#include "../timer.h"
#include "../sound_manager.h"

namespace lm
{
	class LoadingState : public State
	{
		public:
			static LoadingState *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new LoadingState;
					return m_instance;
				}
				return m_instance;
			}
			void update();
			void init();
			void init(State *load_next_state, State *load_last_state);
			void clear();
			void OnEnter();
			void OnExit();
		private:
			LoadingState() {}
			~LoadingState() {}
			static LoadingState *m_instance;
			Sprite *shutter_l;
			Sprite *shutter_r;
			unsigned int animate_duration;
			double a_l;
			double a_r;
			bool is_entered;
			bool is_loaded;
			bool is_exited;
			State* next_state;
			State* last_state;
	};
};

#endif	//LUNATIC_MELODY_LOADING_LOADING_STATE_H