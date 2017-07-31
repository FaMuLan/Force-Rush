#ifndef LUNATIC_MELODY_MAIN_ABOUT_SIDEDIALOG_H
#define LUNATIC_MELODY_MAIN_ABOUT_SIDEDIALOG_H

#include <string>
#include "../control_handler.h"
#include "../sprite.h"
#include "../texture_manager.h"
#include "../timer.h"
#include "main_state.h"

namespace lm
{
	class AboutSidedialog
	{
		public:
			static AboutSidedialog *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new AboutSidedialog;
					return m_instance;
				}
				return m_instance;
			}
			void init();
			void clear();
			void update();
			void render();

			void open();
		private:
			AboutSidedialog() {}
			~AboutSidedialog() {}
			static AboutSidedialog *m_instance;
			Sprite *sidedialog_bottom;
			Sprite *sidedialog_top;
			float a_bottom;
			float a_top;
			int animate_duration;
			bool is_entered;
			bool ready_to_exit;
			bool is_exited;
	};
};

#endif	//LUNATIC_MELODY_MAIN_ABOUT_SIDEDIALOG_H