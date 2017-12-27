#include "song_header.h"
#include "../texture_manager.h"
#include "../sprite.h"
#include "../system.h"
#include "../song_data.h"

fr::SongHeader *fr::SongHeader::m_instance = 0;

void fr::SongHeader::init()
{
	header_base = new Sprite;
	artist_text = new TextArea;
	noter_text = new TextArea;
	version_text = new TextArea;
	duration_text = new TextArea;
	score_text = new TextArea;
	rank_text = new TextArea;
	header_base->init("assets/select/song_header_base.png", 0, 0);
	TextureManager::instance()->loadfont("assets/fonts/Ubuntu-R.ttf", 24);	TextureManager::instance()->loadfont("assets/fonts/Audiowide.ttf", 156);
	artist_text->init("NULL", 192, 32, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 216);
	noter_text->init("NULL", 192, 92, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 216);
	version_text->init("NULL", 192, 152, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 216);
	duration_text->init("NULL", 192, 212, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 216);
	score_text->init("NULL", 472, 212, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 216);
	rank_text->init("?", 578, 126, "assets/fonts/Audiowide.ttf", 156, 0x00, 0x00, 0x00);
}

void fr::SongHeader::clear()
{
	
}

void fr::SongHeader::render()
{
	if (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT)
	{
		header_base->SetPos(160, 0);
		header_base->render();
		header_base->SetPos(440, 0);
		header_base->render();
		artist_text->SetPos(192, 32);
		noter_text->SetPos(192, 92);
		version_text->SetPos(192, 152);
		duration_text->SetPos(192, 212);
		score_text->SetPos(472, 212);
		rank_text->SetPos(578, 126);
		artist_text->render();
		noter_text->render();
		version_text->render();
		duration_text->render();
		score_text->render();
		rank_text->render();
	}
	else
	{
		header_base->SetPos(0, 0);
		header_base->render();
		header_base->SetPos(System::instance()->GetWindowWidth() - 280, 0);
		header_base->render();
		artist_text->SetPos(32, 32);
		noter_text->SetPos(32, 92);
		version_text->SetPos(32, 152);
		duration_text->SetPos(32, 212);
		score_text->SetPos(System::instance()->GetWindowWidth() - 248, 212);
		rank_text->SetPos(System::instance()->GetWindowWidth() - 122, 126);
		artist_text->render();
		noter_text->render();
		version_text->render();
		duration_text->render();
		score_text->render();
		rank_text->render();
	}
}	//void fr::SongHeader::render()

void fr::SongHeader::SetInformation(SongInformation *information)
{
	m_information = information;
	char *duration_ch = new char[10];
	char *score_ch = new char[15];
	std::string rank_ch;
	sprintf(duration_ch, "%ds", m_information->duration / 1000);
	sprintf(score_ch, "score %d", m_information->high_score->score);
	switch (m_information->high_score->rank)
	{
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
		default:
			rank_ch = "?";
		break;
	}
	artist_text->SetText(m_information->artist);
	noter_text->SetText(m_information->noter);
	version_text->SetText(m_information->version);
	duration_text->SetText(duration_ch);
	score_text->SetText(score_ch);
	rank_text->SetText(rank_ch);
	delete [] duration_ch;
	delete [] score_ch;
}