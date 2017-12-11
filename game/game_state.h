#ifndef LUNATIC_MELODY_GAME_GAME_STATE_H
#define LUNATIC_MELODY_GAME_GAME_STATE_H

#include "../state.h"
#include <string>

namespace lm
{
	class Button;
	class Sprite;
	class TextArea;
	struct SongInformation;

	class GameState : public State
	{
		public:
			static GameState *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new GameState;
					return m_instance;
				}
				return m_instance;
			}
			void init();
			void clear();
			void update();

			void SetFile(SongInformation *load_information);
			void DrawWall();
		private:
			GameState() {}
			~GameState() {}
			static GameState *m_instance;
			SongInformation *m_information;
			Button *title_base;
			Sprite *score_base;
			Sprite *wall_l;
			Sprite *wall_r;
			Sprite *background_l;
			Sprite *background_r;
			TextArea *score_text;
	};	//class GameState : public State
};	//namespace lm

#endif	//LUNATIC_MELODY_GAME_GAME_STATE_H