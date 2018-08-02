#include "loading_state.h"
#include <thread>
#include "../system.h"
#include "../sprite.h"
#include "../gui/text_area.h"
#include "../timer.h"
#include "../texture_manager.h"
#include "../sound_manager.h"
#include "../animator.h"

#include "../main/main_state.h"
#include "../prepare/prepare_state.h"
#include "../game/game_state.h"
#include "../game/result_state.h"
#include "../game/gameplay_wizard_state.h"
#include "../about/about_state.h"

fr::LoadingState *fr::LoadingState::m_instance = 0;
fr::State *fr::LoadingState::next_state = 0;
fr::State *fr::LoadingState::last_state = 0;
bool fr::LoadingState::is_loaded = false;

void fr::LoadingState::update()
{
	if (System::instance()->IsWindowModified())
	{
	}

	if (!is_entered)
	{
		OnEnter();
/*
		for (int i = 0; i < text_area_top.size(); i++)
		{
			text_area_top[i]->render(text_area_top[i]->GetX(), text_area_top[i]->GetY() + shutter_top->GetY());
		}
		for (int i = 0; i < text_area_bottom.size(); i++)
		{
			text_area_bottom[i]->render(text_area_bottom[i]->GetX(), text_area_bottom[i]->GetY() + shutter_bottom->GetY());
		}
*/
		render();
	}
	else if (!is_loaded)
	{
//		std::thread refresh_thread(&fr::LoadingState::OnLoad);
//		refresh_thread.detach();
		OnLoad();
/*
		for (int i = 0; i < text_area_top.size(); i++)
		{
			text_area_top[i]->render(text_area_top[i]->GetX(), text_area_top[i]->GetY() + shutter_top->GetY());
		}
		for (int i = 0; i < text_area_bottom.size(); i++)
		{
			text_area_bottom[i]->render(text_area_bottom[i]->GetX(), text_area_bottom[i]->GetY() + shutter_bottom->GetY());
		}
*/
		if (is_loaded)
		{
			Animator::instance()->Animate("loading_exit");
		}
		render();
		//SoundManager::instance()->play("assets/shutter_open.wav", SOUNDTYPE_SFX);
		//加載完立刻放音效
	}
	else if (!is_exited)
	{
		OnExit();
/*
		for (int i = 0; i < text_area_top.size(); i++)
		{
			text_area_top[i]->render(text_area_top[i]->GetX(), text_area_top[i]->GetY() + shutter_top->GetY());
		}
		for (int i = 0; i < text_area_bottom.size(); i++)
		{
			text_area_bottom[i]->render(text_area_bottom[i]->GetX(), text_area_bottom[i]->GetY() + shutter_bottom->GetY());
		}
*/
		if (is_exited)
		{
/*
			for (int i = 0; i < text_area_top.size(); i++)
			{
				delete text_area_top[i];
			}
			for (int i = 0; i < text_area_bottom.size(); i++)
			{
				delete text_area_bottom[i];
			}
			text_area_top.clear();
			text_area_bottom.clear();
*/
		}
		render();
	}
}

void fr::LoadingState::init()
{
	shutter_middle_portrait = new Sprite;
	shutter_middle_landscape = new Sprite;
	shutter_left_portrait = new Sprite;
	shutter_top_landscape = new Sprite;
	shutter_right_portrait = new Sprite;
	shutter_bottom_landscape = new Sprite;
	shutter_middle_portrait->init("assets/base/shutter_middle_portrait.png");
	shutter_middle_landscape->init("assets/base/shutter_middle_landscape.png");
	shutter_left_portrait->init("assets/base/shutter_left_portrait.png");
	shutter_top_landscape->init("assets/base/shutter_top_landscape.png");
	shutter_right_portrait->init("assets/base/shutter_right_portrait.png");
	shutter_bottom_landscape->init("assets/base/shutter_bottom_landscape.png");
	//SoundManager::instance()->load("assets/shutter_close.wav", SOUNDTYPE_SFX);
	//SoundManager::instance()->load("assets/shutter_open.wav", SOUNDTYPE_SFX);
	Animator::instance()->AddAnimation("loading_enter", ANIMATIONTYPE_UNIFORMLY_DECELERATED, 1000);
	Animator::instance()->AddAnimation("loading_exit", ANIMATIONTYPE_UNIFORMLY_ACCELERATED, 1000);

	is_entered = true;
	is_loaded = true;
	is_exited = true;
}

void fr::LoadingState::render()
{
	if (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT)
	{
		shutter_left_portrait->render();
		shutter_right_portrait->render();
		shutter_middle_portrait->render();
	}
	else
	{
		shutter_top_landscape->render();
		shutter_bottom_landscape->render();
		shutter_middle_landscape->render();
	}
}

void fr::LoadingState::init(StateID next_state_id)
{
	is_entered = false;
	is_loaded = false;
	is_exited = false;
	switch (next_state_id)
	{
		case STATE_MAIN:
			next_state = MainState::instance();
		break;
		case STATE_PREPARE:
			next_state = PrepareState::instance();
		break;
		case STATE_GAME:
			next_state = GameState::instance();
		break;
		case STATE_RESULT:
			next_state = ResultState::instance();
		break;
		case STATE_GAMEPLAY_WIZARD:
			next_state = GameplayWizardState::instance();
		break;
		case STATE_ABOUT:
			next_state = AboutState::instance();
		break;
	}
	last_state = System::instance()->current_state;
	TextArea *new_text_area = new TextArea;
//	new_text_area->init("Loading...", System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2, "assets/fonts/Audiowide.ttf", 80, 0x00, 0x00, 0x00);
//	text_area_bottom.push_back(new_text_area);
	is_text_default = true;
	Animator::instance()->Animate("loading_enter");
	//SoundManager::instance()->play("assets/shutter_close.wav", SOUNDTYPE_SFX);
	//一開始就放音效
}

void fr::LoadingState::clear()
{
}

void fr::LoadingState::OnEnter()
{
	shutter_left_portrait->SetPos(-shutter_left_portrait->GetW() + (System::instance()->GetWindowWidth() / 2) * Animator::instance()->GetProcess("loading_enter"), System::instance()->GetWindowHeigh() - shutter_left_portrait->GetH());
	shutter_right_portrait->SetPos(System::instance()->GetWindowWidth() - (System::instance()->GetWindowWidth() / 2) * Animator::instance()->GetProcess("loading_enter"), System::instance()->GetWindowHeigh() - shutter_right_portrait->GetH());
	shutter_middle_portrait->SetPos(System::instance()->GetWindowWidth() / 2 - shutter_middle_portrait->GetW() / 2, -shutter_middle_portrait->GetH() + System::instance()->GetWindowHeigh() * Animator::instance()->GetProcess("loading_enter"));

	shutter_top_landscape->SetPos(System::instance()->GetWindowWidth() - shutter_top_landscape->GetW(), -shutter_top_landscape->GetH() + (System::instance()->GetWindowHeigh() / 2) * Animator::instance()->GetProcess("loading_enter"));
	shutter_bottom_landscape->SetPos(System::instance()->GetWindowWidth() - shutter_bottom_landscape->GetW(), System::instance()->GetWindowHeigh() - (System::instance()->GetWindowHeigh() / 2) * Animator::instance()->GetProcess("loading_enter"));
	shutter_middle_landscape->SetPos(-shutter_middle_landscape->GetW() + System::instance()->GetWindowWidth() * Animator::instance()->GetProcess("loading_enter"), System::instance()->GetWindowHeigh() / 2 - shutter_middle_landscape->GetH() / 2);
	if (Animator::instance()->IsTimeUp("loading_enter"))
	{
//		shutter_top->SetPos(0, System::instance()->GetWindowHeigh() - 534);
//		shutter_bottom->SetPos(0, 0);
		Animator::instance()->ResetAnimation("loading_enter");
		is_entered = true;
	}
}

void fr::LoadingState::OnExit()
{
	shutter_left_portrait->SetPos(-shutter_left_portrait->GetW() + (System::instance()->GetWindowWidth() / 2) * (1.0f - Animator::instance()->GetProcess("loading_exit")), System::instance()->GetWindowHeigh() - shutter_left_portrait->GetH());
	shutter_right_portrait->SetPos(System::instance()->GetWindowWidth() - (System::instance()->GetWindowWidth() / 2) * (1.0f - Animator::instance()->GetProcess("loading_exit")), System::instance()->GetWindowHeigh() - shutter_right_portrait->GetH());
	shutter_middle_portrait->SetPos(System::instance()->GetWindowWidth() / 2 - shutter_middle_portrait->GetW() / 2, -shutter_middle_portrait->GetH() + System::instance()->GetWindowHeigh() * (1.0f - Animator::instance()->GetProcess("loading_exit")));

	shutter_top_landscape->SetPos(System::instance()->GetWindowWidth() - shutter_top_landscape->GetW(), -shutter_top_landscape->GetH() + (System::instance()->GetWindowHeigh() / 2) * (1.0f - Animator::instance()->GetProcess("loading_exit")));
	shutter_bottom_landscape->SetPos(System::instance()->GetWindowWidth() - shutter_bottom_landscape->GetW(), System::instance()->GetWindowHeigh() - (System::instance()->GetWindowHeigh() / 2) * (1.0f - Animator::instance()->GetProcess("loading_exit")));
	shutter_middle_landscape->SetPos(-shutter_middle_landscape->GetW() + System::instance()->GetWindowWidth() * (1.0f - Animator::instance()->GetProcess("loading_exit")), System::instance()->GetWindowHeigh() / 2 - shutter_middle_landscape->GetH() / 2);
	if (Animator::instance()->IsTimeUp("loading_exit"))
	{
//		shutter_top->SetPos(0, System::instance()->GetWindowHeigh());
//		shutter_bottom->SetPos(0, -848);
		Animator::instance()->ResetAnimation("loading_exit");
		is_exited = true;
		is_entered = false;
		is_loaded = false;
	}
}

void fr::LoadingState::OnLoad()
{
	last_state->clear();
	next_state->init();
	System::instance()->current_state = next_state;
	is_loaded = true;
}
/*
void fr::LoadingState::AddText(std::string text, std::string font_path, int font_size, int x, int y, char r, char g, char b, ShutterType type)
{
	if (is_text_default)
	{
		delete text_area_bottom[0];
		text_area_bottom.clear();
		is_text_default = false;
	}
	TextArea *new_text_area = new TextArea;
	new_text_area->init(text, x, y, font_path, font_size, r, g, b);
	if (type == SHUTTER_TOP)
	{
		text_area_top.push_back(new_text_area);
	}
	else
	{
		text_area_bottom.push_back(new_text_area);
	}
}
*/

bool fr::LoadingState::IsSwitching()
{
	return !is_exited;
}

bool fr::LoadingState::IsLoading()
{
	return is_entered && !is_loaded;
}