#ifndef FORCE_RUSH_GAME_RESULT_STATE_H
#define FORCE_RUSH_GAME_RESULT_STATE_H

#include "../state.h"

namespace fr
{
	class Sprite;
	class Button;
	struct SongInformation;
	struct Score;

	class ResultState : public State
	{
		public:
			static ResultState *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new ResultState;
					return m_instance;
				}
				return m_instance;
			}
			void init();
			void clear();
			void update();

			void LoadScore(SongInformation *load_information, Score *load_score);
		private:
			ResultState() {}
			~ResultState() {}
			static ResultState *m_instance;
			Sprite *song_base;
			Sprite *score_base;
			Sprite *rank_base;
			Sprite *performance_base;
			Sprite *user_base;
			Button *b_return;
			Button *b_retry;
			SongInformation *m_information;
			Score *m_score;
	};
};

#endif	//FORCE_RUSH_GAME_RESULT_STATE_H