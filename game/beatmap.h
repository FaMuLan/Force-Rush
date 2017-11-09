#ifndef LUNATIC_MELODY_GAME_BEATMAP_H
#define LUNATIC_MELODY_GAME_BEATMAP_H

#include <map>
#include <vector>
#include <string>

namespace lm
{
	class Column;

	enum Judgement
	{
		JUDGEMENT_NONE = 0,
		JUDGEMENT_PG = 2,	//P GREAT
		JUDGEMENT_GR = 1,	//GREAT
		JUDGEMENT_GD = 0,	//GOOD
		JUDGEMENT_ER = -5	//ERROR
	};
	//這樣枚舉就可以直接跟分數放一起運算了
	//特別提醒:為了拉開分數差距，GOOD不加分，ERROR會扣分

	struct Note
	{
		int time;
		int time_end;
	};

	class Beatmap
	{
		public:
			static Beatmap *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new Beatmap;
					return m_instance;
				}
				return m_instance;
			}
			void load(std::string path);
			void clear();
			void update();
			void render();
			Judgement judge(int note_time, bool is_pressed = true, bool is_ln_pressing = false);
			//第三個參數中考慮到ln尾判的判定與一般note不一樣
			int GetCombo();
			int GetScore();
			int GetDuration();
			int GetOffset();
		private:
			Beatmap() {}
			~Beatmap() {}
			static Beatmap *m_instance;
			std::vector<Column*> m_column;
			std::map<Judgement, int> judge_count;
			std::map<int, int> column_mapper;
			std::string audio_path;
			int offset;
			int note_duration;
			int score;
			int combo;
	};
};

#endif	//LUNATIC_MELODY_GAME_BEATMAP_H