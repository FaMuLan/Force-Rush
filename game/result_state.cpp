#include "result_state.h"
#include "beatmap.h"
#include "../gui/button.h"
#include "../sprite.h"
#include "../gui/text_area.h"
#include "../system.h"
#include "../texture_manager.h"
#include "../song_data.h"
#include "../prepare/prepare_state.h"
#include "../control_handler.h"
#include "../prepare/song_list.h"
#include "../loading/loading_state.h"
#include "game_state.h"
#include "../user/user_profile.h"
#include "../user/setting.h"
#include "../background.h"

fr::ResultState *fr::ResultState::m_instance = 0;

void fr::ResultState::init()
{
	song_base = new Sprite;
	score_base = new Sprite;
	rank_base = new Sprite;
	user_base = new Sprite;
	b_return = new Button;
	b_retry = new Button;

	title_text = new TextArea;
	artist_text = new TextArea;
	version_text = new TextArea;
	difficulty_text = new TextArea;
	score_text = new TextArea;
	pure_text = new TextArea;
	safe_text = new TextArea;
	warning_text = new TextArea;
	error_text = new TextArea;
	chain_text = new TextArea;
	rank_text = new TextArea;
	user_name_text = new TextArea;
	performance_point_text = new TextArea;
	new_record_text = new TextArea;
	new_performance_text = new TextArea;

	song_base->init("assets/result/song_base.png", Rect(System::instance()->GetWindowWidth() / 2 - 360, System::instance()->GetWindowHeigh() / 2 - 244, 0, 0));
	score_base->init("assets/result/score_base.png", Rect(System::instance()->GetWindowWidth() / 2 + 128, System::instance()->GetWindowHeigh() / 2 - 244, 0, 0));
	rank_base->init("assets/result/rank_base.png", Rect(System::instance()->GetWindowWidth() / 2 - 104, System::instance()->GetWindowHeigh() / 2 - 68, 0, 0));
	user_base->init("assets/result/user_base.png", Rect(System::instance()->GetWindowWidth() / 2 - 360, System::instance()->GetWindowHeigh() / 2 - 68, 0, 0));
	b_return->init("assets/base/sort_button.png", Rect(0, System::instance()->GetWindowHeigh() - 80, 176, 80));
	b_return->AddPressedFrame( "assets/base/sort_button_pressed.png");
	b_return->AddText("Return", b_return->GetW() / 2, b_return->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
	b_retry->init("assets/base/sort_button.png", Rect(System::instance()->GetWindowWidth() - 176, System::instance()->GetWindowHeigh() - 80, 176, 80));
	b_retry->AddPressedFrame("assets/base/sort_button_pressed.png");
	b_retry->AddText("Retry", b_retry->GetW() / 2, b_retry->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
	is_new_record = false;
	is_new_performance = false;

	double acc = double(m_score->score) / double(m_information->full_score);
	if (acc >= 0.9)
	{
		m_score->rank = RANK_S;
	}
	else if (acc >= 0.8)
	{
		m_score->rank = RANK_A;
	}
	else if (acc >= 0.7)
	{
		m_score->rank = RANK_B;
	}
	else if (acc >= 0.6)
	{
		m_score->rank = RANK_C;
	}
	else
	{
		m_score->rank = RANK_D;
	}

	if (m_score->score > m_information->high_score->score && !Setting::instance()->IsAuto() && !Setting::instance()->IsSlideOut())
	{
		m_information->high_score = m_score;
		SongList::instance()->WriteList();
		is_new_record = true;
	}

	int previous_performance = UserProfile::instance()->GetPerformancePoint();
	int current_performance = UserProfile::instance()->CalculatePerformancePoint(m_information->difficulty, acc, !Setting::instance()->IsAuto() && !Setting::instance()->IsSlideOut());
	if (previous_performance < current_performance)
	{
		is_new_performance = true;
	}

	char *difficulty_ch = new char[10];
	char *pure_ch = new char[10];
	char *safe_ch = new char[10];
	char *warning_ch = new char[10];
	char *error_ch = new char[10];
	char *score_ch = new char[10];
	char *performance_ch = new char[20];
	std::string rank_ch;

	sprintf(difficulty_ch, "Lv.%d", m_information->difficulty);
	sprintf(pure_ch, "PURE  %d", m_score->pure);
	sprintf(safe_ch, "SAFE  %d", m_score->safe);
	sprintf(warning_ch, "WARNING  %d", m_score->warning);
	sprintf(error_ch, "ERROR %d", m_score->error);
	sprintf(score_ch, "SCORE  %d", m_score->score);
	sprintf(performance_ch, "performance:%d", current_performance);

	switch (m_score->rank)
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
			rank_ch = "A";
		break;
		case 5:
			rank_ch = "S";
		break;
	}

	title_text->init(m_information->title, song_base->GetX() + 32, song_base->GetY() + 24, "assets/fonts/Ubuntu-R.ttf", 32, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 416);
	artist_text->init(m_information->artist, song_base->GetX() + 32, song_base->GetY() + 72, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 416);
	version_text->init(m_information->version, song_base->GetX() + 96, song_base->GetY() + 112, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 416);
	difficulty_text->init(difficulty_ch, song_base->GetX() + 32, song_base->GetY() + 112, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 56);
	score_text->init(score_ch, score_base->GetX() + 32, score_base->GetY() + 24, "assets/fonts/Ubuntu-R.ttf", 32, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
	pure_text->init(pure_ch, score_base->GetX() + 32, score_base->GetY() + 80, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
	safe_text->init(safe_ch, score_base->GetX() + 32, score_base->GetY() + 128, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
	warning_text->init(warning_ch, score_base->GetX() + 32, score_base->GetY() + 176, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
	error_text->init(error_ch, score_base->GetX() + 32, score_base->GetY() + 224, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
	rank_text->init(rank_ch, rank_base->GetX() + rank_base->GetW() / 2, rank_base->GetY() + rank_base->GetH() / 2, "assets/fonts/Audiowide.ttf", 168, 0x00, 0x00, 0x00);
	user_name_text->init(UserProfile::instance()->GetUserName(), user_base->GetX() + 32, user_base->GetY() + 32, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);
	performance_point_text->init(performance_ch, user_base->GetX() + 32, user_base->GetY() + 80, "assets/fonts/Ubuntu-R.ttf", 18, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);
	new_record_text->init("Score Replaced", score_base->GetX() + 32, score_base->GetY() + 272, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);
	new_performance_text->init("Performance Break", user_base->GetX() + 32, user_base->GetY() + 128, "assets/fonts/Ubuntu-R.ttf", 18, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);

	delete [] difficulty_ch;
	delete [] pure_ch;
	delete [] safe_ch;
	delete [] warning_ch;
	delete [] error_ch;
	delete [] score_ch;
	delete [] performance_ch;
}

void fr::ResultState::clear()
{
	song_base->clear();
	score_base->clear();
	rank_base->clear();
	user_base->clear();
	b_return->clear();
	b_retry->clear();
	title_text->clear();
	artist_text->clear();
	version_text->clear();
	difficulty_text->clear();
	score_text->clear();
	pure_text->clear();
	safe_text->clear();
	warning_text->clear();
	error_text->clear();
	chain_text->clear();
	acc_text->clear();
	rank_text->clear();
	user_name_text->clear();
	performance_point_text->clear();
	new_record_text->clear();
	new_performance_text->clear();
	delete song_base;
	delete score_base;
	delete rank_base;
	delete user_base;
	delete b_return;
	delete b_retry;
	delete title_text;
	delete artist_text;
	delete version_text;
	delete difficulty_text;
	delete score_text;
	delete pure_text;
	delete safe_text;
	delete warning_text;
	delete error_text;
	delete chain_text;
	delete acc_text;
	delete rank_text;
	delete user_name_text;
	delete performance_point_text;
	delete new_record_text;
	delete new_performance_text;
}

void fr::ResultState::update()
{
	b_return->update();
	b_retry->update();

	if (System::instance()->IsWindowModified())
	{
		song_base->SetPos(System::instance()->GetWindowWidth() / 2 - 360, System::instance()->GetWindowHeigh() / 2 - 244);
		score_base->SetPos(System::instance()->GetWindowWidth() / 2 + 128, System::instance()->GetWindowHeigh() / 2 - 244);
		rank_base->SetPos(System::instance()->GetWindowWidth() / 2 - 104, System::instance()->GetWindowHeigh() / 2 - 68);
		user_base->SetPos(System::instance()->GetWindowWidth() / 2 - 360, System::instance()->GetWindowHeigh() / 2 - 68);
		b_return->SetPos(0, System::instance()->GetWindowHeigh() - 80);
		b_retry->SetPos(System::instance()->GetWindowWidth() - 176, System::instance()->GetWindowHeigh() - 80);

		title_text->SetPos(song_base->GetX() + 32, song_base->GetY() + 24);
		artist_text->SetPos(song_base->GetX() + 32, song_base->GetY() + 72);
		version_text->SetPos(song_base->GetX() + 96, song_base->GetY() + 112);
		difficulty_text->SetPos(song_base->GetX() + 32, song_base->GetY() + 112);
		score_text->SetPos(score_base->GetX() + 32, score_base->GetY() + 24);
		pure_text->SetPos(score_base->GetX() + 32, score_base->GetY() + 80);
		safe_text->SetPos(score_base->GetX() + 32, score_base->GetY() + 128);
		warning_text->SetPos(score_base->GetX() + 32, score_base->GetY() + 176);
		error_text->SetPos(score_base->GetX() + 32, score_base->GetY() + 224);
		rank_text->SetPos(rank_base->GetX() + rank_base->GetW() / 2, rank_base->GetY() + rank_base->GetH() / 2);
		user_name_text->SetPos(user_base->GetX() + 32, user_base->GetY() + 32);
		performance_point_text->SetPos(user_base->GetX() + 32, user_base->GetY() + 80);
		new_record_text->SetPos(score_base->GetX() + 32, score_base->GetY() + 272);
		new_performance_text->SetPos(user_base->GetX() + 32, user_base->GetY() + 128);
	}

	Background::instance()->render();
	song_base->render();
	score_base->render();
	rank_base->render();
	user_base->render();
	b_return->render();
	b_retry->render();

	title_text->render();
	artist_text->render();
	version_text->render();
	difficulty_text->render();
	score_text->render();
	pure_text->render();
	safe_text->render();
	warning_text->render();
	error_text->render();
	rank_text->render();
	user_name_text->render();
	performance_point_text->render();
	if (is_new_record)
	{
		new_record_text->render();
	}
	if (is_new_performance)
	{
		new_performance_text->render();
	}

	if (b_return->IsReleased() || ControlHandler::instance()->IsKeyDown(SDL_SCANCODE_AC_BACK))
	{
		LoadingState::instance()->init(STATE_PREPARE);
	}
	if (b_retry->IsReleased())
	{
		LoadingState::instance()->init(STATE_GAME);
		GameState::instance()->SetFile(m_information);
	}
}

void fr::ResultState::LoadScore(fr::SongInformation *load_information, fr::Score *load_score)
{
	m_information = load_information;
	m_score = load_score;
}