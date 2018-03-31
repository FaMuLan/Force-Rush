#ifndef FORCE_RUSH_SONG_DATA_H
#define FORCE_RUSH_SONG_DATA_H

#include <string>

namespace fr
{
	enum Rank
	{
		RANK_NONE = 0,
		RANK_D,
		RANK_C,
		RANK_B,
		RANK_A,
		RANK_S,
	};

	enum NoteType
	{
		NOTETYPE_NORMAL = 0,
		NOTETYPE_SLIDE_IN_LEFT = 1,
		NOTETYPE_SLIDE_IN_RIGHT = 2,
		NOTETYPE_SLIDE_OUT_LEFT = 3,
		NOTETYPE_SLIDE_OUT_RIGHT = 4,
		NOTETYPE_SLIDE_THROUGH = 5,
		NOTETYPE_SLIDE_END_LEFT = 6,
		NOTETYPE_SLIDE_END_RIGHT = 7
	};

	struct Note
	{
		unsigned int time;
		unsigned int time_end;
		NoteType type;
		NoteType type_end;
	};

	struct Timeline
	{
		unsigned int start_time;
		unsigned int end_time;
		float bpm;
		float speed;
	};

	struct NoteSet
	{
		std::vector<Note*> note;
		std::vector<Timeline*> timeline;
	};

	struct Score
	{
		int pure;
		int safe;
		int warning;
		int error;
		int chain;
		int score;
		Rank rank;
	};

	struct SongInformation
	{
		std::string id;
		std::string title;
		std::string artist;
		std::string noter;
		std::string version;
		int difficulty;
		int duration;
		std::string audio_path;
		int preview_time;
		std::string file_path;
		Score *high_score;
		int full_score;
	};

	inline bool CompareNote(const Note *note1, const Note *note2)
	{
		return note1->time < note2->time;
	}
	//用於排序
};

#endif