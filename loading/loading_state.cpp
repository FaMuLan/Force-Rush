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
	a_l = 1165.f * 2 / ( 500 ^ 2 );
	a_r = 459.f * 2 / ( 500 ^ 2 );
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
s = (v0 + vt) / 2 * t
  = (2v0 - at) / 2 * t
vt = v0 - at
*/

void lm::LoadingState::OnEnter()
{
	int shutter_l_pos_x = -1165 + ((2 * a_l * 500 - a_l * Timer::instance()->GetTime("shutter")) * 0.5f *  Timer::instance()->GetTime("shutter"));
	int shutter_r_pos_x = System::instance()->GetWindowWidth() - ((2 * a_r * 500 - a_r * Timer::instance()->GetTime("shutter")) * 0.5f *  Timer::instance()->GetTime("shutter"));
	shutter_l->SetPos(shutter_l_pos_x, 0);
	shutter_r->SetPos(shutter_r_pos_x, 0);
	if (Timer::instance()->GetTime("shutter") >= 500)
	{
		shutter_l->SetPos(0, 0);
		shutter_r->SetPos(System::instance()->GetWindowWidth() - 459, 0);
		Timer::instance()->ResetTimer("shutter");
		is_entered = true;
	}
}

void lm::LoadingState::OnExit()
{
	int shutter_l_pos_x = - a_l * (500 + Timer::instance()->GetTime("shutter")) * 0.5f *  Timer::instance()->GetTime("shutter");
	int shutter_r_pos_x = System::instance()->GetWindowWidth() + a_r * (500 + Timer::instance()->GetTime("shutter")) * 0.5f *  Timer::instance()->GetTime("shutter");
	shutter_l->SetPos(shutter_l_pos_x, 0);
	shutter_r->SetPos(shutter_r_pos_x, 0);
	if (Timer::instance()->GetTime("shutter") >= 500)
	{
		shutter_l->SetPos(System::instance()->GetWindowWidth(), 0);
		shutter_r->SetPos(System::instance()->GetWindowWidth(), 0);
		Timer::instance()->ResetTimer("shutter");
		is_exited = true;
	}
}