#ifndef LUNATIC_MELODY_SONG_DATA_H
#define LUNATIC_MELODY_SONG_DATA_H

#include <string>

namespace lm
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

	struct Score
	{
		int pure;
		int great;
		int good;
		int error;
		int combo;
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
	};
};

#endif