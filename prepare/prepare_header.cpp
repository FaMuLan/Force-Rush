#include "prepare_header.h"
#include "../texture_manager.h"
#include "../gui/button.h"
#include "../system.h"
#include "../data.h"

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
	safe_text = new TextArea;
	warning_text = new TextArea;
	error_text = new TextArea;
	artist_label = new TextArea;
	noter_label = new TextArea;
	version_label = new TextArea;
	duration_label = new TextArea;

	information_base->init("assets/prepare/song_header_base.png");
	information_base->AddPressedFrame("assets/prepare/song_header_base_pressed.png");
	score_base->init("assets/prepare/song_header_base.png");
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

	artist_text->init("NULL", Point2Di(32, 32), "assets/fonts/Ubuntu-R.ttf", 24, Color(0x00, 0x00, 0x00), TEXTFORMAT_LEFT, 216);
	noter_text->init("NULL", Point2Di(32, 92), "assets/fonts/Ubuntu-R.ttf", 24, Color(0x00, 0x00, 0x00), TEXTFORMAT_LEFT, 216);
	version_text->init("NULL", Point2Di(32, 152), "assets/fonts/Ubuntu-R.ttf", 24, Color(0x00, 0x00, 0x00), TEXTFORMAT_LEFT, 216);
	duration_text->init("NULL", Point2Di(32, 212), "assets/fonts/Ubuntu-R.ttf", 24, Color(0x00, 0x00, 0x00), TEXTFORMAT_LEFT, 216);
	score_text->init("NULL", Point2Di(32, 212), "assets/fonts/Ubuntu-R.ttf", 24, Color(0x00, 0x00, 0x00), TEXTFORMAT_LEFT, 216);
	rank_text->init("?", Point2Di(138, 126), "assets/fonts/Audiowide.ttf", 156, Color(0x00, 0x00, 0x00));
	pure_text->init("PURE      ??", Point2Di(32, 32), "assets/fonts/Ubuntu-R.ttf", 24, Color(0x00, 0x00, 0x00), TEXTFORMAT_LEFT, 216);
	safe_text->init("GREAT     ??", Point2Di(32, 92), "assets/fonts/Ubuntu-R.ttf", 24, Color(0x00, 0x00, 0x00), TEXTFORMAT_LEFT, 216);
	warning_text->init("GOOD      ??", Point2Di(32, 152), "assets/fonts/Ubuntu-R.ttf", 24, Color(0x00, 0x00, 0x00), TEXTFORMAT_LEFT, 216);
	error_text->init("ERROR     ??", Point2Di(32, 212), "assets/fonts/Ubuntu-R.ttf", 24, Color(0x00, 0x00, 0x00), TEXTFORMAT_LEFT, 216);
	artist_label->init("Artist", Point2Di(32, 16), "assets/fonts/Audiowide.ttf", 16, Color(0x00, 0x00, 0x00), TEXTFORMAT_LEFT);
	noter_label->init("Noter", Point2Di(32, 76), "assets/fonts/Audiowide.ttf", 16, Color(0x00, 0x00, 0x00), TEXTFORMAT_LEFT);
	version_label->init("Version", Point2Di(32, 136), "assets/fonts/Audiowide.ttf", 16, Color(0x00, 0x00, 0x00), TEXTFORMAT_LEFT);
	duration_label->init("Length", Point2Di(32, 196), "assets/fonts/Audiowide.ttf", 16, Color(0x00, 0x00, 0x00), TEXTFORMAT_LEFT);
	information_base->AddText(artist_text);
	information_base->AddText(noter_text);
	information_base->AddText(version_text);
	information_base->AddText(duration_text);
	information_base->AddText(artist_label);
	information_base->AddText(noter_label);
	information_base->AddText(version_label);
	information_base->AddText(duration_label);
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
			score_base->AddText(safe_text);
			score_base->AddText(warning_text);
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
	char *safe_ch = new char[15];
	char *warning_ch = new char[15];
	char *error_ch = new char[15];
	std::string rank_ch;
	sprintf(duration_ch, "%ds", m_information->duration / 1000);
	sprintf(score_ch, "score %d", m_information->high_score->score);
	sprintf(pure_ch, "PURE     %d", m_information->high_score->pure);
	sprintf(safe_ch, "SAFE     %d", m_information->high_score->safe);
	sprintf(warning_ch, "WARNING  %d", m_information->high_score->warning);
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
	safe_text->SetText(safe_ch);
	warning_text->SetText(warning_ch);
	error_text->SetText(error_ch);
	delete [] duration_ch;
	delete [] score_ch;
	delete [] pure_ch;
	delete [] safe_ch;
	delete [] warning_ch;
	delete [] error_ch;
}