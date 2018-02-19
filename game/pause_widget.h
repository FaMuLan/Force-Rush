#ifndef FORCE_RUSH_GAME_PAUSE_WIDGET_H
#define FORCE_RUSH_GAME_PAUSE_WIDGET_H

namespace fr
{
	class Sprite;

	class PauseWidget
	{
		public:
			static PauseWidget *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new PauseWidget;
					return m_instance;
				}
				return m_instance;
			}
			void init();
			void clear();
			void update();
			void render();
			void OnEnter();
			void OnExit();
			void SwitchShown();
			bool IsShown();
		private:
			PauseWidget() {}
			~PauseWidget() {}
			static PauseWidget *m_instance;

			bool is_shown;
			bool is_entered;
			bool is_exited;
			Sprite *widget_base;
	};
};

#endif