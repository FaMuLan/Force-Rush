#include "loading_state.h"

lm::LoadingState *lm::LoadingState::m_instance = 0;

void lm::LoadingState::update()
{
	if (!is_entered)
	{
		OnEnter();
		shutter_l->render();
		shutter_r->render();
	}
	else if (!is_loaded)
	{
		last_state->clear();
		next_state->init();
		System::instance()->current_state = next_state;
		is_loaded = true;
		shutter_l->render();
		shutter_r->render();
		Timer::instance()->RunTimer("shutter");
	}
	else if (!is_exited)
	{
		OnExit();
		shutter_l->render();
		shutter_r->render();
	}
}

void lm::LoadingState::init()
{
	shutter_l = new Sprite;
	shutter_r = new Sprite;
	shutter_l->load("assets/shutter_l.png", 0, 0, 1165, 960);
	shutter_r->load("assets/shutter_r.png", 0, 0, 459, 960);
	animate_duration = 1000;
	//動畫持續時間
	a_l = 1165.0 * 2 / ( animate_duration * animate_duration );
	a_r = 459.0 * 2 / ( animate_duration * animate_duration );
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
	long shutter_l_pos_x = -1165 + ((2 * a_l * animate_duration - a_l * Timer::instance()->GetTime("shutter")) * 0.5f *  Timer::instance()->GetTime("shutter"));
	long shutter_r_pos_x = System::instance()->GetWindowWidth() - ((2 * a_r * animate_duration - a_r * Timer::instance()->GetTime("shutter")) * 0.5f *  Timer::instance()->GetTime("shutter"));
	//求出某個時段的位移(感覺物理沒白學)
	shutter_l->SetPos(shutter_l_pos_x, 0);
	shutter_r->SetPos(shutter_r_pos_x, 0);
	if (Timer::instance()->GetTime("shutter") >= animate_duration)
	{
		shutter_l->SetPos(0, 0);
		shutter_r->SetPos(System::instance()->GetWindowWidth() - 459, 0);
		Timer::instance()->ResetTimer("shutter");
		is_entered = true;
	}
}

void lm::LoadingState::OnExit()
{
	int shutter_l_pos_x = -((a_l * Timer::instance()->GetTime("shutter")) * 0.5f *  Timer::instance()->GetTime("shutter"));
	int shutter_r_pos_x = System::instance()->GetWindowWidth() - 459 + ((a_r * Timer::instance()->GetTime("shutter")) * 0.5f *  Timer::instance()->GetTime("shutter"));
	shutter_l->SetPos(shutter_l_pos_x, 0);
	shutter_r->SetPos(shutter_r_pos_x, 0);
	if (Timer::instance()->GetTime("shutter") >= animate_duration)
	{
		shutter_l->SetPos(System::instance()->GetWindowWidth(), 0);
		shutter_r->SetPos(System::instance()->GetWindowWidth(), 0);
		Timer::instance()->ResetTimer("shutter");
		is_exited = true;
	}
}