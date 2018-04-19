#ifndef FORCE_RUSH_PREPARE_LIST_WIDGET_H
#define FORCE_RUSH_PREPARE_LIST_WIDGET_H

namespace fr
{
	class Sprite;
	class Button;
	class TextArea;

	class ListWidget
	{
		public:
			static ListWidget *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new ListWidget;
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
			ListWidget() {}
			~ListWidget() {}
			static ListWidget *m_instance;
			Sprite *widget_base;
			bool is_shown;
			bool is_entered;
			bool is_exited;

			TextArea *sort_type_label;
			Button *sort_type_left;
			Button *sort_type_right;
			TextArea *sort_type_text;
			TextArea *reverse_label;
			Button *reverse_switch;
	};
};

#endif