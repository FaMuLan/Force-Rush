#ifndef FORCE_RUSH_SELECT_SONG_HEADER_H
#define FORCE_RUSH_SELECT_SONG_HEADER_H

#include "song_list.h"

namespace fr
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
	};	//class SongHeader
};	//namespace fr

#endif	//FORCE_RUSH_SELECT_SONG_HEADER_H