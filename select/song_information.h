#ifndef LUNATIC_MELODY_SELECT_SONG_INFORMATION_H
#define LUNATIC_MELODY_SELECT_SONG_INFORMATION_H

namespace lm
{
	class SongInforamation : public Button
	{
		public:
			void load();
			void clear();
			void update();
			void render();
		private:
			bool is_selected;
			std::string m_title;
			std::string m_artist;
			std::string m_noter;
			std::string m_bpm;
	};
};

#endif	//LUNATIC_MELODY_SELECT_SONG_INFORMATION_H