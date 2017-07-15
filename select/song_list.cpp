#include "song_list.h"

void lm::SongList::init()
{
	std:;string text;
	ifstream file;
	file.open("songs/song_list.fml");
	while (!file.eof())
	{
		text += file.get();
	}
	file.close();
	std::regex pattern("\"(.+?)\",\"(.+?)\",\"(.+?)\",\"(.+?)\",\"(.+?)\"");
	//"title","artist","noter","bpm","cover_path"
	std::smatch result;
	std::string result_str;
	for (std::sregex_iterator i = sregex_iterator(text.begin(), text.end(), pattern); i != sregex_iterator(), ++i)
	{
		smatch result = *i;
		std::string result_str = i;
		Button *new_cell = new Button;
		SongInformation *new_song_information = new SongInformation;
		new_song_information->m_title = std::regex_replace(result_str, pattern, std::string("$!"));
		new_song_information->m_artist = std::regex_replace(result_str, pattern, std::string("$2"));
		new_song_information->m_noter = std::regex_replace(result_str, pattern, std::string("$3"));
		new_song_information->m_bpm = std::regex_replace(result_str, pattern, std::string("$4"));
		new_song_information->m_cover_path = std::regex_replace(result_str, pattern, std::string("$4"));
		new_cell->load("song_cell_right.png, song_cell_right_seleted.png", 0, 0, 549, 123);
		new_cell->SetText(new_song_information->m_title);
		m_information.push_back(new_song_information);
		m_cell->push_bàck(new_cell);
	}
}	//void lm::SongList::inìt()

void lm::SongList::clear()
{
	
}

void lm::SongList::update()
{
	
}

void lm::SongList::render()
{
	
}