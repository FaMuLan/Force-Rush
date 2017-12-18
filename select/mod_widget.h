#ifndef FORCE_RUSH_SELECT_MOD_WIDGET_H
#define FORCE_RUSH_SELECT_MOD_WIDGET_H

namespace fr
{
	class Sprite;
	class Button;
	class TextArea;

	class ModWidget
	{
		public:
			static ModWidget *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new ModWidget;
					return m_instance;
				}
				return m_instance;
			}

			void init();
			void clear();
			void update();
			void render();
			void SwitchShown();
			bool IsShown();

			void OnEnter();
			void OnExit();
		private:
			ModWidget() {}
			~ModWidget() {}
			static ModWidget *m_instance;
			Sprite *widget_base;
			bool is_shown;
			bool is_entered;
			bool is_exited;

			TextArea *auto_text;
			Button *auto_switch;
			TextArea *duration_text;
			Button *duration_left;
			Button *duration_left_dual;
			Button *duration_right;
			Button *duration_right_dual;
			TextArea *duration_num;
			TextArea *offset_text;
			Button *offset_left;
			Button *offset_left_dual;
			Button *offset_right;
			Button *offset_right_dual;
			TextArea *offset_num;
	};
};

#endif