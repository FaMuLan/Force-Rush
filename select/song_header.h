#ifndef LUNATIC_MELODY_SELECT_SONG_HEADER_H
#define LUNATIC_MELODY_SELECT_SONG_HEADER_H

#include "song_list.h"

namespace lm
{
	class SongHeader
	{
		public:
			static SongHeader *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new SongHeader;
					return m_instance;
				}
				return m_instance;
			}
			void init();
			void clear();
			void render();
			void SetInformation(SongInformation *information);
		private:
			SongHeader() {}
			~SongHeader() {}
			static SongHeader *m_instance;
			Sprite *header_base;
			SongInformation *m_information;
	};
};

#endif