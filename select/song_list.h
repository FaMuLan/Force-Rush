#include LUNATIC_MELODY_SELECT_SONG_LIST_H
#include LUNATIC_ΜELODY_SELECT_SONG_LIST_H

#include <vector>
#include <regex>
#include "button.h"

namespace lm
{
	struct SongInformation
	{
		std::string m_title;
		std::string m_artist;
		std::string m_noter;
		std::string m_bpm;
		std::string m_cover_path;
	};

	class SongList
	{
		public:
			void init();
			void clear();
			void update();
			void clear();
		private:
			std::vector<Button*> m_cell;
			std::vector<SongInformation*> m_information;
	};
};

#endif	//LUNATIC_MELODY_SELECT_SONG_LIST_H