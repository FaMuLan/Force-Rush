#ifndef FORCE_RUSH_PREPARE_SONG_LIST_H
#define FORCE_RUSH_PREPARE_SONG_LIST_H

#include <vector>
#include <string>
#include "../gui/list.h"

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

	class SongList : public List
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
			void init();
			void clear();
			void update();
			void render();
			void PushText();

			bool LoadList();
			static void WriteList();
			static void RefreshList();
			bool IsRefreshing();
			void RefreshListSize();

			static void SwitchSort(SortType type, bool reverse);
			void RollList();
			void Search(std::string pattern_str);
			bool IsReverse();
			SortType GetSortType();
		private:
			SongList() {}
			~SongList() {}
			static SongList *m_instance;

			TextInputBox *search_bar;
			static std::vector<SongInformation*> m_information;
			static std::vector<SongInformation*> shown_information;
			static SongInformation *null_information;
			static bool is_refreshing;
			static bool is_loaded;
			static bool is_reverse;
			static SortType sort_type;
	};
};	//namespace fr

#endif	//FORCE_RUSH_PREPARE_SONG_LIST_H