#include "loading_state.h"
#include "../system.h"
#include "../sprite.h"
#include "../text_area.h"
#include "../timer.h"
#include "../texture_manager.h"
#include "../sound_manager.h"
#include "../animator.h"

lm::LoadingState *lm::LoadingState::m_instance = 0;

void lm::LoadingState::update()
{
	if (!is_entered)
	{
		OnEnter();
		shutter_bottom->render();
		shutter_top->render();
		shutter_left->render();
		shutter_right->render();
		shutter_center->render();

		for (int i = 0; i < text_area_top.size(); i++)
		{
			text_area_top[i]->render(text_area_top[i]->GetX(), text_area_top[i]->GetY() + shutter_top->GetY());
		}
		for (int i = 0; i < text_area_bottom.size(); i++)
		{
			text_area_bottom[i]->render(text_area_bottom[i]->GetX(), text_area_bottom[i]->GetY() + shutter_bottom->GetY());
		}
	}
	else if (!is_loaded)
	{
		last_state->clear();
		next_state->init();
		System::instance()->current_state = next_state;
		is_loaded = true;
		shutter_bottom->render();
		shutter_top->render();
		shutter_left->render();
		shutter_right->render();
		shutter_center->render();

		for (int i = 0; i < text_area_top.size(); i++)
		{
			text_area_top[i]->render(text_area_top[i]->GetX(), text_area_top[i]->GetY() + shutter_top->GetY());
		}
		for (int i = 0; i < text_area_bottom.size(); i++)
		{
			text_area_bottom[i]->render(text_area_bottom[i]->GetX(), text_area_bottom[i]->GetY() + shutter_bottom->GetY());
		}

		Animator::instance()->Animate("loading_exit");
		//SoundManager::instance()->play("assets/shutter_open.wav", SOUNDTYPE_SFX);
		//加載完立刻放音效
	}
	else if (!is_exited)
	{
		OnExit();
		shutter_bottom->render();
		shutter_top->render();
		shutter_left->render();
		shutter_right->render();
		shutter_center->render();

		for (int i = 0; i < text_area_top.size(); i++)
		{
			text_area_top[i]->render(text_area_top[i]->GetX(), text_area_top[i]->GetY() + shutter_top->GetY());
		}
		for (int i = 0; i < text_area_bottom.size(); i++)
		{
			text_area_bottom[i]->render(text_area_bottom[i]->GetX(), text_area_bottom[i]->GetY() + shutter_bottom->GetY());
		}
		if (is_exited)
		{
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
		}
	}
}

void lm::LoadingState::init()
{
	shutter_top = new Sprite;
	shutter_bottom = new Sprite;
	shutter_left = new Sprite;
	shutter_right = new Sprite;
	shutter_center = new Sprite;
	shutter_top->init("assets/base/shutter_top.png");
	shutter_bottom->init("assets/base/shutter_bottom.png");
	shutter_left->init("assets/base/shutter_left.png");
	shutter_right->init("assets/base/shutter_right.png");
	shutter_center->init("assets/base/shutter_center.png");
	TextureManager::instance()->loadfont("assets/fonts/Audiowide.ttf", 80);
	//SoundManager::instance()->load("assets/shutter_close.wav", SOUNDTYPE_SFX);
	//SoundManager::instance()->load("assets/shutter_open.wav", SOUNDTYPE_SFX);
	Animator::instance()->AddAnimation("loading_enter", ANIMATIONTYPE_UNIFORMLY_DECELERATED, 400);
	Animator::instance()->AddAnimation("loading_exit", ANIMATIONTYPE_UNIFORMLY_ACCELERATED, 400);
	is_entered = true;
	is_loaded = true;
	is_exited = true;
}

void lm::LoadingState::init(State *load_next_state, State *load_last_state)
{
	is_entered = false;
	is_loaded = false;
	is_exited = false;
	next_state = load_next_state;
	last_state = load_last_state;
	TextArea *new_text_area = new TextArea;
	new_text_area->init("Loading...", System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2, "assets/fonts/Audiowide.ttf", 80, 0x00, 0x00, 0x00);
	text_area_bottom.push_back(new_text_area);
	is_text_default = true;
	Animator::instance()->Animate("loading_enter");
	//SoundManager::instance()->play("assets/shutter_close.wav", SOUNDTYPE_SFX);
	//一開始就放音效
}

void lm::LoadingState::clear()
{
}

void lm::LoadingState::OnEnter()
{
	int shutter_top_x = System::instance()->GetWindowWidth() / 2 - (shutter_center->GetW() - 8) / 2 + shutter_top->GetW() - shutter_top->GetW() * Animator::instance()->GetProcess("loading_enter");
	int shutter_bottom_x = System::instance()->GetWindowWidth() / 2 + (shutter_center->GetW() - 8) / 2 - 2 * shutter_bottom->GetW() + shutter_bottom->GetW() * Animator::instance()->GetProcess("loading_enter");
	int shutter_left_y = System::instance()->GetWindowHeigh() / 2 + (shutter_center->GetH() - 8) / 2 - 2 * shutter_left->GetH() + shutter_left->GetH() * Animator::instance()->GetProcess("loading_enter");
	int shutter_right_y = System::instance()->GetWindowHeigh() / 2 - (shutter_center->GetH() - 8) / 2 + shutter_right->GetH() - shutter_right->GetH() * Animator::instance()->GetProcess("loading_enter");
	float shutter_center_scale = Animator::instance()->GetProcess("loading_enter");

	shutter_top->SetPos(shutter_top_x,  System::instance()->GetWindowHeigh() / 2 - (shutter_center->GetH() - 8) / 2 - shutter_top->GetH());
	shutter_bottom->SetPos(shutter_bottom_x, System::instance()->GetWindowHeigh() / 2 + (shutter_center->GetH() - 8) / 2);
	shutter_left->SetPos(System::instance()->GetWindowWidth() / 2 - (shutter_center->GetW() - 8) / 2 - shutter_left->GetW(), shutter_left_y);
	shutter_right->SetPos(System::instance()->GetWindowWidth() / 2 + (shutter_center->GetW() - 8) / 2, shutter_right_y);
	shutter_center->SetScale(shutter_center_scale);
	shutter_center->SetPos(System::instance()->GetWindowWidth() / 2 - shutter_center->GetW() / 2 * shutter_center_scale, System::instance()->GetWindowHeigh() / 2 - shutter_center->GetH() / 2 * shutter_center_scale);

	if (Animator::instance()->IsTimeUp("loading_enter"))
	{
//		shutter_top->SetPos(0, System::instance()->GetWindowHeigh() - 534);
//		shutter_bottom->SetPos(0, 0);
		Animator::instance()->ResetAnimation("loading_enter");
		is_entered = true;
	}
}

void lm::LoadingState::OnExit()
{
	int shutter_top_x = System::instance()->GetWindowWidth() / 2 - (shutter_center->GetW() - 8) / 2 + shutter_top->GetW() * Animator::instance()->GetProcess("loading_exit");
	int shutter_bottom_x = System::instance()->GetWindowWidth() / 2 + (shutter_center->GetW() - 8) / 2 - shutter_bottom->GetW() - shutter_bottom->GetW() * Animator::instance()->GetProcess("loading_exit");
	int shutter_left_y = System::instance()->GetWindowHeigh() / 2 + (shutter_center->GetH() - 8) / 2 - shutter_left->GetH() - shutter_left->GetH() * Animator::instance()->GetProcess("loading_exit");
	int shutter_right_y = System::instance()->GetWindowHeigh() / 2 - (shutter_center->GetH() - 8) / 2 + shutter_right->GetH() * Animator::instance()->GetProcess("loading_exit");
	float shutter_center_scale = 1.f - Animator::instance()->GetProcess("loading_exit");

	shutter_top->SetPos(shutter_top_x,  System::instance()->GetWindowHeigh() / 2 - (shutter_center->GetH() - 8) / 2 - shutter_top->GetH());
	shutter_bottom->SetPos(shutter_bottom_x, System::instance()->GetWindowHeigh() / 2 + (shutter_center->GetH() - 8) / 2);
	shutter_left->SetPos(System::instance()->GetWindowWidth() / 2 - (shutter_center->GetW() - 8) / 2 - shutter_left->GetW(), shutter_left_y);
	shutter_right->SetPos(System::instance()->GetWindowWidth() / 2 + (shutter_center->GetW() - 8) / 2, shutter_right_y);
	shutter_center->SetScale(shutter_center_scale);
	shutter_center->SetPos(System::instance()->GetWindowWidth() / 2 - shutter_center->GetW() / 2 * shutter_center_scale, System::instance()->GetWindowHeigh() / 2 - shutter_center->GetH() / 2 * shutter_center_scale);

	if (Animator::instance()->IsTimeUp("loading_exit"))
	{
//		shutter_top->SetPos(0, System::instance()->GetWindowHeigh());
//		shutter_bottom->SetPos(0, -848);
		Animator::instance()->ResetAnimation("loading_exit");
		is_exited = true;
	}
}

void lm::LoadingState::AddText(std::string text, std::string font_path, int font_size, int x, int y, char r, char g, char b, ShutterType type)
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

bool lm::LoadingState::IsSwitching()
{
	return !is_exited;
}