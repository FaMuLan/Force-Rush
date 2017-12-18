#include "song_header.h"
#include "../texture_manager.h"
#include "../sprite.h"
#include "../system.h"
#include "../song_data.h"

fr::SongHeader *fr::SongHeader::m_instance = 0;

void fr::SongHeader::init()
{
	header_base = new Sprite;
	header_base->init("assets/select/song_header_base.png", 0, 0);
	TextureManager::instance()->loadfont("assets/fonts/Ubuntu-R.ttf", 24);	TextureManager::instance()->loadfont("assets/fonts/Audiowide.ttf", 156);
}

void fr::SongHeader::clear()
{
	
}

void fr::SongHeader::render()
{
	char *difficulty_ch = new char[3];
	char *duration_ch = new char[4];
	char *score_ch = new char[12];
	std::string rank_ch;
	sprintf(difficulty_ch, "Lv.%d", m_information->difficulty);
	sprintf(duration_ch, "%ds", m_information->duration / 1000);
	sprintf(score_ch, "score %d", m_information->high_score->score);
	switch (m_information->high_score->rank)
	{
		case 0:
			rank_ch = "?";
		break;
		case 1:
			rank_ch = "D";
		break;
		case 2:
			rank_ch = "C";
		break;
		case 3:
			rank_ch = "B";
		break;
		case 4:
			rank_ch = "W";
		break;
		case 5:
			rank_ch = "S";
		break;
	}

	if (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT)
	{
		header_base->SetPos(160, 0);
		header_base->render();
		header_base->SetPos(440, 0);
		header_base->render();
		TextureManager::instance()->render(m_information->artist, 192, 32, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 216);
		TextureManager::instance()->render(m_information->noter, 192, 92, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 216);
		TextureManager::instance()->render(m_information->version, 192, 152, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 216);
		TextureManager::instance()->render(duration_ch, 192, 212, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 216);
		TextureManager::instance()->render(score_ch, 472, 212, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 216);
		TextureManager::instance()->render(rank_ch, 578, 126, "assets/fonts/Audiowide.ttf", 156, 0x00, 0x00, 0x00);
	}
	else
	{
		header_base->SetPos(0, 0);
		header_base->render();
		header_base->SetPos(System::instance()->GetWindowWidth() - 280, 0);
		header_base->render();
		TextureManager::instance()->render(m_information->artist, 32, 32, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 216);
		TextureManager::instance()->render(m_information->noter, 32, 92, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 216);
		TextureManager::instance()->render(m_information->version, 32, 152, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 216);
		TextureManager::instance()->render(duration_ch, 32, 212, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 216);
		TextureManager::instance()->render(score_ch, System::instance()->GetWindowWidth() - 248, 212, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 216);
		TextureManager::instance()->render(rank_ch, System::instance()->GetWindowWidth() - 122, 126, "assets/fonts/Audiowide.ttf", 156, 0x00, 0x00, 0x00);
	}
}	//void fr::SongHeader::render()

void fr::SongHeader::SetInformation(SongInformation *information)
{
	m_information = information;
}