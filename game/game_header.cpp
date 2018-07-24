#include "game_header.h"
#include "../sprite.h"
#include "../gui/button.h"
#include "../gui/text_area.h"
#include "../texture_manager.h"
#include "../control_handler.h"
#include "../animator.h"
#include "../system.h"
#include "../sound_manager.h"
#include "../timer.h"
#include "../song_data.h"
#include "../loading/loading_state.h"
#include "../user/user_profile.h"
#include "../user/setting.h"
#include "game_state.h"
#include "beatmap.h"

fr::GameHeader *fr::GameHeader::m_instance = 0;

void fr::GameHeader::init()
{
	pause_widget_base = new Sprite;
	score_base = new Sprite;
	user_base = new Sprite;
	duration_process_bar = new Sprite;
	performance_process_bar = new Sprite;
	title_base = new Button;
	pause_resume = new Button;
	pause_retry = new Button;
	pause_retire = new Button;
	score_text = new TextArea;
	duration_text = new TextArea;
	performance_text = new TextArea;
	user_name_text = new TextArea;
	tips_text = new TextArea;

	pause_widget_base->init("assets/game/pause_widget_base.png");
	pause_resume->init("assets/base/sort_button.png");
	pause_resume->AddPressedFrame("assets/base/sort_button_pressed.png");
	pause_resume->AddText("Resume", pause_resume->GetW() / 2, pause_resume->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
	pause_retry->init("assets/base/sort_button.png");
	pause_retry->AddPressedFrame("assets/base/sort_button_pressed.png");
	pause_retry->AddText("Retry", pause_retry->GetW() / 2, pause_retry->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
	pause_retire->init("assets/base/sort_button.png");
	pause_retire->AddPressedFrame("assets/base/sort_button_pressed.png");
	pause_retire->AddText("Retire", pause_retire->GetW() / 2, pause_retire->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);

	tips_text->init("Tips: " + Setting::instance()->GetRandomTips(), pause_widget_base->GetX() + pause_widget_base->GetW() / 2, pause_widget_base->GetY() + 188, "assets/fonts/Miui-Regular.ttf", 25, 0x00, 0x00, 0x00, TEXTFORMAT_MIDDLE, 560, true);

	title_base->init("assets/game/title_base.png", Rect(System::instance()->GetWindowWidth() / 2 - 360, 0, 0, 0));
	title_base->AddPressedFrame("assets/game/title_base_pressed.png");
	title_base->AddText(GameState::instance()->m_information->title, title_base->GetW() / 2, title_base->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00, TEXTFORMAT_MIDDLE, 720);
	score_base->init("assets/base/widget_min_base.png");
	score_base->SetPos(0, title_base->GetX() >= score_base->GetW() ? 0 : 64);
	user_base->init("assets/base/widget_min_base.png");
	user_base->SetPos(System::instance()->GetWindowWidth() - user_base->GetW(), title_base->GetX() >= user_base->GetW() ? 0 : 64);
	duration_process_bar->init("assets/base/process_bar.png");
	duration_process_bar->SetPos(score_base->GetX() + 32, score_base->GetY() + 124);
	performance_process_bar->init("assets/base/process_bar.png");
	performance_process_bar->SetPos(user_base->GetX() + 32, user_base->GetY() + 124);
	score_text->init("0", score_base->GetX() + 32, score_base->GetY() + 16, "assets/fonts/Audiowide.ttf", 56, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);
	duration_text->init(" ", score_base->GetX() + 32, score_base->GetY() + 88, "assets/fonts/Audiowide.ttf", 20, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);
	performance_text->init(" ", user_base->GetX() + 32, user_base->GetY() + 88, "assets/fonts/Audiowide.ttf", 14, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);
	user_name_text->init(UserProfile::instance()->GetUserName(), user_base->GetX() + 32, user_base->GetY() + 16, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);
	Animator::instance()->AddAnimation("pause_widget_enter", ANIMATIONTYPE_UNIFORMLY_DECELERATED, 300);
	Animator::instance()->AddAnimation("pause_widget_exit", ANIMATIONTYPE_UNIFORMLY_ACCELERATED, 300);
	pause_is_shown = false;
	pause_is_entered = false;
	pause_is_exited = true;
}

void fr::GameHeader::clear()
{
	pause_widget_base->clear();
	title_base->clear();
	score_base->clear();
	user_base->clear();
	duration_process_bar->clear();
	performance_process_bar->clear();
	pause_resume->clear();
	pause_retry->clear();
	pause_retire->clear();
	score_text->clear();
	duration_text->clear();
	performance_text->clear();
	user_name_text->clear();
	tips_text->clear();
	delete pause_widget_base;
	delete title_base;
	delete score_base;
	delete user_base;
	delete duration_process_bar;
	delete performance_process_bar;
	delete pause_resume;
	delete pause_retry;
	delete pause_retire;
	delete score_text;
	delete duration_text;
	delete performance_text;
	delete user_name_text;
	delete tips_text;
}

void fr::GameHeader::update()
{
	title_base->update();
	int current_performance_point = UserProfile::instance()->CalculatePerformancePoint(GameState::instance()->m_information->difficulty, float(GameBeatmap::instance()->GetScore()) / float(GameState::instance()->m_information->full_score));
	char *score_ch = new char[6];
	char *performance_ch = new char[20];
	sprintf(score_ch, "%d", GameBeatmap::instance()->GetScore());
	sprintf(performance_ch, "performance:%d / %d", current_performance_point, UserProfile::instance()->GetPerformancePoint());
	score_text->SetText(score_ch);
	performance_text->SetText(performance_ch);
	delete [] score_ch;
	delete [] performance_ch;

	char *duration_ch = new char[20];
	int current_min = (Timer::instance()->GetTime("game") - 2000) / 60000;
	int current_sec = (Timer::instance()->GetTime("game") - 2000) / 1000 - current_min * 60;
	int song_length_min = GameState::instance()->m_information->duration / 60000;
	int song_length_sec = GameState::instance()->m_information->duration / 1000 - song_length_min * 60;
	current_min = Timer::instance()->GetTime("game") - 2000 > GameState::instance()->m_information->duration ? song_length_min : current_min;
	current_sec = Timer::instance()->GetTime("game") - 2000 > GameState::instance()->m_information->duration ? song_length_sec : current_sec;
	current_min = Timer::instance()->GetTime("game") <= 2000 ? 0 : current_min;
	current_sec = Timer::instance()->GetTime("game") <= 2000 ? 0 : current_sec;
	sprintf(duration_ch, "%d:%02d / %d:%02d", current_min, current_sec, song_length_min, song_length_sec);
	duration_text->SetText(duration_ch);
	delete [] duration_ch;

	float duration_process = float(Timer::instance()->GetTime("game") - 2000) / GameState::instance()->m_information->duration;
	duration_process = duration_process < 0 ? 0 : duration_process;
	duration_process = duration_process > 1 ? 1 : duration_process;
	duration_process_bar->SetSrcRect(Rect(0, 0, duration_process * 216.f, 8));
	duration_process_bar->SetSize(duration_process * 216.f, 8);

	float performance_process = float(current_performance_point) / float(UserProfile::instance()->GetPerformancePoint());
	performance_process = performance_process < 0 ? 0 : performance_process;
	performance_process = performance_process > 1 ? 1 : performance_process;
	performance_process_bar->SetSrcRect(Rect(0, 0, performance_process * 216.f, 8));
	performance_process_bar->SetSize(performance_process * 216.f, 8);

	if (title_base->IsReleased() || ControlHandler::instance()->IsKeyDown(SDL_SCANCODE_AC_BACK))
	{
		GameHeader::instance()->SwitchPause();
//		SoundManager::instance()->SwitchPause("default_music");
		if (GameHeader::instance()->IsPaused())
		{
			SoundManager::instance()->GetProcess("default_music")->is_playing = false;
			Timer::instance()->PauseTimer("game");
			tips_text->SetText("Tips: " + Setting::instance()->GetRandomTips());
		}
		else
		{
			SoundManager::instance()->GetProcess("default_music")->is_playing = true;
			Timer::instance()->RunTimer("game");
		}
	}

	if (!pause_is_shown && pause_is_exited)	//normal
	{
		if (System::instance()->IsWindowModified())
		{
			title_base->SetPos(System::instance()->GetWindowWidth() / 2 - title_base->GetW() / 2, 0);
			score_base->SetPos(0, title_base->GetX() >= score_base->GetW() ? 0 : 64);
			user_base->SetPos(System::instance()->GetWindowWidth() - user_base->GetW(), title_base->GetX() >= user_base->GetW() ? 0 : 64);
			duration_process_bar->SetPos(score_base->GetX() + 32, score_base->GetY() + 124);
			performance_process_bar->SetPos(user_base->GetX() + 32, user_base->GetY() + 124);
			score_text->SetPos(score_base->GetX() + 32, score_base->GetY() + 16);
			performance_text->SetPos(user_base->GetX() + 32, user_base->GetY() + 88);
			duration_text->SetPos(score_base->GetX() + 32, score_base->GetY() + 88);
			user_name_text->SetPos(user_base->GetX() + 32, user_base->GetY() + 16);
		}
	}
	if (pause_is_shown && pause_is_entered)
	{
		if (System::instance()->IsWindowModified())
		{
			title_base->SetPos(System::instance()->GetWindowWidth() / 2 - title_base->GetW() / 2, 320);
			score_base->SetPos(0, title_base->GetX() >= score_base->GetW() ? 0 : 384);
			user_base->SetPos(System::instance()->GetWindowWidth() - user_base->GetW(), title_base->GetX() >= user_base->GetW() ? 0 : 384);
			duration_process_bar->SetPos(score_base->GetX() + 32, score_base->GetY() + 124);
			performance_process_bar->SetPos(user_base->GetX() + 32, user_base->GetY() + 124);
			score_text->SetPos(score_base->GetX() + 32, score_base->GetY() + 16);
			performance_text->SetPos(user_base->GetX() + 32, user_base->GetY() + 88);
			duration_text->SetPos(score_base->GetX() + 32, score_base->GetY() + 88);
			user_name_text->SetPos(user_base->GetX() + 32, user_base->GetY() + 16);
			pause_widget_base->SetPos(System::instance()->GetWindowWidth() / 2 - pause_widget_base->GetW() / 2, 0);
			pause_resume->SetPos(pause_widget_base->GetX() + 32, pause_widget_base->GetY() + 32);
			pause_retry->SetPos(pause_widget_base->GetX() + pause_widget_base->GetW() / 2 - pause_retry->GetW() / 2, pause_widget_base->GetY() + 32);
			pause_retire->SetPos(pause_widget_base->GetX() + pause_widget_base->GetW() - pause_retire->GetW() - 32, pause_widget_base->GetY() + 32);
			tips_text->SetPos(pause_widget_base->GetX() + pause_widget_base->GetW() / 2, pause_widget_base->GetY() + 188);
		}
		pause_resume->update();
		pause_retry->update();
		pause_retire->update();

		if (pause_resume->IsReleased())
		{
			GameHeader::instance()->SwitchPause();
//			SoundManager::instance()->SwitchPause("default_music");
			Timer::instance()->RunTimer("game");
		}
		if (pause_retry->IsReleased())
		{
			LoadingState::instance()->init(STATE_GAME);
			GameState::instance()->SetFile(GameState::instance()->m_information);
		}
		if (pause_retire->IsReleased())
		{
			LoadingState::instance()->init(STATE_PREPARE);
		}
	}
	else if (pause_is_shown)
	{
		PauseOnEnter();
	}
	else if (!pause_is_shown && !pause_is_exited)
	{
		PauseOnExit();
	}
}

void fr::GameHeader::render()
{
	if (!pause_is_exited)
	{
		pause_widget_base->render();
		pause_resume->render();
		pause_retry->render();
		pause_retire->render();
		tips_text->render();
	}
	score_base->render();
	user_base->render();
	score_text->render();
	duration_text->render();
	performance_text->render();
	title_base->render();
	duration_process_bar->render();
	performance_process_bar->render();
	user_name_text->render();
}

void fr::GameHeader::PauseOnEnter()
{
	int widget_y =  -pause_widget_base->GetH() + pause_widget_base->GetH() * Animator::instance()->GetProcess("pause_widget_enter");

	title_base->SetPos(System::instance()->GetWindowWidth() / 2 - title_base->GetW() / 2, widget_y + pause_widget_base->GetH());
	score_base->SetPos(0, title_base->GetX() >= score_base->GetW() ? 0 : 64 + widget_y + pause_widget_base->GetH());
	user_base->SetPos(System::instance()->GetWindowWidth() - user_base->GetW(), title_base->GetX() >= user_base->GetW() ? 0 : 64 + widget_y + pause_widget_base->GetH());
	pause_widget_base->SetPos(System::instance()->GetWindowWidth() / 2 - pause_widget_base->GetW() / 2, widget_y);
	pause_resume->SetPos(pause_widget_base->GetX() + 32, pause_widget_base->GetY() + 32);
	pause_retry->SetPos(pause_widget_base->GetX() + pause_widget_base->GetW() / 2 - pause_retry->GetW() / 2, pause_widget_base->GetY() + 32);
	pause_retire->SetPos(pause_widget_base->GetX() + pause_widget_base->GetW() - pause_retire->GetW() - 32, pause_widget_base->GetY() + 32);
	tips_text->SetPos(pause_widget_base->GetX() + pause_widget_base->GetW() / 2, pause_widget_base->GetY() + 188);
	duration_process_bar->SetPos(score_base->GetX() + 32, score_base->GetY() + 124);
	performance_process_bar->SetPos(user_base->GetX() + 32, user_base->GetY() + 124);
	score_text->SetPos(score_base->GetX() + 32, score_base->GetY() + 16);
	performance_text->SetPos(user_base->GetX() + 32, user_base->GetY() + 88);
	duration_text->SetPos(score_base->GetX() + 32, score_base->GetY() + 88);
	user_name_text->SetPos(user_base->GetX() + 32, user_base->GetY() + 16);
	if (Animator::instance()->IsTimeUp("pause_widget_enter"))
	{		Animator::instance()->ResetAnimation("pause_widget_enter");
		pause_is_entered = true;
	}
}

void fr::GameHeader::PauseOnExit()
{
	int widget_y = -pause_widget_base->GetH() * Animator::instance()->GetProcess("pause_widget_exit");

	title_base->SetPos(System::instance()->GetWindowWidth() / 2 - title_base->GetW() / 2, widget_y + pause_widget_base->GetH());
	score_base->SetPos(0, title_base->GetX() >= score_base->GetW() ? 0 : 64 + widget_y + pause_widget_base->GetH());
	user_base->SetPos(System::instance()->GetWindowWidth() - user_base->GetW(), title_base->GetX() >= user_base->GetW() ? 0 : 64 + widget_y + pause_widget_base->GetH());
	pause_widget_base->SetPos(System::instance()->GetWindowWidth() / 2 - pause_widget_base->GetW() / 2, widget_y);
	pause_resume->SetPos(pause_widget_base->GetX() + 32, pause_widget_base->GetY() + 32);
	pause_retry->SetPos(pause_widget_base->GetX() + pause_widget_base->GetW() / 2 - pause_retry->GetW() / 2, pause_widget_base->GetY() + 32);
	pause_retire->SetPos(pause_widget_base->GetX() + pause_widget_base->GetW() - pause_retire->GetW() - 32, pause_widget_base->GetY() + 32);
	tips_text->SetPos(pause_widget_base->GetX() + pause_widget_base->GetW() / 2, pause_widget_base->GetY() + 188);
	duration_process_bar->SetPos(score_base->GetX() + 32, score_base->GetY() + 124);
	performance_process_bar->SetPos(user_base->GetX() + 32, user_base->GetY() + 124);
	score_text->SetPos(score_base->GetX() + 32, score_base->GetY() + 16);
	performance_text->SetPos(user_base->GetX() + 32, user_base->GetY() + 88);
	duration_text->SetPos(score_base->GetX() + 32, score_base->GetY() + 88);
	user_name_text->SetPos(user_base->GetX() + 32, user_base->GetY() + 16);
	if (Animator::instance()->IsTimeUp("pause_widget_exit"))
	{
		Animator::instance()->ResetAnimation("pause_widget_exit");
		pause_is_entered = false;
		pause_is_exited = true;
	}
}

void fr::GameHeader::SwitchPause()
{
	pause_is_shown = !pause_is_shown;
	if (pause_is_shown)
	{
		Animator::instance()->Animate("pause_widget_enter");
		pause_is_exited = false;
	}
	else
	{
		Animator::instance()->Animate("pause_widget_exit");
	}
}

bool fr::GameHeader::IsPaused()
{
	return pause_is_shown;
}