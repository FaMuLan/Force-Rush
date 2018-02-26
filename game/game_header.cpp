#include "game_header.h"
#include "../sprite.h"
#include "../button.h"
#include "../text_area.h"
#include "../texture_manager.h"
#include "../animator.h"
#include "../system.h"
#include "../sound_manager.h"
#include "../timer.h"
#include "../song_data.h"
#include "../loading/loading_state.h"
#include "game_state.h"
#include "beatmap.h"

fr::GameHeader *fr::GameHeader::m_instance = 0;

void fr::GameHeader::init()
{
	pause_widget_base = new Sprite;
	score_base = new Sprite;
	user_base = new Sprite;
	duration_process_bar = new Sprite;
	title_base = new Button;
	pause_resume = new Button;
	pause_retry = new Button;
	pause_retire = new Button;
	score_text = new TextArea;
	duration_text = new TextArea;

		TextureManager::instance()->loadfont("assets/fonts/Audiowide.ttf", 36);
	TextureManager::instance()->loadfont("assets/fonts/Audiowide.ttf", 56);
	TextureManager::instance()->loadfont("assets/fonts/Audiowide.ttf", 20);

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

	title_base->init("assets/game/title_base.png", Rect(System::instance()->GetWindowWidth() / 2 - 360, 0, 0, 0));
	title_base->AddPressedFrame("assets/game/title_base_pressed.png");
	title_base->AddText(GameState::instance()->m_information->title, title_base->GetW() / 2, title_base->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
	score_base->init("assets/base/widget_min_base.png");
	score_base->SetPos(0, title_base->GetX() >= score_base->GetW() ? 0 : 64);
	user_base->init("assets/base/widget_min_base.png");
	user_base->SetPos(System::instance()->GetWindowWidth() - user_base->GetW(), title_base->GetX() >= user_base->GetW() ? 0 : 64);
	duration_process_bar->init("assets/base/process_bar.png");
	duration_process_bar->SetPos(score_base->GetX() + 32, score_base->GetY() + 124);
	score_text->init("0", score_base->GetX() + 32, score_base->GetY() + 16, "assets/fonts/Audiowide.ttf", 56, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);
	duration_text->init(" ", score_base->GetX() + 32, score_base->GetY() + 88, "assets/fonts/Audiowide.ttf", 20, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);
	Animator::instance()->AddAnimation("pause_widget_enter", ANIMATIONTYPE_UNIFORMLY_DECELERATED, 300);
	Animator::instance()->AddAnimation("pause_widget_exit", ANIMATIONTYPE_UNIFORMLY_ACCELERATED, 300);
	pause_is_shown = false;
	pause_is_entered = false;
	pause_is_exited = true;
}

void fr::GameHeader::clear()
{
	
}

void fr::GameHeader::update()
{
	title_base->update();
	char *score_ch = new char[6];
	sprintf(score_ch, "%d", GameBeatmap::instance()->GetScore());
	score_text->SetText(score_ch);
	delete [] score_ch;

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
	float process = float(Timer::instance()->GetTime("game") - 2000) / GameState::instance()->m_information->duration;
	process = Timer::instance()->GetTime("game") - 2000 < 0 ? 0 : process;
	process = Timer::instance()->GetTime("game") - 2000 > GameState::instance()->m_information->duration ? 1 : process;
	duration_process_bar->SetSrcRect(Rect(0, 0, process * 216.f, 8));
	duration_process_bar->SetSize(process * 216.f, 8);

	if (title_base->IsReleased())
	{
		GameHeader::instance()->SwitchPause();
		SoundManager::instance()->SwitchPause();
		if (GameHeader::instance()->IsPaused())
		{
			Timer::instance()->PauseTimer("game");
		}
		else
		{
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
			score_text->SetPos(score_base->GetX() + 32, score_base->GetY() + 16);
			duration_text->SetPos(score_base->GetX() + 32, score_base->GetY() + 88);
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
			score_text->SetPos(score_base->GetX() + 32, score_base->GetY() + 16);
			duration_text->SetPos(score_base->GetX() + 32, score_base->GetY() + 88);
			pause_widget_base->SetPos(System::instance()->GetWindowWidth() / 2 - pause_widget_base->GetW() / 2, 0);
			pause_resume->SetPos(pause_widget_base->GetX() + 32, pause_widget_base->GetY() + 32);
			pause_retry->SetPos(pause_widget_base->GetX() + pause_widget_base->GetW() / 2 - pause_retry->GetW() / 2, pause_widget_base->GetY() + 32);
			pause_retire->SetPos(pause_widget_base->GetX() + pause_widget_base->GetW() - pause_retire->GetW() - 32, pause_widget_base->GetY() + 32);
		}
		pause_resume->update();
		pause_retry->update();
		pause_retire->update();

		if (pause_resume->IsReleased())
		{
			GameHeader::instance()->SwitchPause();
			SoundManager::instance()->SwitchPause();
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
	}
	score_base->render();
	user_base->render();
	score_text->render();
	duration_text->render();
	title_base->render();
	duration_process_bar->render();
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
	duration_process_bar->SetPos(score_base->GetX() + 32, score_base->GetY() + 124);
	score_text->SetPos(score_base->GetX() + 32, score_base->GetY() + 16);
	duration_text->SetPos(score_base->GetX() + 32, score_base->GetY() + 88);
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
	duration_process_bar->SetPos(score_base->GetX() + 32, score_base->GetY() + 124);
	score_text->SetPos(score_base->GetX() + 32, score_base->GetY() + 16);
	duration_text->SetPos(score_base->GetX() + 32, score_base->GetY() + 88);
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