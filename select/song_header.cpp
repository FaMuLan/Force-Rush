#include "song_header.h"
#include "../texture_manager.h"
#include "../sprite.h"
#include "../system.h"

lm::SongHeader *lm::SongHeader::m_instance = 0;

void lm::SongHeader::init()
{
	header_base = new Sprite;
	header_base->init("assets/select/song_header_base.png", 0, 0);
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
	sprintf(duration_ch, "%ds", m_information->duration / 1000);

	if (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT)
	{
		header_base->SetPos(160, 0);
		header_base->render();
		header_base->SetPos(440, 0);
		header_base->render();
		TextureManager::instance()->render(m_information->artist, 192, 32, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
		TextureManager::instance()->render(m_information->noter, 192, 92, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
		TextureManager::instance()->render(m_information->version, 192, 152, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
		TextureManager::instance()->render(duration_ch, 192, 212, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
	}
	else
	{
		header_base->SetPos(0, 0);
		header_base->render();
		header_base->SetPos(System::instance()->GetWindowWidth() - 280, 0);
		header_base->render();
		TextureManager::instance()->render(m_information->artist, 32, 32, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 232);
		TextureManager::instance()->render(m_information->noter, 32, 92, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 232);
		TextureManager::instance()->render(m_information->version, 32, 152, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 232);
		TextureManager::instance()->render(duration_ch, 32, 212, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 232);
	}
}	//void lm::SongHeader::render()

void lm::SongHeader::SetInformation(SongInformation *information)
{
	m_information = information;
}