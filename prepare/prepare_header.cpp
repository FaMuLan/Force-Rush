#include "prepare_header.h"
#include "../texture_manager.h"
#include "../button.h"
#include "../system.h"
#include "../song_data.h"

fr::PrepareHeader *fr::PrepareHeader::m_instance = 0;

void fr::PrepareHeader::init()
{
	information_base = new Button;
	score_base = new Button;
	artist_text = new TextArea;
	noter_text = new TextArea;
	version_text = new TextArea;
	duration_text = new TextArea;
	score_text = new TextArea;
	rank_text = new TextArea;
	pure_text = new TextArea;
	great_text = new TextArea;
	good_text = new TextArea;
	error_text = new TextArea;

	information_base->init("assets/prepare/song_header_base.png", 0, 0);
	information_base->AddPressedFrame("assets/prepare/song_header_base_pressed.png");
	score_base->init("assets/prepare/song_header_base.png", 0, 0);
	score_base->AddPressedFrame("assets/prepare/song_header_base_pressed.png");
	if (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT)
	{
		information_base->SetPos(160, 0);
		score_base->SetPos(440, 0);
	}
	else
	{
		information_base->SetPos(0, 0);
		score_base->SetPos(System::instance()->GetWindowWidth() - 280, 0);
	}
	TextureManager::instance()->loadfont("assets/fonts/Ubuntu-R.ttf", 24);	TextureManager::instance()->loadfont("assets/fonts/Audiowide.ttf", 156);
	artist_text->init("NULL", 32, 32, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 216);
	noter_text->init("NULL", 32, 92, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 216);
	version_text->init("NULL", 32, 152, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 216);
	duration_text->init("NULL", 32, 212, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 216);
	score_text->init("NULL", 32, 212, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 216);
	rank_text->init("?", 138, 126, "assets/fonts/Audiowide.ttf", 156, 0x00, 0x00, 0x00);
	pure_text->init("PURE      ??", 32, 32, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 216);
	great_text->init("GREAT     ??", 32, 92, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 216);
	good_text->init("GOOD      ??", 32, 152, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 216);
	error_text->init("ERROR     ??", 32, 212, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 216);
	information_base->AddText(artist_text);
	information_base->AddText(noter_text);
	information_base->AddText(version_text);
	information_base->AddText(duration_text);
	score_base->AddText(score_text);
	score_base->AddText(rank_text);
	is_score_detail_shown = false;
}

void fr::PrepareHeader::clear()
{
	
}

void fr::PrepareHeader::update()
{
	if (System::instance()->IsWindowModified())
	{
		if (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT)
		{
			information_base->SetPos(160, 0);
			score_base->SetPos(440, 0);
		}
		else
		{
			information_base->SetPos(0, 0);
			score_base->SetPos(System::instance()->GetWindowWidth() - 280, 0);
		}
	}

	information_base->update();
	score_base->update();

	if (score_base->IsReleased())
	{
		is_score_detail_shown = !is_score_detail_shown;
		score_base->ClearText();
		if (is_score_detail_shown)
		{
			score_base->AddText(pure_text);
			score_base->AddText(great_text);
			score_base->AddText(good_text);
			score_base->AddText(error_text);
		}
		else
		{
			score_base->AddText(score_text);
			score_base->AddText(rank_text);
		}
	}
}

void fr::PrepareHeader::render()
{
	information_base->render();
	score_base->render();
}	//void fr::PrepareHeader::render()

void fr::PrepareHeader::SetInformation(SongInformation *information)
{
	m_information = information;
	char *duration_ch = new char[10];
	char *score_ch = new char[15];
	char *pure_ch = new char[15];
	char *great_ch = new char[15];
	char *good_ch = new char[15];
	char *error_ch = new char[15];
	std::string rank_ch;
	sprintf(duration_ch, "%ds", m_information->duration / 1000);
	sprintf(score_ch, "score %d", m_information->high_score->score);
	sprintf(pure_ch, "PURE     %d", m_information->high_score->pure);
	sprintf(great_ch, "GREAT    %d", m_information->high_score->great);
	sprintf(good_ch, "GOOD     %d", m_information->high_score->good);
	sprintf(error_ch, "ERROR    %d", m_information->high_score->error);
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
			rank_ch = "A";
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
	pure_text->SetText(pure_ch);
	great_text->SetText(great_ch);
	good_text->SetText(good_ch);
	error_text->SetText(error_ch);
	delete [] duration_ch;
	delete [] score_ch;
	delete [] pure_ch;
	delete [] great_ch;
	delete [] good_ch;
	delete [] error_ch;
}