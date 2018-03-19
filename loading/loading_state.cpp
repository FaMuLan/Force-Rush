#include "loading_state.h"
#include <thread>
#include "../system.h"
#include "../sprite.h"
#include "../text_area.h"
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
		arrow_front_pos.clear();
		arrow_back_pos.clear();
		int arrow_count = 0;
		background_length = System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT ? System::instance()->GetWindowHeigh() : System::instance()->GetWindowWidth();
		arrow_count = background_length / 420 + 1;
		for (int i = 0; i < arrow_count; i++)
		{
			arrow_front_pos.push_back(0);
			arrow_back_pos.push_back(0);
		}
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
	shutter_arrow_front_portrait = new Sprite;
	shutter_arrow_front_landscape = new Sprite;
	shutter_arrow_back_portrait = new Sprite;
	shutter_arrow_back_landscape = new Sprite;
	shutter_arrow_front_portrait->init("assets/base/shutter_arrow_front_portrait.png");
	shutter_arrow_front_landscape->init("assets/base/shutter_arrow_front_landscape.png");
	shutter_arrow_back_portrait->init("assets/base/shutter_arrow_back_portrait.png");
	shutter_arrow_back_landscape->init("assets/base/shutter_arrow_back_landscape.png");
	//SoundManager::instance()->load("assets/shutter_close.wav", SOUNDTYPE_SFX);
	//SoundManager::instance()->load("assets/shutter_open.wav", SOUNDTYPE_SFX);
	Animator::instance()->AddAnimation("loading_enter", ANIMATIONTYPE_UNIFORMLY_DECELERATED, 1000);
	Animator::instance()->AddAnimation("loading_exit", ANIMATIONTYPE_UNIFORMLY_ACCELERATED, 1000);

	int arrow_count = 0;
	background_length = System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT ? System::instance()->GetWindowHeigh() : System::instance()->GetWindowWidth();
	arrow_count = background_length / 420 + 1;
	for (int i = 0; i < arrow_count; i++)
	{
		arrow_front_pos.push_back(0);
		arrow_back_pos.push_back(0);
	}

	is_entered = true;
	is_loaded = true;
	is_exited = true;
}

void fr::LoadingState::render()
{
	if (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT)
	{
		for (int i = 0; i < arrow_front_pos.size(); i++)
		{
			shutter_arrow_back_portrait->SetPos(System::instance()->GetWindowWidth() / 2 - shutter_arrow_back_portrait->GetW() / 2, arrow_back_pos[i]);
			shutter_arrow_back_portrait->render();
		}
		for (int i = 0; i < arrow_front_pos.size(); i++)
		{
			shutter_arrow_front_portrait->SetPos(System::instance()->GetWindowWidth() / 2 - shutter_arrow_front_portrait->GetW() / 2, arrow_front_pos[i]);
			shutter_arrow_front_portrait->render();
		}
	}
	else
	{
		for (int i = 0; i < arrow_back_pos.size(); i++)
		{
			shutter_arrow_back_landscape->SetPos(arrow_back_pos[i], System::instance()->GetWindowHeigh() / 2 - shutter_arrow_back_landscape->GetH() / 2);
			shutter_arrow_back_landscape->render();
		}
		for (int i = 0; i < arrow_back_pos.size(); i++)
		{
			shutter_arrow_front_landscape->SetPos(arrow_front_pos[i], System::instance()->GetWindowHeigh() / 2 - shutter_arrow_front_landscape->GetH() / 2);
			shutter_arrow_front_landscape->render();
		}
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
	for (int i = 0; i < arrow_front_pos.size(); i++)
	{
		int front_start = -1000 * (i + 1);
		int front_end = background_length - 420 * (i + 1);
		int back_start = background_length + 1000 * i;
		int back_end = -255 + 420 * i;
		arrow_front_pos[i] = front_start + (front_end - front_start) * Animator::instance()->GetProcess("loading_enter");
		arrow_back_pos[i] = back_start + (back_end - back_start) * Animator::instance()->GetProcess("loading_enter");
	}
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
	for (int i = 0; i < arrow_front_pos.size(); i++)
	{
		int front_start = background_length - 420 * (i + 1);
		int front_end = background_length + 1000 * (arrow_front_pos.size() - i - 1);
		int back_start = -255 + 420 * i;
		int back_end = -1000 * (arrow_back_pos.size() - i);
		arrow_front_pos[i] = front_start + (front_end - front_start) * Animator::instance()->GetProcess("loading_exit");
		arrow_back_pos[i] = back_start + (back_end - back_start) * Animator::instance()->GetProcess("loading_exit");
	}
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