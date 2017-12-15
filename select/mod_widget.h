#ifndef LUNATIC_MELODY_SELECT_MOD_WIDGET_H
#define LUNATIC_MELODY_SELECT_MOD_WIDGET_H

namespace lm
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
	};
};

#endif