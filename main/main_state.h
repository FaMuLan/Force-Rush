#ifndef FORCE_RUSH_MAIN_MAIN_STATE_H
#define FORCE_RUSH_MAIN_MAIN_STATE_H

#include "../state.h"

namespace fr
{
	class Button;
	class TextArea;
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
			Button *main_about;
			Sprite *widget_base;
			Sprite *performance_process_bar;
			TextArea *user_name_text;
			TextArea *performance_point_text;
			bool is_locked;
	};
};

#endif	//FORCE_RUSH_MAIN_MAIN_STATE_H