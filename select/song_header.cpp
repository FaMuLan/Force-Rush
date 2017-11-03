#include "song_header.h"
#include "../texture_manager.h"
#include "../sprite.h"
#include "../system.h"

lm::SongHeader *lm::SongHeader::m_instance = 0;

void lm::SongHeader::init()
{
	header_base_portrait = new Sprite;
	header_base_landscape = new Sprite;
	header_base_portrait->init("assets/select_header_portrait.png", 0, 0);
	header_base_landscape->init("assets/select_header_landscape.png", 0, 0);
	TextureManager::instance()->loadfont("assets/Ubuntu-R.ttf", 32);
	TextureManager::instance()->loadfont("assets/Ubuntu-R.ttf", 24);
}

void lm::SongHeader::clear()
{
	
}

void lm::SongHeader::render()
{
	if (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT)
	{
		header_base_portrait->render();
		TextureManager::instance()->render(m_information->title, 32, 40, "assets/Ubuntu-R.ttf", 32, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
		TextureManager::instance()->render(m_information->artist, 32, 112, "assets/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
		TextureManager::instance()->render(m_information->noter, 32, 176, "assets/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
		TextureManager::instance()->render(m_information->bpm, 32, 240, "assets/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
	}
	else
	{
		header_base_landscape->render();
		TextureManager::instance()->render(m_information->title, 32, 40, "assets/Ubuntu-R.ttf", 32, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
		TextureManager::instance()->render(m_information->artist, 32, 112, "assets/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
		TextureManager::instance()->render(m_information->noter, 32, 176, "assets/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
		TextureManager::instance()->render(m_information->bpm, 32, 240, "assets/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
	}
}	//void lm::SongHeader::render()

void lm::SongHeader::SetInformation(SongInformation *information)
{
	m_information = information;
}