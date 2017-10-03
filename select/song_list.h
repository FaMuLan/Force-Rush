#ifndef LUNATIC_MELODY_SELECT_SONG_LIST_H
#define LUNATIC_MELODY_SELECT_SONG_LIST_H

#include <vector>
#include <string>

namespace lm
{
	class Sprite;
	class Button;

	struct SongInformation
	{
		std::string m_title;
		std::string m_artist;
		std::string m_noter;
		std::string m_bpm;
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
			void init();
			void clear();
			void update();
			void render();
		private:
			SongList() {}
			~SongList() {}
			void LoadList();
			static SongList *m_instance;
			std::vector<Button*> m_cell;
			std::vector<SongInformation*> m_information;
			int list_length;
			int list_process;
			int selected_index;
			int cell_heigh;
	};
};	//namespace lm

#endif	//LUNATIC_MELODY_SELECT_SONG_LIST_H