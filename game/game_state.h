#ifndef LUNATIC_MELODY_GAME_GAME_STATE_H
#define LUNATIC_MELODY_GAME_GAME_STATE_H

#include "../state.h"
#include <string>

namespace lm
{
	class Button;

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

			void SetFile(std::string load_path);
		private:
			GameState() {}
			~GameState() {}
			static GameState *m_instance;
			std::string file_path;
			Button *game_pause;
	};	//class GameState : public State
};	//namespace lm

#endif	//LUNATIC_MELODY_GAME_GAME_STATE_H