#include "loading_state.h"
#include "../system.h"
#include "../sprite.h"
#include "../text_area.h"
#include "../timer.h"
#include "../texture_manager.h"
#include "../sound_manager.h"

lm::LoadingState *lm::LoadingState::m_instance = 0;

void lm::LoadingState::update()
{
	if (!is_entered)
	{
		OnEnter();
		shutter_bottom->render();
		shutter_top->render();

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

		for (int i = 0; i < text_area_top.size(); i++)
		{
			text_area_top[i]->render(text_area_top[i]->GetX(), text_area_top[i]->GetY() + shutter_top->GetY());
		}
		for (int i = 0; i < text_area_bottom.size(); i++)
		{
			text_area_bottom[i]->render(text_area_bottom[i]->GetX(), text_area_bottom[i]->GetY() + shutter_bottom->GetY());
		}

		Timer::instance()->RunTimer("shutter");
		//SoundManager::instance()->play("assets/shutter_open.wav", SOUNDTYPE_SFX);
		//加載完立刻放音效
	}
	else if (!is_exited)
	{
		OnExit();
		shutter_bottom->render();
		shutter_top->render();

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
	shutter_top->load("assets/shutter_top.png", 0, 0, 720, 534);
	shutter_bottom->load("assets/shutter_bottom.png", 0, 0, 720, 848);
	TextureManager::instance()->loadfont("assets/Audiowide.ttf", 80);
	//SoundManager::instance()->load("assets/shutter_close.wav", SOUNDTYPE_SFX);
	//SoundManager::instance()->load("assets/shutter_open.wav", SOUNDTYPE_SFX);
	animate_duration = 500;
	//動畫持續時間
	a_top = 534.0 * 2 / ( animate_duration * animate_duration );
	a_bottom = 848.0 * 2 / ( animate_duration * animate_duration );
	//求出加速度
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
	Timer::instance()->RunTimer("shutter");
	TextArea *new_text_area = new TextArea;
	new_text_area->load("Loading...", "assets/Audiowide.ttf", 80, 0x00, 0x00, 0x00);
	new_text_area->SetPos(System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2);
	text_area_bottom.push_back(new_text_area);
	is_text_default = true;
	//SoundManager::instance()->play("assets/shutter_close.wav", SOUNDTYPE_SFX);
	//一開始就放音效
}

void lm::LoadingState::clear()
{
}

/*
先弄一點運動學公式出來
s = v0 * t + 0.5 * a * (t ^ 2)
  = (2 * v0 - a * t) / 2 * t
vt = v0 - at
v0 = aT
*/

void lm::LoadingState::OnEnter()
{
	int shutter_top_pos_y = System::instance()->GetWindowHeigh() - ((2 * a_top * animate_duration - a_top * Timer::instance()->GetTime("shutter")) * 0.5f *  Timer::instance()->GetTime("shutter"));
	int shutter_bottom_pos_y = -848 + ((2 * a_bottom * animate_duration - a_bottom * Timer::instance()->GetTime("shutter")) * 0.5f *  Timer::instance()->GetTime("shutter"));
	//求出某個時段的位移(感覺物理沒白學)
	shutter_top->SetPos(0, shutter_top_pos_y);
	shutter_bottom->SetPos(0, shutter_bottom_pos_y);
	if (Timer::instance()->GetTime("shutter") >= animate_duration)
	{
		shutter_top->SetPos(0, System::instance()->GetWindowHeigh() - 534);
		shutter_bottom->SetPos(0, 0);
		Timer::instance()->ResetTimer("shutter");
		is_entered = true;
	}
}

void lm::LoadingState::OnExit()
{
	int shutter_top_pos_y = System::instance()->GetWindowHeigh() - 534 + ((a_top * Timer::instance()->GetTime("shutter")) * 0.5f *  Timer::instance()->GetTime("shutter"));
	int shutter_bottom_pos_y = -((a_bottom * Timer::instance()->GetTime("shutter")) * 0.5f *  Timer::instance()->GetTime("shutter"));
	shutter_top->SetPos(0, shutter_top_pos_y);
	shutter_bottom->SetPos(0, shutter_bottom_pos_y);
	if (Timer::instance()->GetTime("shutter") >= animate_duration)
	{
		shutter_top->SetPos(0, System::instance()->GetWindowHeigh());
		shutter_bottom->SetPos(0, -848);
		Timer::instance()->ResetTimer("shutter");
		is_exited = true;
	}
}

void lm::LoadingState::AddText(std::string text, std::string font_path, int font_size, int x, int y, int r, int g, int b, ShutterType type)
{
	if (is_text_default)
	{
		delete text_area_bottom[0];
		text_area_bottom.clear();
		is_text_default = false;
	}
	TextArea *new_text_area = new TextArea;
	new_text_area->load(text, font_path, font_size, r, g, b);
	new_text_area->SetPos(x, y);
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