#ifndef FORCE_RUSH_PREPARE_PREPARE_HEADER_H
#define FORCE_RUSH_PREPARE_PREPARE_HEADER_H

#include "song_list.h"

namespace fr
{
	class TextArea;
	class Button;
	class SongInformation;
	
	class PrepareHeader
	{
		public:
			static PrepareHeader *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new PrepareHeader;
					return m_instance;
				}
				return m_instance;
			}
			void init();
			void clear();
			void update();
			void render();
			void SetInformation(SongInformation *information);
		private:
			PrepareHeader() {}
			~PrepareHeader() {}
			static PrepareHeader *m_instance;
			Button *information_base;
			Button *score_base;
			SongInformation *m_information;
			TextArea *artist_text;
			TextArea *noter_text;
			TextArea *version_text;
			TextArea *duration_text;
			TextArea *score_text;
			TextArea *rank_text;
			TextArea *pure_text;
			TextArea *safe_text;
			TextArea *warning_text;
			TextArea *error_text;
			TextArea *artist_label;
			TextArea *noter_label;
			TextArea *version_label;
			TextArea *duration_label;
			bool is_score_detail_shown;
	};	//class PrepareHeader
};	//namespace fr

#endif	//FORCE_RUSH_PREPARE_PREPARE_HEADER_H