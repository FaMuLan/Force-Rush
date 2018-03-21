#ifndef FORCE_RUSH_GAME_BEATMAP_H
#define FORCE_RUSH_GAME_BEATMAP_H

#include <map>
#include <vector>
#include <string>

namespace fr
{
	class Column;
	class Sprite;
	class TextArea;
	struct SongInformation;
	struct Score;
	struct Note;

	enum Judgement
	{
		JUDGEMENT_NONE = -1,	//不與其他判定衝突
		JUDGEMENT_SAFE = 0,	//與ERROR相對的判定
		JUDGEMENT_PG = 2,	//P GREAT
		JUDGEMENT_GR = 1,	//GREAT
		JUDGEMENT_GD = -2,	//GOOD
		JUDGEMENT_ER = -5	//ERROR
	};
	//這樣枚舉就可以直接跟分數放一起運算了
	//特別提醒:為了拉開分數差距，GOOD不加分，ERROR會扣分

	class Beatmap
	{
		public:
			virtual void init();
			virtual void clear();
			virtual void update();
			virtual void render();
			virtual Judgement judge(int note_time, bool is_pressed = true, bool is_ln_pressing = false, bool error_only = false) = 0;
			virtual void AddForce(float angle);
		protected:
			std::string audio_path;
			Sprite *play_base;
			Sprite *wall;
			Sprite *ground;
			int *wall_vectrices;
			int *ground_vectrices;
			int *play_base_vectrices;
			std::vector<Column*> m_column;
			float last_angle;
			float current_angle;
			float last_force;
			float force;
			float angle_diff;
	};

	class GameBeatmap : public Beatmap
	{
		public:
			static GameBeatmap *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new GameBeatmap;
					return m_instance;
				}
				return m_instance;
			}
			void load(SongInformation *load_information);
			void clear();
			void update();
			void render();
			Judgement judge(int note_time, bool is_pressed = true, bool is_ln_pressing = false, bool error_only = false);
			//第三個參數中考慮到ln尾判的判定與一般note不一樣
			int GetChain();
			int GetScore();
		private:
			GameBeatmap() {}
			~GameBeatmap() {}
			static GameBeatmap *m_instance;
			SongInformation *m_information;
			Score *m_score;
			std::map<Judgement, int> judge_count;
			TextArea *chain_text;
			TextArea *judge_text;
			TextArea *early_text;
			TextArea *late_text;
			
			bool show_early;
			bool show_late;
			bool is_waiting;
			bool is_ended;
			int current_chain;
	};
};

#endif	//FORCE_RUSH_GAME_BEATMAP_H