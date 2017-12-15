#include "mod_widget.h"
#include "../sprite.h"
#include "../button.h"
#include "../text_area.h"
#include "../animator.h"
#include "../system.h"
#include "../user/setting.h"
#include "../control_handler.h"

lm::ModWidget *lm::ModWidget::m_instance = 0;

void lm::ModWidget::init()
{
	widget_base = new Sprite;
	auto_switch = new Button;
	auto_text = new TextArea;
	widget_base->init("assets/select/mod_widget.png");
	auto_switch->init("assets/base/sort_button.png");
	auto_switch->AddPressedFrame( "assets/base/sort_button_pressed.png");
	auto_switch->AddText(Setting::instance()->IsAuto() ? "ON" : "OFF", auto_switch->GetW() / 2, auto_switch->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
	auto_text->init("AUTO", 0, 0, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);
	is_shown = false;
	is_entered = false;
	is_exited = true;
	Animator::instance()->AddAnimation("mod_enter", ANIMATIONTYPE_UNIFORMLY_DECELERATED, 300);
	Animator::instance()->AddAnimation("mod_exit", ANIMATIONTYPE_UNIFORMLY_ACCELERATED, 300);
}

void lm::ModWidget::clear()
{
	
}

void lm::ModWidget::update()
{
	if (is_shown && is_entered)
	{
		if (System::instance()->IsWindowModified())
		{
			widget_base->SetPos(System::instance()->GetWindowWidth() / 2 - widget_base->GetW() / 2, System::instance()->GetWindowHeigh() - widget_base->GetH());
			auto_switch->SetPos(widget_base->GetX() + 528, widget_base->GetY() + 32);
			auto_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 50);
		}
		for (int i = 0; i < ControlHandler::instance()->GetFingerCount(); i++)
		{
			Finger load_finger = ControlHandler::instance()->GetFinger(i);
			if (!load_finger.moved)
			{
				if (load_finger.x < System::instance()->GetWindowWidth() / 2 - widget_base->GetW() / 2 || load_finger.x > System::instance()->GetWindowWidth() / 2 + widget_base->GetW() / 2 || load_finger.y < System::instance()->GetWindowHeigh() - widget_base->GetH())
				{
					SwitchShown();
				}
			}
		}
		auto_switch->update();

		if (auto_switch->IsReleased())
		{
			Setting::instance()->SwitchAuto();
			auto_switch->ClearText();
			auto_switch->AddText(Setting::instance()->IsAuto() ? "ON" : "OFF", auto_switch->GetW() / 2, auto_switch->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
		}
	}
	else if (is_shown)
	{
		OnEnter();
	}
	else if (!is_shown && !is_exited)
	{
		OnExit();
	}
}

void lm::ModWidget::render()
{
	if (!is_exited)
	{
		widget_base->render();
		auto_text->render();
		auto_switch->render();
	}
}

void lm::ModWidget::SwitchShown()
{
	is_shown = !is_shown;
	if (is_shown)
	{
		Animator::instance()->Animate("mod_enter");
		is_exited = false;
	}
	else
	{
		Animator::instance()->Animate("mod_exit");
	}
}

bool lm::ModWidget::IsShown()
{
	return is_shown;
}

void lm::ModWidget::OnEnter()
{
	int widget_x = -widget_base->GetW() + (System::instance()->GetWindowWidth() / 2 + widget_base->GetW() / 2) * Animator::instance()->GetProcess("mod_enter");
	widget_base->SetPos(widget_x, System::instance()->GetWindowHeigh() - widget_base->GetH());
	auto_switch->SetPos(widget_base->GetX() + 528, widget_base->GetY() + 32);
	auto_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 50);
	if (Animator::instance()->IsTimeUp("mod_enter"))
	{
		Animator::instance()->ResetAnimation("mod_enter");
		is_entered = true;
	}
}

void lm::ModWidget::OnExit()
{
	int widget_x = System::instance()->GetWindowWidth() / 2 - widget_base->GetW() / 2 - (System::instance()->GetWindowWidth() / 2 + widget_base->GetW() / 2) * Animator::instance()->GetProcess("mod_exit");
	widget_base->SetPos(widget_x, System::instance()->GetWindowHeigh() - widget_base->GetH());
	auto_switch->SetPos(widget_base->GetX() + 528, widget_base->GetY() + 32);
	auto_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 50);
	if (Animator::instance()->IsTimeUp("mod_exit"))
	{
		Animator::instance()->ResetAnimation("mod_exit");
		is_entered = false;
		is_exited = true;
	}
}