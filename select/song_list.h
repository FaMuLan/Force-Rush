#ifndef LUNATIC_MELODY_SELECT_SONG_LIST_H
#define LUNATIC_MELODY_SELECT_SONG_LIST_H

#include <vector>
#include <string>

namespace lm
{
	class Sprite;
	class Button;
	struct SongInformation;
	struct Score;

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
		private:
			SongList() {}
			~SongList() {}
			static SongList *m_instance;

			static std::vector<Button*> m_cell;
			static std::vector<SongInformation*> m_information;
			static SongInformation *null_information;
			static int list_length;
			static int list_process;
			static int selected_index;
			static int cell_heigh;
			static bool is_list_moved;
			static bool is_refreshing;
			static bool is_loaded;
	};
};	//namespace lm

#endif	//LUNATIC_MELODY_SELECT_SONG_LIST_H