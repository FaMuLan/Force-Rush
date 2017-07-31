#include "about_sidedialog.h"

lm::AboutSidedialog *lm::AboutSidedialog::m_instance = 0;

void lm::AboutSidedialog::init()
{
	sidedialog_bottom = new Sprite;
	sidedialog_top = new Sprite;

	sidedialog_bottom->load("assets/sidedialog_bottom.png", -219, 0, 219, 541);
	sidedialog_top->load("assets/sidedialog_top.png", -756, 0, 756, 964);
	is_entered = true;
	ready_to_exit = true;
	is_exited = true;
	animate_duration = 500;
	a_bottom = 890.0 * 2 / (animate_duration * animate_duration);
	a_top = 756.0 * 2 / (animate_duration * animate_duration);
}

void lm::AboutSidedialog::clear()
{
	sidedialog_bottom->clear();
	sidedialog_top->clear();
}

void lm::AboutSidedialog::update()
{
	if (!is_entered)
	{
		MainState::instance()->lock(true);
		//上鎖
		int sidedialog_bottom_pos_x = -400 + ((2 * a_bottom * animate_duration - a_bottom * Timer::instance()->GetTime("sidedialog")) * 0.5f *  Timer::instance()->GetTime("sidedialog"));
		int sidedialog_top_pos_x = -756 + ((2 * a_top * animate_duration - a_top * Timer::instance()->GetTime("sidedialog")) * 0.5f *  Timer::instance()->GetTime("sidedialog"));
		sidedialog_bottom->SetPos(sidedialog_bottom_pos_x, 0);
		sidedialog_top->SetPos(sidedialog_top_pos_x, 0);
		if (Timer::instance()->GetTime("sidedialog") >= animate_duration)
		{
			sidedialog_bottom->SetPos(490, 0);
			sidedialog_top->SetPos(0, 0);
			Timer::instance()->ResetTimer("sidedialog");
			is_entered = true;
		}
	}
	else if (!ready_to_exit)
	{
		for (int i = 0; i < ControlHandler::instance()->GetFingerCount(); i++)
		{
			Finger load_finger = ControlHandler::instance()->GetFinger(i);
			if (!load_finger.moved)
			{
				if (load_finger.x >= 500)
				{
					ready_to_exit = true;
					Timer::instance()->RunTimer("sidedialog");
				}
			}
		}
	}
	else if (!is_exited)
	{
		int sidedialog_bottom_pos_x = 490 - ((a_bottom * Timer::instance()->GetTime("sidedialog")) * 0.5f *  Timer::instance()->GetTime("sidedialog"));
		int sidedialog_top_pos_x = -((a_top * Timer::instance()->GetTime("sidedialog")) * 0.5f *  Timer::instance()->GetTime("sidedialog"));
		sidedialog_bottom->SetPos(sidedialog_bottom_pos_x, 0);
		sidedialog_top->SetPos(sidedialog_top_pos_x, 0);
		if (Timer::instance()->GetTime("sidedialog") >= animate_duration)
		{
			sidedialog_top->SetPos(System::instance()->GetWindowWidth(), 0);
			sidedialog_bottom->SetPos(System::instance()->GetWindowWidth(), 0);
			Timer::instance()->ResetTimer("sidedialog");
			is_exited = true;
			MainState::instance()->lock(false);
			//解鎖
		}
	}
}	//void lm::AboutSidedialog::update()

void lm::AboutSidedialog::render()
{
	if (!(is_entered && ready_to_exit && is_exited))
	{
		sidedialog_bottom->render();
		sidedialog_top->render();
		TextureManager::instance()->render("Program: FaMuLan", sidedialog_bottom->GetX() - 200, 300, "assets/GeosansLight-Oblique.ttf", 0xFF, 0xFF, 0xFF);
		TextureManager::instance()->render("Source: Arcaea", sidedialog_bottom->GetX() - 200, 400, "assets/GeosansLight-Oblique.ttf", 0xFF, 0xFF, 0xFF);
	}
}

void lm::AboutSidedialog::open()
{
	is_entered = false;
	ready_to_exit = false;
	is_exited = false;
	Timer::instance()->RunTimer("sidedialog");
}