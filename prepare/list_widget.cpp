#include "list_widget.h"
#include "../sprite.h"
#include "../button.h"
#include "../text_area.h"
#include "../animator.h"
#include "../system.h"
#include "../user/setting.h"
#include "../control_handler.h"
#include "song_list.h"

fr::ListWidget *fr::ListWidget::m_instance = 0;

void fr::ListWidget::init()
{
	widget_base = new Sprite;

	sort_type_label = new TextArea;
	sort_type_left = new Button;
	sort_type_right = new Button;
	sort_type_text = new TextArea;
	reverse_label = new TextArea;
	reverse_switch = new Button;

	widget_base->init("assets/base/widget_base.png");

	sort_type_label->init("SORT", 0, 0, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);
	sort_type_left->init("assets/base/arrow_left.png");
	sort_type_left->AddPressedFrame("assets/base/arrow_left_pressed.png");
	sort_type_right->init("assets/base/arrow_right.png");
	sort_type_right->AddPressedFrame("assets/base/arrow_right_pressed.png");
	switch (SongList::instance()->GetSortType())
	{
		case SORTTYPE_DEFAULT:
			sort_type_text->init("DEFAULT", 0, 0, "assets/fonts/Audiowide.ttf", 24, 0x00, 0x00, 0x00);
		break;
		case SORTTYPE_ARTIST:
			sort_type_text->init("ARTIST", 0, 0, "assets/fonts/Audiowide.ttf", 24, 0x00, 0x00, 0x00);
		break;
		case SORTTYPE_NOTER:
			sort_type_text->init("NOTER", 0, 0, "assets/fonts/Audiowide.ttf", 24, 0x00, 0x00, 0x00);
		break;
		case SORTTYPE_TITLE:
			sort_type_text->init("TITLE", 0, 0, "assets/fonts/Audiowide.ttf", 24, 0x00, 0x00, 0x00);
		break;
		case SORTTYPE_DIFFICULTY:
			sort_type_text->init("DIFFICULTY", 0, 0, "assets/fonts/Audiowide.ttf", 24, 0x00, 0x00, 0x00);
		break;
		case SORTTYPE_DURATION:
			sort_type_text->init("LENGTH", 0, 0, "assets/fonts/Audiowide.ttf", 24, 0x00, 0x00, 0x00);
		break;
	}
	reverse_label->init("REVERSE", 0, 0, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);
	reverse_switch->init("assets/base/sort_button.png");
	reverse_switch->AddPressedFrame("assets/base/sort_button_pressed.png");
	reverse_switch->AddText(SongList::instance()->IsReverse() ? "ON" : "OFF", reverse_switch->GetW() / 2, reverse_switch->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
	

	is_shown = false;
	is_entered = false;
	is_exited = true;
	Animator::instance()->AddAnimation("list_enter", ANIMATIONTYPE_UNIFORMLY_DECELERATED, 300);
	Animator::instance()->AddAnimation("list_exit", ANIMATIONTYPE_UNIFORMLY_ACCELERATED, 300);
}

void fr::ListWidget::clear()
{
	
}

void fr::ListWidget::update()
{
	if (is_shown && is_entered)
	{
		if (System::instance()->IsWindowModified())
		{
			widget_base->SetPos(System::instance()->GetWindowWidth() / 2 - widget_base->GetW() / 2, System::instance()->GetWindowHeigh() - widget_base->GetH());
			sort_type_label->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 50);
			sort_type_left->SetPos(widget_base->GetX() + 430,  widget_base->GetY() + 50);
			sort_type_right->SetPos(widget_base->GetX() + 622, widget_base->GetY() + 50);
			sort_type_text->SetPos(widget_base->GetX() + 544, widget_base->GetY() + 68);
			reverse_label->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 154);
			reverse_switch->SetPos(widget_base->GetX() + 464, widget_base->GetY() + 136);
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
		if (ControlHandler::instance()->IsKeyDown(SDL_SCANCODE_AC_BACK))
		{
			SwitchShown();
		}

		sort_type_left->update();
		sort_type_right->update();
		reverse_switch->update();

		if (sort_type_left->IsReleased())
		{
			int sort_type_num = SongList::instance()->GetSortType();
			sort_type_num = sort_type_num - 1 < 0 ? 5 : sort_type_num - 1;
			SongList::instance()->SwitchSort(SortType(sort_type_num), SongList::instance()->IsReverse());
			switch (SongList::instance()->GetSortType())
			{
				case SORTTYPE_DEFAULT:
					sort_type_text->SetText("DEFAULT");
				break;
				case SORTTYPE_ARTIST:
					sort_type_text->SetText("ARTIST");
				break;
				case SORTTYPE_NOTER:
					sort_type_text->SetText("NOTER");
				break;
				case SORTTYPE_TITLE:
					sort_type_text->SetText("TITLE");
				break;
				case SORTTYPE_DIFFICULTY:
					sort_type_text->SetText("DIFFICULTY");
				break;
				case SORTTYPE_DURATION:
					sort_type_text->SetText("LENGTH");
				break;
			}
		}
		if (sort_type_right->IsReleased())
		{
			int sort_type_num = SongList::instance()->GetSortType();
			sort_type_num = sort_type_num + 1 > 5 ? 0 : sort_type_num + 1;
			SongList::instance()->SwitchSort(SortType(sort_type_num), SongList::instance()->IsReverse());
			switch (SongList::instance()->GetSortType())
			{
				case SORTTYPE_DEFAULT:
					sort_type_text->SetText("DEFAULT");
				break;
				case SORTTYPE_ARTIST:
					sort_type_text->SetText("ARTIST");
				break;
				case SORTTYPE_NOTER:
					sort_type_text->SetText("NOTER");
				break;
				case SORTTYPE_TITLE:
					sort_type_text->SetText("TITLE");
				break;
				case SORTTYPE_DIFFICULTY:
					sort_type_text->SetText("DIFFICULTY");
				break;
				case SORTTYPE_DURATION:
					sort_type_text->SetText("LENGTH");
				break;
			}
		}
		if (reverse_switch->IsReleased())
		{
			SongList::instance()->SwitchSort(SongList::instance()->GetSortType(), !SongList::instance()->IsReverse());
			reverse_switch->ClearText();
			reverse_switch->AddText(SongList::instance()->IsReverse() ? "ON" : "OFF", reverse_switch->GetW() / 2, reverse_switch->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
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

void fr::ListWidget::render()
{
	if (!is_exited)
	{
		widget_base->render();
		sort_type_label->render();
		sort_type_left->render();
		sort_type_right->render();
		sort_type_text->render();
		reverse_label->render();
		reverse_switch->render();
	}
}

void fr::ListWidget::SwitchShown()
{
	is_shown = !is_shown;
	if (is_shown)
	{
		Animator::instance()->Animate("list_enter");
		is_exited = false;
	}
	else
	{
		Animator::instance()->Animate("list_exit");
	}
}

bool fr::ListWidget::IsShown()
{
	return is_shown || !is_exited;
}

void fr::ListWidget::OnEnter()
{
	int widget_x = System::instance()->GetWindowWidth() - (System::instance()->GetWindowWidth() / 2 + widget_base->GetW() / 2) * Animator::instance()->GetProcess("list_enter");
	widget_base->SetPos(widget_x, System::instance()->GetWindowHeigh() - widget_base->GetH());
	sort_type_label->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 50);
	sort_type_left->SetPos(widget_base->GetX() + 430,  widget_base->GetY() + 50);
	sort_type_right->SetPos(widget_base->GetX() + 622, widget_base->GetY() + 50);
	sort_type_text->SetPos(widget_base->GetX() + 544, widget_base->GetY() + 68);
	reverse_label->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 154);
	reverse_switch->SetPos(widget_base->GetX() + 464, widget_base->GetY() + 136);
	if (Animator::instance()->IsTimeUp("list_enter"))
	{
		Animator::instance()->ResetAnimation("list_enter");
		is_entered = true;
	}
}

void fr::ListWidget::OnExit()
{
	int widget_x = System::instance()->GetWindowWidth() / 2 - widget_base->GetW() / 2 + (System::instance()->GetWindowWidth() / 2 + widget_base->GetW() / 2) * Animator::instance()->GetProcess("list_exit");
	widget_base->SetPos(widget_x, System::instance()->GetWindowHeigh() - widget_base->GetH());
	sort_type_label->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 50);
	sort_type_left->SetPos(widget_base->GetX() + 430,  widget_base->GetY() + 50);
	sort_type_right->SetPos(widget_base->GetX() + 622, widget_base->GetY() + 50);
	sort_type_text->SetPos(widget_base->GetX() + 544, widget_base->GetY() + 68);
	reverse_label->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 154);
	reverse_switch->SetPos(widget_base->GetX() + 464, widget_base->GetY() + 136);
	if (Animator::instance()->IsTimeUp("list_exit"))
	{
		Animator::instance()->ResetAnimation("list_exit");
		is_entered = false;
		is_exited = true;
	}
}