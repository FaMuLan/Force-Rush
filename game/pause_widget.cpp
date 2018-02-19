#include "pause_widget.h"
#include "../sprite.h"
#include "../animator.h"
#include "../system.h"

fr::PauseWidget *fr::PauseWidget::m_instance = 0;

void fr::PauseWidget::init()
{
	widget_base = new Sprite;
	widget_base->init("assets/game/pause_widget_base.png");
	Animator::instance()->AddAnimation("pause_widget_enter", ANIMATIONTYPE_UNIFORMLY_DECELERATED, 300);
	Animator::instance()->AddAnimation("pause_widget_exit", ANIMATIONTYPE_UNIFORMLY_ACCELERATED, 300);
	is_shown = false;
	is_entered = false;
	is_exited = true;
}

void fr::PauseWidget::clear()
{
	
}

void fr::PauseWidget::update()
{
	if (is_shown && is_entered)
	{
		if (System::instance()->IsWindowModified())
		{
			widget_base->SetPos(System::instance()->GetWindowWidth() / 2 - widget_base->GetW() / 2, 0);
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

void fr::PauseWidget::render()
{
	if (!is_exited)
	{
		widget_base->render();
	}
}

void fr::PauseWidget::OnEnter()
{
	int widget_y =  -widget_base->GetH() + widget_base->GetH() * Animator::instance()->GetProcess("pause_widget_enter");
	widget_base->SetPos(System::instance()->GetWindowWidth() / 2 - widget_base->GetW() / 2, widget_y);
	if (Animator::instance()->IsTimeUp("pause_widget_enter"))
	{		Animator::instance()->ResetAnimation("pause_widget_enter");
		is_entered = true;
	}
}

void fr::PauseWidget::OnExit()
{
	int widget_y = -widget_base->GetH() * Animator::instance()->GetProcess("pause_widget_exit");
	widget_base->SetPos(System::instance()->GetWindowWidth() / 2 - widget_base->GetW() / 2, widget_y);
	if (Animator::instance()->IsTimeUp("pause_widget_exit"))
	{
		Animator::instance()->ResetAnimation("pause_widget_exit");
		is_entered = false;
		is_exited = true;
	}
}

void fr::PauseWidget::SwitchShown()
{
	is_shown = !is_shown;
	if (is_shown)
	{
		Animator::instance()->Animate("pause_widget_enter");
		is_exited = false;
	}
	else
	{
		Animator::instance()->Animate("pause_widget_exit");
	}
}

bool fr::PauseWidget::IsShown()
{
	return is_shown;
}