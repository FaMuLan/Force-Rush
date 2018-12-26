#ifndef FORCE_RUSH_DATA_H
#define FORCE_RUSH_DATA_H

#include <string>
#include <vector>

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

	class Beat
	{
		public:
			Beat() {}
			Beat(unsigned int input_bar, unsigned int input_beat, unsigned int input_divide) : bar(input_bar), beat(input_beat), divide(input_divide) {}
			unsigned int bar;
			unsigned int beat;
			unsigned int divide;
	};

	struct BeatNote
	{
		Beat time;
		Beat time_end;
		int column;
	};

	struct BeatTimeline
	{
		Beat start_time;
		float bpm;
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
	
	class Vector2Di
	{
		public:
			Vector2Di() {}
			Vector2Di(int input_x, int input_y);
			Vector2Di(double input_length, double input_angle);
			Vector2Di operator+(Vector2Di input);
			Vector2Di operator-(Vector2Di input);
			Vector2Di operator*(double input);
			Vector2Di operator/(double input);
			int operator*(Vector2Di input);
			double GetIntersectingAngle(Vector2Di input);

			int x;
			int y;
			double length;
			double angle;
	};

	class Vector3Df
	{
		public:
			Vector3Df() {}
			Vector3Df(float input_x, float input_y, float input_z) : x(input_x), y(input_y), z(input_z) {}
			Vector3Df(double input_length, double input_angle);
			Vector3Df operator+(Vector3Df input);
			Vector3Df operator-(Vector3Df input);
			Vector3Df operator*(double input);
			Vector3Df operator/(double input);
			int operator*(Vector3Df input);

			float x;
			float y;
			float z;
			double length;
			double angle_x;
			double angle_y;
	};

	class Point2Di
	{
		public:
			Point2Di() {}
			Point2Di(int input_x, int input_y) : x(input_x), y(input_y) {}
			Vector2Di operator-(Point2Di input);
			Point2Di operator+(Vector2Di input);
			int x;
			int y;
	};

	class Point3Df
	{
		public:
			Point3Df() {}
			Point3Df(float input_x, float input_y, float input_z) : x(input_x), y(input_y), z(input_z) {}
			Vector3Df operator-(Point3Df input);
			Point3Df operator+(Vector3Df input);
			float x;
			float y;
			float z;
	};

	class Color
	{
		public:
			Color() {}
			Color(int input_r, int input_g, int input_b, int input_a = 0) : r(input_r), g(input_g), b(input_b), a(input_a) {}
			int r;
			int g;
			int b;
			int a;
	};

	class Rect
	{
		public:
			Rect() {}
			Rect(int input_x, int input_y, int input_w, int input_h) : x(input_x), y(input_y), w(input_w), h(input_h) {}
			int x;
			int y;
			int w;
			int h;
	};

	void Rect2DtoGLVectrices(Rect dest_rect, Rect source_rect, Rect texture_size, float *vectrices, bool modify_dest_rect = true, bool modify_source_rect = true, float scale = 1);
	bool UTF8toUnicode(const std::string &utf8, std::vector<unsigned int> &unicode);
	Point2Di Point3DftoPoint2Di(Point3Df input);
	bool CompareString(std::string str1, std::string str2);
	inline bool CompareTitle(const SongInformation *arg1, const SongInformation *arg2);
	inline bool CompareArtist(const SongInformation *arg1, const SongInformation *arg2);
	inline bool CompareNoter(const SongInformation *arg1, const SongInformation *arg2);
	inline bool CompareDifficulty(const SongInformation *arg1, const SongInformation *arg2);
	inline bool CompareDuration(const SongInformation *arg1, const SongInformation *arg2);
	inline bool CompareTitleReverse(const SongInformation *arg1, const SongInformation *arg2);
	inline bool CompareArtistReverse(const SongInformation *arg1, const SongInformation *arg2);
	inline bool CompareNoterReverse(const SongInformation *arg1, const SongInformation *arg2);
	inline bool CompareDifficultyReverse(const SongInformation *arg1, const SongInformation *arg2);
	inline bool CompareDurationReverse(const SongInformation *arg1, const SongInformation *arg2);
	inline bool CompareNote(const Note *arg1, comst Note *arg2);
};

#endif