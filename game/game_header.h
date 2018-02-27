#ifndef FORCE_RUSH_GAME_GAME_HEADER_H
#define FORCE_RUSH_GAME_GAME_HEADER_H

namespace fr
{
	class Sprite;
	class Button;
	class TextArea;

	class GameHeader
	{
		public:
			static GameHeader *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new GameHeader;
					return m_instance;
				}
				return m_instance;
			}
			void init();
			void clear();
			void update();
			void render();
			void PauseOnEnter();
			void PauseOnExit();
			void SwitchPause();
			bool IsPaused();
		private:
			GameHeader() {}
			~GameHeader() {}
			static GameHeader *m_instance;

			bool pause_is_shown;
			bool pause_is_entered;
			bool pause_is_exited;
			Sprite *pause_widget_base;
			Button *title_base;
			Sprite *score_base;
			Sprite *user_base;
			Sprite *duration_process_bar;

			Button *pause_resume;
			Button *pause_retry;
			Button *pause_retire;

			TextArea *score_text;
			TextArea *duration_text;
			TextArea *user_name_text;
			TextArea *tips_text;
	};
};

#endif