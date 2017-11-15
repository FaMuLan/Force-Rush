#include "song_header.h"
#include "../texture_manager.h"
#include "../sprite.h"
#include "../system.h"

lm::SongHeader *lm::SongHeader::m_instance = 0;

void lm::SongHeader::init()
{
	header_base_portrait = new Sprite;
	header_base_landscape = new Sprite;
	header_base_portrait->init("assets/select/header_portrait.png", 0, 0);
	header_base_landscape->init("assets/select/header_landscape.png", 0, 0);
	TextureManager::instance()->loadfont("assets/fonts/Ubuntu-R.ttf", 32);
	TextureManager::instance()->loadfont("assets/fonts/Ubuntu-R.ttf", 24);
}

void lm::SongHeader::clear()
{
	
}

void lm::SongHeader::render()
{
	char *difficulty_ch = new char[3];
	char *duration_ch = new char[4];
	sprintf(difficulty_ch, "Lv.%d", m_information->difficulty);
	sprintf(duration_ch, "%ds", m_information->duration);

	if (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT)
	{
		header_base_portrait->render();
		TextureManager::instance()->render(m_information->title, 32, 40, "assets/fonts/Ubuntu-R.ttf", 32, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 416);
		TextureManager::instance()->render(m_information->artist, 32, 112, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
		TextureManager::instance()->render(m_information->noter, 32, 176, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
		TextureManager::instance()->render(m_information->bpm, 32, 240, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
		TextureManager::instance()->render(m_information->version, 264, 112, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
		TextureManager::instance()->render(difficulty_ch, 264, 176, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
		TextureManager::instance()->render(duration_ch, 264, 240, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
	}
	else
	{
		header_base_landscape->render();
		TextureManager::instance()->render(m_information->title, 32, 40, "assets/fonts/Ubuntu-R.ttf", 32, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 464);
		TextureManager::instance()->render(m_information->artist, 32, 112, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 232);
		TextureManager::instance()->render(m_information->noter, 32, 176, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 232);
		TextureManager::instance()->render(m_information->bpm, 32, 240, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 232);
		TextureManager::instance()->render(m_information->version, 312, 112, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 232);
		TextureManager::instance()->render(difficulty_ch, 312, 176, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 232);
		TextureManager::instance()->render(duration_ch, 264, 240, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 232);
	}
}	//void lm::SongHeader::render()

void lm::SongHeader::SetInformation(SongInformation *information)
{
	m_information = information;
}