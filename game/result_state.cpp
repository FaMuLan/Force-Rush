#include "result_state.h"
#include "beatmap.h"
#include "../button.h"
#include "../sprite.h"
#include "../text_area.h"
#include "../system.h"
#include "../texture_manager.h"
#include "../song_data.h"
#include "../prepare/prepare_state.h"
#include "../prepare/song_list.h"
#include "../loading/loading_state.h"
#include "game_state.h"
#include "../user/user_profile.h"
#include "../user/setting.h"

fr::ResultState *fr::ResultState::m_instance = 0;

void fr::ResultState::init()
{
	song_base = new Sprite;
	score_base = new Sprite;
	rank_base = new Sprite;
	performance_base = new Sprite;
	user_base = new Sprite;
	b_return = new Button;
	b_retry = new Button;

	title_text = new TextArea;
	artist_text = new TextArea;
	version_text = new TextArea;
	difficulty_text = new TextArea;
	score_text = new TextArea;
	pure_text = new TextArea;
	great_text = new TextArea;
	good_text = new TextArea;
	error_text = new TextArea;
	rank_text = new TextArea;
	user_name_text = new TextArea;
	performance_point_text = new TextArea;
	new_record_text = new TextArea;
	new_performance_text = new TextArea;
	TextureManager::instance()->loadfont("assets/fonts/Ubuntu-R.ttf", 32);
	TextureManager::instance()->loadfont("assets/fonts/Ubuntu-R.ttf", 24);
	TextureManager::instance()->loadfont("assets/fonts/Ubuntu-R.ttf", 18);
	TextureManager::instance()->loadfont("assets/fonts/Audiowide.ttf", 168);
	TextureManager::instance()->loadfont("assets/fonts/Audiowide.ttf", 18);
	TextureManager::instance()->loadfont("assets/fonts/Audiowide.ttf", 24);
	song_base->init("assets/result/song_base.png", Rect(0, System::instance()->GetWindowHeigh() - 720, 0, 0));
	score_base->init("assets/result/score_base.png", Rect(0, System::instance()->GetWindowHeigh() - 544, 0, 0));
	rank_base->init("assets/result/rank_base.png", Rect(System::instance()->GetWindowWidth() - 232, System::instance()->GetWindowHeigh() - 720, 0, 0));
	performance_base->init("assets/result/performance_base.png", Rect(System::instance()->GetWindowWidth() - 232, System::instance()->GetWindowHeigh() - 488, 0, 0));
	user_base->init("assets/result/user_base.png", Rect(System::instance()->GetWindowWidth() - 232,  System::instance()->GetWindowHeigh() - 318, 0, 0));
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

	if (m_score->score > m_information->high_score->score && !Setting::instance()->IsAuto())
	{
		m_information->high_score = m_score;
		SongList::instance()->WriteList();
		is_new_record = true;
	}

	int previous_performance = UserProfile::instance()->GetPerformancePoint();
	int current_performance = UserProfile::instance()->CalculatePerformancePoint(m_information->difficulty, acc);
	if (previous_performance < current_performance)
	{
		is_new_performance = true;
	}

	char *difficulty_ch = new char[10];
	char *pure_ch = new char[10];
	char *great_ch = new char[10];
	char *good_ch = new char[10];
	char *error_ch = new char[10];
	char *score_ch = new char[10];
	char *performance_ch = new char[20];
	std::string rank_ch;

	sprintf(difficulty_ch, "Lv.%d", m_information->difficulty);
	sprintf(pure_ch, "PURE  %d", m_score->pure);
	sprintf(great_ch, "GREAT %d", m_score->great);
	sprintf(good_ch, "GOOD  %d", m_score->good);
	sprintf(error_ch, "ERROR %d", m_score->error);
	sprintf(score_ch, "SCORE  %d", m_score->score);
	sprintf(performance_ch, "Performance:%d", current_performance);

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

	title_text->init(m_information->title, 32, System::instance()->GetWindowHeigh() - 696, "assets/fonts/Ubuntu-R.ttf", 32, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 416);
	artist_text->init(m_information->artist, 32, System::instance()->GetWindowHeigh() - 648, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 416);
	version_text->init(m_information->version, 96, System::instance()->GetWindowHeigh() - 608, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 416);
	difficulty_text->init(difficulty_ch, 32, System::instance()->GetWindowHeigh() - 608, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 56);
	score_text->init(score_ch, 32, System::instance()->GetWindowHeigh() - 520, "assets/fonts/Ubuntu-R.ttf", 32, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
	pure_text->init(pure_ch, 32, System::instance()->GetWindowHeigh() - 472, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
	great_text->init(great_ch, 32, System::instance()->GetWindowHeigh() - 432, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
	good_text->init(good_ch, 32, System::instance()->GetWindowHeigh() - 392, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
	error_text->init(error_ch, 32, System::instance()->GetWindowHeigh() - 352, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
	rank_text->init(rank_ch, System::instance()->GetWindowWidth() - 116, System::instance()->GetWindowHeigh() - 604, "assets/fonts/Audiowide.ttf", 168, 0x00, 0x00, 0x00);
	user_name_text->init(UserProfile::instance()->GetUserName(), System::instance()->GetWindowWidth() - 200, System::instance()->GetWindowHeigh() - 280, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);
	performance_point_text->init(performance_ch, System::instance()->GetWindowWidth() - 200, System::instance()->GetWindowHeigh() - 224, "assets/fonts/Ubuntu-R.ttf", 18, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);
	new_record_text->init("Score Replaced", 216, System::instance()->GetWindowHeigh() - 520, "assets/fonts/Audiowide.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);
	new_performance_text->init("Driver Upgrade", System::instance()->GetWindowWidth() - 200, System::instance()->GetWindowHeigh() - 174, "assets/fonts/Audiowide.ttf", 18, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);

	delete [] difficulty_ch;
	delete [] pure_ch;
	delete [] great_ch;
	delete [] good_ch;
	delete [] error_ch;
	delete [] score_ch;
	delete [] performance_ch;
}

void fr::ResultState::clear()
{
	
}

void fr::ResultState::update()
{
	b_return->update();
	b_retry->update();

	if (System::instance()->IsWindowModified())
	{
		song_base->SetPos(0, System::instance()->GetWindowHeigh() - 720);
		score_base->SetPos(0, System::instance()->GetWindowHeigh() - 544);
		rank_base->SetPos(System::instance()->GetWindowWidth() - 232, System::instance()->GetWindowHeigh() - 720);
		performance_base->SetPos(System::instance()->GetWindowWidth() - 232, System::instance()->GetWindowHeigh() - 488);
		user_base->SetPos(System::instance()->GetWindowWidth() - 232,  System::instance()->GetWindowHeigh() - 318);
		b_return->SetPos(0, System::instance()->GetWindowHeigh() - 80);
		b_retry->SetPos(System::instance()->GetWindowWidth() - 176, System::instance()->GetWindowHeigh() - 80);

		title_text->SetPos(32, System::instance()->GetWindowHeigh() - 696);
		artist_text->SetPos(32, System::instance()->GetWindowHeigh() - 648);
		version_text->SetPos(96, System::instance()->GetWindowHeigh() - 608);
		difficulty_text->SetPos(32, System::instance()->GetWindowHeigh() - 608);
		score_text->SetPos(32, System::instance()->GetWindowHeigh() - 520);
		pure_text->SetPos(32, System::instance()->GetWindowHeigh() - 472);
		great_text->SetPos(32, System::instance()->GetWindowHeigh() - 432);
		good_text->SetPos(32, System::instance()->GetWindowHeigh() - 392);
		error_text->SetPos(32, System::instance()->GetWindowHeigh() - 352);
		rank_text->SetPos(System::instance()->GetWindowWidth() - 116, System::instance()->GetWindowHeigh() - 604);
		user_name_text->SetPos(System::instance()->GetWindowWidth() - 200, System::instance()->GetWindowHeigh() - 280);
		performance_point_text->SetPos(System::instance()->GetWindowWidth() - 200, System::instance()->GetWindowHeigh() - 224);
		new_record_text->SetPos(216, System::instance()->GetWindowHeigh() - 520);
		new_performance_text->SetPos(System::instance()->GetWindowWidth() - 200, System::instance()->GetWindowHeigh() - 174);
		
	}

	UserProfile::instance()->RenderCharacter();
	song_base->render();
	score_base->render();
	rank_base->render();
	performance_base->render();
	user_base->render();
	b_return->render();
	b_retry->render();

	title_text->render();
	artist_text->render();
	version_text->render();
	difficulty_text->render();
	score_text->render();
	pure_text->render();
	great_text->render();
	good_text->render();
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

	if (b_return->IsReleased())
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