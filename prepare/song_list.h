#ifndef FORCE_RUSH_PREPARE_SONG_LIST_H
#define FORCE_RUSH_PREPARE_SONG_LIST_H

#include <vector>
#include <string>

namespace fr
{
	class Sprite;
	class Button;
	class TextInputBox;
	struct SongInformation;
	struct Score;

	enum SortType
	{
		SORTTYPE_DEFAULT = 0,
		SORTTYPE_ARTIST,
		SORTTYPE_NOTER,
		SORTTYPE_TITLE,
		SORTTYPE_DIFFICULTY,
		SORTTYPE_DURATION
	};

	class SongList
	{
		public:
			static SongList *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new SongList;
					return m_instance;
				}
				return m_instance;
			}
			static void init();
			static void clear();
			static void update();
			static void render();

			static bool LoadList();
			static void WriteList();
			static void RefreshList();
			static bool IsRefreshing();
			static void RefreshListSize();

			static void OnEnter();
			static void OnExit();
			static void SwitchSort(SortType type, bool reverse);
			static void RollList();
			static void Search(std::string pattern_str);
			static void SwitchShown();
			static bool IsShown();
			static bool IsReverse();
			static SortType GetSortType();
		private:
			SongList() {}
			~SongList() {}
			static SongList *m_instance;

			static std::vector<Button*> m_cell;
			static TextInputBox *search_bar;
			static std::vector<SongInformation*> m_information;
			static std::vector<SongInformation*> shown_information;
			static SongInformation *null_information;
			static int list_length;
			static int list_process;
			static int selected_index;
			static int cell_heigh;
			static int cell_pos_offset_y;
			static bool is_list_moved;
			static bool last_list_moved;
			static bool is_refreshing;
			static bool is_loaded;
			static bool is_reverse;
			static SortType sort_type;

			static bool is_shown;
			static bool is_entered;
			static bool is_exited;
	};
};	//namespace fr

#endif	//FORCE_RUSH_PREPARE_SONG_LIST_H