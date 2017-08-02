#ifndef LUNATIC_MELODY_MAIN_MAIN_STATE_H
#define LUNATIC_MELODY_MAIN_MAIN_STATE_H

#include "../state.h"
#include "../sprite.h"
#include "../button.h"
#include "../debug_widget.h"
#include "../sound_manager.h"
#include "../loading/loading_state.h"
#include "../crafting/crafting_state.h"
#include "../select/select_state.h"
#include "about_sidedialog.h"

namespace lm
{
	class Button;
	class Sprite;

	class MainState : public State
	{
		public:
			static MainState *instance()
			{
				if (m_instance == NULL)
				{
					m_instance = new MainState;
					return m_instance;
				}
				return m_instance;
			}
			void init();
			void clear();
			void update();
			void lock(bool load);
		private:
			MainState() {}
			~MainState() {}
			static MainState *m_instance;
			Button *main_start;
			Button *main_debug;
			Button *main_about;
			Sprite *main_character;
			Sprite *background;
			bool is_locked;
	};
};

#endif	//LUNATIC_MELODY_MAIN_MAIN_STATE_H