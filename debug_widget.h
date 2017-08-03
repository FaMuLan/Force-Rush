#ifndef LUNATIC_MELODY_DEBUG_WIDGET_H
#define LUNATIC_MELODY_DEBUG_WIDGET_H

#include <string>
#include <vector>
#include "control_handler.h"
#include "texture_manager.h"
#include "sprite.h"
#include "system.h"

namespace lm
{
	class DebugWidget
	{
		public:
			static DebugWidget *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new DebugWidget;
					return m_instance;
				}
				return m_instance;
			}
			void init();
			void clear();
			void update();

			void PushLog(std::string load_log);
			void ShowWidget(bool load);
			bool IsShowing();
		private:
			DebugWidget() {}
			~DebugWidget() {}
			static DebugWidget *m_instance;
			std::vector<std::string> text;
			bool is_showing;
			Sprite *background;
			int font_size;
			int row_count;
			int m_x;
			int m_y;
			int m_w;
			int m_h;
			int v_x;
			int v_y;
	};
};

#endif	//LUNATIC_MELODY_DEBUG_WIDGET_H