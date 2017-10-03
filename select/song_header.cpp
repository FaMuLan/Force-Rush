#include "song_header.h"
#include "../texture_manager.h"
#include "../sprite.h"

lm::SongHeader *lm::SongHeader::m_instance = 0;

void lm::SongHeader::init()
{
	header_base = new Sprite;
	header_base->load("assets/select_header_portrait.png", 0, 0, 720, 300);
	TextureManager::instance()->loadfont("assets/Ubuntu-R.ttf", 32);
	TextureManager::instance()->loadfont("assets/Ubuntu-R.ttf", 24);
}

void lm::SongHeader::clear()
{
	
}

void lm::SongHeader::render()
{
	header_base->render();
		TextureManager::instance()->render(m_information->m_title, 32, 40, "assets/Ubuntu-R.ttf", 32, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
	TextureManager::instance()->render(m_information->m_artist, 32, 112, "assets/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
	TextureManager::instance()->render(m_information->m_noter, 32, 176, "assets/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
	TextureManager::instance()->render(m_information->m_bpm, 32, 240, "assets/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
}

void lm::SongHeader::SetInformation(SongInformation *information)
{
	m_information = information;
}