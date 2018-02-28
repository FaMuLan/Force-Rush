#ifndef FORCE_RUSH_GAME_RESULT_STATE_H
#define FORCE_RUSH_GAME_RESULT_STATE_H

#include "../state.h"

namespace fr
{
	class Sprite;
	class TextArea;
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
			Sprite *user_base;
			Button *b_return;
			Button *b_retry;
			SongInformation *m_information;
			Score *m_score;

			TextArea *title_text;
			TextArea *artist_text;
			TextArea *version_text;
			TextArea *difficulty_text;
			TextArea *score_text;
			TextArea *pure_text;
			TextArea *safe_text;
			TextArea *warning_text;
			TextArea *error_text;
			TextArea *chain_text;
			TextArea *acc_text;
			TextArea *rank_text;
			TextArea *user_name_text;
			TextArea *performance_point_text;
			TextArea *new_record_text;
			bool is_new_record;
			TextArea *new_performance_text;
			bool is_new_performance;
	};
};

#endif	//FORCE_RUSH_GAME_RESULT_STATE_H