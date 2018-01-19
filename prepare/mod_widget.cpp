#include "mod_widget.h"
#include "../sprite.h"
#include "../button.h"
#include "../text_area.h"
#include "../animator.h"
#include "../system.h"
#include "../user/setting.h"
#include "prepare_state.h"
#include "../loading/loading_state.h"
#include "../control_handler.h"

fr::ModWidget *fr::ModWidget::m_instance = 0;

void fr::ModWidget::init()
{
	widget_base = new Sprite;

	auto_switch = new Button;
	slide_out_switch = new Button;
	duration_left = new Button;
	duration_left_dual = new Button;
	duration_right = new Button;
	duration_right_dual = new Button;
	offset_left = new Button;
	offset_left_dual = new Button;
	offset_right = new Button;
	offset_right_dual = new Button;
	gameplay_wizard_button = new Button;

	auto_text = new TextArea;
	slide_out_text = new TextArea;
	duration_text = new TextArea;
	offset_text = new TextArea;
	gameplay_wizard_text = new TextArea;
	duration_num = new TextArea;
	offset_num = new TextArea;

	widget_base->init("assets/base/widget_base.png");
	auto_switch->init("assets/base/sort_button.png");
	auto_switch->AddPressedFrame( "assets/base/sort_button_pressed.png");
	auto_switch->AddText(Setting::instance()->IsAuto() ? "ON" : "OFF", auto_switch->GetW() / 2, auto_switch->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
	slide_out_switch->init("assets/base/sort_button.png");
	slide_out_switch->AddPressedFrame( "assets/base/sort_button_pressed.png");
	slide_out_switch->AddText(Setting::instance()->IsSlideOut() ? "ON" : "OFF", slide_out_switch->GetW() / 2, slide_out_switch->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
	duration_left->init("assets/base/arrow_left.png");
	duration_left->AddPressedFrame("assets/base/arrow_left_pressed.png");
	duration_left_dual->init("assets/base/arrow_left_dual.png");
	duration_left_dual->AddPressedFrame("assets/base/arrow_left_dual_pressed.png");
	duration_right->init("assets/base/arrow_right.png");
	duration_right->AddPressedFrame("assets/base/arrow_right_pressed.png");
	duration_right_dual->init("assets/base/arrow_right_dual.png");
	duration_right_dual->AddPressedFrame("assets/base/arrow_right_dual_pressed.png");
	offset_left->init("assets/base/arrow_left.png");
	offset_left->AddPressedFrame("assets/base/arrow_left_pressed.png");
	offset_left_dual->init("assets/base/arrow_left_dual.png");
	offset_left_dual->AddPressedFrame("assets/base/arrow_left_dual_pressed.png");
	offset_right->init("assets/base/arrow_right.png");
	offset_right->AddPressedFrame("assets/base/arrow_right_pressed.png");
	offset_right_dual->init("assets/base/arrow_right_dual.png");
	offset_right_dual->AddPressedFrame("assets/base/arrow_right_dual_pressed.png");
	gameplay_wizard_button->init("assets/base/sort_button.png");
	gameplay_wizard_button->AddPressedFrame("assets/base/sort_button_pressed.png");
	gameplay_wizard_button->AddText("START", gameplay_wizard_button->GetW() / 2, gameplay_wizard_button->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);

	auto_text->init("AUTO", 0, 0, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);
	slide_out_text->init("SLIDE OUT", 0, 0, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);
	duration_text->init("DURATION", 0, 0, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);
	offset_text->init("OFFSET", 0, 0, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);
	gameplay_wizard_text->init("GAMEPLAY WIZARD", 0, 0, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);
	char *duration_ch = new char[4];
	char *offset_ch = new char[5];
	sprintf(duration_ch, "%d", Setting::instance()->GetDuration());
	sprintf(offset_ch, "%d", Setting::instance()->GetOffset());
	duration_num->init(duration_ch, 0, 0, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
	offset_num->init(offset_ch, 0, 0, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
	delete [] duration_ch;
	delete [] offset_ch;

	is_shown = false;
	is_entered = false;
	is_exited = true;
	Animator::instance()->AddAnimation("mod_enter", ANIMATIONTYPE_UNIFORMLY_DECELERATED, 300);
	Animator::instance()->AddAnimation("mod_exit", ANIMATIONTYPE_UNIFORMLY_ACCELERATED, 300);
}

void fr::ModWidget::clear()
{
	
}

void fr::ModWidget::update()
{
	if (is_shown && is_entered)
	{
		if (System::instance()->IsWindowModified())
		{
			widget_base->SetPos(System::instance()->GetWindowWidth() / 2 - widget_base->GetW() / 2, System::instance()->GetWindowHeigh() - widget_base->GetH());
			auto_switch->SetPos(widget_base->GetX() + 464, widget_base->GetY() + 32);
			slide_out_switch->SetPos(widget_base->GetX() + 464, widget_base->GetY() + 134);
			duration_left->SetPos(widget_base->GetX() + 430,  widget_base->GetY() + 258);
			duration_right->SetPos(widget_base->GetX() + 622, widget_base->GetY() + 258);
			duration_left_dual->SetPos(widget_base->GetX() + 382,  widget_base->GetY() + 258);
			duration_right_dual->SetPos(widget_base->GetX() + 660, widget_base->GetY() + 258);
			offset_left->SetPos(widget_base->GetX() + 430, widget_base->GetY() + 362);
			offset_right->SetPos(widget_base->GetX() + 622, widget_base->GetY() + 362);
			offset_left_dual->SetPos(widget_base->GetX() + 382, widget_base->GetY() + 362);
			offset_right_dual->SetPos(widget_base->GetX() + 660, widget_base->GetY() + 362);
			gameplay_wizard_button->SetPos(widget_base->GetX() + 464, widget_base->GetY() + 448);
			auto_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 50);
			slide_out_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 154);
			duration_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 258);
			offset_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 362);
			gameplay_wizard_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 466);
			duration_num->SetPos(widget_base->GetX() + 544, widget_base->GetY() + 276);
			offset_num->SetPos(widget_base->GetX() + 544, widget_base->GetY() + 380);
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
		slide_out_switch->update();
		duration_left->update();
		duration_right->update();
		duration_left_dual->update();
		duration_right_dual->update();
		offset_left->update();
		offset_right->update();
		offset_left_dual->update();
		offset_right_dual->update();
		gameplay_wizard_button->update();

		if (auto_switch->IsReleased())
		{
			Setting::instance()->SwitchAuto();
			auto_switch->ClearText();
			auto_switch->AddText(Setting::instance()->IsAuto() ? "ON" : "OFF", auto_switch->GetW() / 2, auto_switch->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
		}

		if (slide_out_switch->IsReleased())
		{
			Setting::instance()->SwitchSlideOut();
			slide_out_switch->ClearText();
			slide_out_switch->AddText(Setting::instance()->IsSlideOut() ? "ON" : "OFF", slide_out_switch->GetW() / 2, slide_out_switch->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
		}

		if (duration_left->IsReleased())
		{
			Setting::instance()->SetDuration(Setting::instance()->GetDuration() - 1);
			char *duration_ch = new char[4];
			sprintf(duration_ch, "%d", Setting::instance()->GetDuration());
			duration_num->SetText(duration_ch);
			delete [] duration_ch;
		}
		if (duration_right->IsReleased())
		{
			Setting::instance()->SetDuration(Setting::instance()->GetDuration() + 1);
			char *duration_ch = new char[4];
			sprintf(duration_ch, "%d", Setting::instance()->GetDuration());
			duration_num->SetText(duration_ch);
			delete [] duration_ch;
		}
		if (duration_left_dual->IsReleased())
		{
			Setting::instance()->SetDuration(Setting::instance()->GetDuration() - 10);
			char *duration_ch = new char[4];
			sprintf(duration_ch, "%d", Setting::instance()->GetDuration());
			duration_num->SetText(duration_ch);
			delete [] duration_ch;
		}
		if (duration_right_dual->IsReleased())
		{
			Setting::instance()->SetDuration(Setting::instance()->GetDuration() + 10);
			char *duration_ch = new char[4];
			sprintf(duration_ch, "%d", Setting::instance()->GetDuration());
			duration_num->SetText(duration_ch);
			delete [] duration_ch;
		}

		if (offset_left->IsReleased())
		{
			Setting::instance()->SetOffset(Setting::instance()->GetOffset() - 1);
			char *offset_ch = new char[4];
			sprintf(offset_ch, "%d", Setting::instance()->GetOffset());
			offset_num->SetText(offset_ch);
			delete [] offset_ch;
		}
		if (offset_right->IsReleased())
		{
			Setting::instance()->SetOffset(Setting::instance()->GetOffset() + 1);
			char *offset_ch = new char[4];
			sprintf(offset_ch, "%d", Setting::instance()->GetOffset());
			offset_num->SetText(offset_ch);
			delete [] offset_ch;
		}
		if (offset_left_dual->IsReleased())
		{
			Setting::instance()->SetOffset(Setting::instance()->GetOffset() - 10);
			char *offset_ch = new char[4];
			sprintf(offset_ch, "%d", Setting::instance()->GetOffset());
			offset_num->SetText(offset_ch);
			delete [] offset_ch;
		}
		if (offset_right_dual->IsReleased())
		{
			Setting::instance()->SetOffset(Setting::instance()->GetOffset() + 10);
			char *offset_ch = new char[4];
			sprintf(offset_ch, "%d", Setting::instance()->GetOffset());
			offset_num->SetText(offset_ch);
			delete [] offset_ch;
		}

		if (gameplay_wizard_button->IsReleased())
		{
			LoadingState::instance()->init(STATE_GAMEPLAY_WIZARD);
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

void fr::ModWidget::render()
{
	if (!is_exited)
	{
		widget_base->render();
		auto_text->render();
		slide_out_text->render();
		duration_text->render();
		offset_text->render();
		gameplay_wizard_text->render();
		auto_switch->render();
		slide_out_switch->render();
		duration_left->render();
		duration_right->render();
		duration_left_dual->render();
		duration_right_dual->render();
		offset_left->render();
		offset_right->render();
		offset_left_dual->render();
		offset_right_dual->render();
		gameplay_wizard_button->render();
		duration_num->render();
		offset_num->render();
	}
}

void fr::ModWidget::SwitchShown()
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

bool fr::ModWidget::IsShown()
{
	return is_shown || !is_exited;
}

void fr::ModWidget::OnEnter()
{
	int widget_x = -widget_base->GetW() + (System::instance()->GetWindowWidth() / 2 + widget_base->GetW() / 2) * Animator::instance()->GetProcess("mod_enter");
	widget_base->SetPos(widget_x, System::instance()->GetWindowHeigh() - widget_base->GetH());
	auto_switch->SetPos(widget_base->GetX() + 464, widget_base->GetY() + 32);
	slide_out_switch->SetPos(widget_base->GetX() + 464, widget_base->GetY() + 134);
	duration_left->SetPos(widget_base->GetX() + 430,  widget_base->GetY() + 258);
	duration_right->SetPos(widget_base->GetX() + 622, widget_base->GetY() + 258);
	duration_left_dual->SetPos(widget_base->GetX() + 382,  widget_base->GetY() + 258);
	duration_right_dual->SetPos(widget_base->GetX() + 660, widget_base->GetY() + 258);
	offset_left->SetPos(widget_base->GetX() + 430, widget_base->GetY() + 362);
	offset_right->SetPos(widget_base->GetX() + 622, widget_base->GetY() + 362);
	offset_left_dual->SetPos(widget_base->GetX() + 382, widget_base->GetY() + 362);
	offset_right_dual->SetPos(widget_base->GetX() + 660, widget_base->GetY() + 362);
	gameplay_wizard_button->SetPos(widget_base->GetX() + 464, widget_base->GetY() + 448);
	auto_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 50);
	slide_out_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 154);
	duration_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 258);
	offset_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 362);
	gameplay_wizard_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 466);
	duration_num->SetPos(widget_base->GetX() + 544, widget_base->GetY() + 276);
	offset_num->SetPos(widget_base->GetX() + 544, widget_base->GetY() + 380);
	if (Animator::instance()->IsTimeUp("mod_enter"))
	{
		Animator::instance()->ResetAnimation("mod_enter");
		is_entered = true;
	}
}

void fr::ModWidget::OnExit()
{
	int widget_x = System::instance()->GetWindowWidth() / 2 - widget_base->GetW() / 2 - (System::instance()->GetWindowWidth() / 2 + widget_base->GetW() / 2) * Animator::instance()->GetProcess("mod_exit");
	widget_base->SetPos(widget_x, System::instance()->GetWindowHeigh() - widget_base->GetH());
	auto_switch->SetPos(widget_base->GetX() + 464, widget_base->GetY() + 32);
	slide_out_switch->SetPos(widget_base->GetX() + 464, widget_base->GetY() + 134);
	duration_left->SetPos(widget_base->GetX() + 430,  widget_base->GetY() + 258);
	duration_right->SetPos(widget_base->GetX() + 622, widget_base->GetY() + 258);
	duration_left_dual->SetPos(widget_base->GetX() + 382,  widget_base->GetY() + 258);
	duration_right_dual->SetPos(widget_base->GetX() + 660, widget_base->GetY() + 258);
	offset_left->SetPos(widget_base->GetX() + 430, widget_base->GetY() + 362);
	offset_right->SetPos(widget_base->GetX() + 622, widget_base->GetY() + 362);
	offset_left_dual->SetPos(widget_base->GetX() + 382, widget_base->GetY() + 362);
	offset_right_dual->SetPos(widget_base->GetX() + 660, widget_base->GetY() + 362);
	gameplay_wizard_button->SetPos(widget_base->GetX() + 464, widget_base->GetY() + 448);
	auto_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 50);
	slide_out_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 154);
	duration_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 258);
	offset_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 362);
	gameplay_wizard_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 466);
	duration_num->SetPos(widget_base->GetX() + 544, widget_base->GetY() + 276);
	offset_num->SetPos(widget_base->GetX() + 544, widget_base->GetY() + 380);
	if (Animator::instance()->IsTimeUp("mod_exit"))
	{
		Animator::instance()->ResetAnimation("mod_exit");
		is_entered = false;
		is_exited = true;
	}
}