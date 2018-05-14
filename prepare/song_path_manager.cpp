#include "../gui/button.h"
#include "../sprite.h"
#include "../system.h"
#include "../animator.h"
#include "../control_handler.h"
#include "../user/setting.h"
#include "../tools/file_explorer.h"
#include "song_path_manager.h"


fr::SongPathManagerWidget *fr::SongPathManagerWidget::m_instance = 0;
fr::SongPathList *fr::SongPathList::m_instance = 0;

void fr::SongPathManagerWidget::init()
{
	widget_base = new Sprite;
	new_path = new Button;
	delete_path = new Button;
	widget_base->init("assets/base/widget_base.png");
	new_path->init("assets/base/sort_button.png");
	new_path->AddPressedFrame("assets/base/sort_button_pressed.png");
	new_path->AddText("NEW", new_path->GetW() / 2, new_path->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
	delete_path->init("assets/base/sort_button.png");
	delete_path->AddPressedFrame("assets/base/sort_button_pressed.png");
	delete_path->AddText("DEL", delete_path->GetW() / 2, delete_path->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
	SongPathList::instance()->init();
	SongPathList::instance()->SetPos(widget_base->GetX() + 192, widget_base->GetY() + 32);

	is_shown = false;
	is_entered = false;
	is_exited = true;
	Animator::instance()->AddAnimation("song_path_manager_enter", ANIMATIONTYPE_UNIFORMLY_DECELERATED, 300);
	Animator::instance()->AddAnimation("song_path_manager_exit", ANIMATIONTYPE_UNIFORMLY_ACCELERATED, 300);
}

void fr::SongPathManagerWidget::clear()
{
	
}

void fr::SongPathManagerWidget::update()
{
	if (is_shown && is_entered)
	{
		if (System::instance()->IsWindowModified())
		{
			widget_base->SetPos(System::instance()->GetWindowWidth() / 2 - widget_base->GetW() / 2, System::instance()->GetWindowHeigh() - widget_base->GetH());
			new_path->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 32);
			delete_path->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 104);
			SongPathList::instance()->SetPos(widget_base->GetX() + 192, widget_base->GetY() + 32);
		}
		for (int i = 0; i < ControlHandler::instance()->GetFingerCount(); i++)
		{
			Finger load_finger = ControlHandler::instance()->GetFinger(i);
			if (!load_finger.moved && !FileExplorerWidget::instance()->IsShown())
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

		new_path->update();
		delete_path->update();
		SongPathList::instance()->update();

		if (new_path->IsReleased() && !FileExplorerWidget::instance()->IsShown())
		{
			FileExplorerWidget::instance()->SwitchShown();
		}
		if (delete_path->IsReleased() && !FileExplorerWidget::instance()->IsShown())
		{
			FileExplorerWidget::instance()->SwitchShown();
		}
	}
	else if (is_shown)
	{
		OnEnter();
		SongPathList::instance()->update();
	}
	else if (!is_shown && !is_exited)
	{
		OnExit();
		SongPathList::instance()->update();
	}
}

void fr::SongPathManagerWidget::render()
{
	if (!is_exited)
	{
		widget_base->render();
		new_path->render();
		delete_path->render();
		SongPathList::instance()->render();
	}
}

void fr::SongPathManagerWidget::SwitchShown()
{
	is_shown = !is_shown;
	if (is_shown)
	{
		Animator::instance()->Animate("song_path_manager_enter");
		is_exited = false;
	}
	else
	{
		Animator::instance()->Animate("song_path_manager_exit");
	}
}

bool fr::SongPathManagerWidget::IsShown()
{
	return is_shown || !is_exited;
}


void fr::SongPathManagerWidget::OnEnter()
{
	int widget_x = System::instance()->GetWindowWidth() - (System::instance()->GetWindowWidth() / 2 + widget_base->GetW() / 2) * Animator::instance()->GetProcess("song_path_manager_enter");
	widget_base->SetPos(widget_x, System::instance()->GetWindowHeigh() - widget_base->GetH());
	new_path->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 32);
	delete_path->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 104);
	SongPathList::instance()->SetPos(widget_base->GetX() + 192, widget_base->GetY() + 32);

	if (Animator::instance()->IsTimeUp("song_path_manager_enter"))
	{
		Animator::instance()->ResetAnimation("song_path_manager_enter");
		is_entered = true;
	}
}

void fr::SongPathManagerWidget::OnExit()
{
	int widget_x = System::instance()->GetWindowWidth() / 2 - widget_base->GetW() / 2 + (System::instance()->GetWindowWidth() / 2 + widget_base->GetW() / 2) * Animator::instance()->GetProcess("song_path_manager_exit");
	widget_base->SetPos(widget_x, System::instance()->GetWindowHeigh() - widget_base->GetH());
	new_path->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 32);
	delete_path->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 104);
	SongPathList::instance()->SetPos(widget_base->GetX() + 192, widget_base->GetY() + 32);

	if (Animator::instance()->IsTimeUp("song_path_manager_exit"))
	{
		Animator::instance()->ResetAnimation("song_path_manager_exit");
		is_entered = false;
		is_exited = true;
	}
}

void fr::SongPathList::init()
{
	cell_base_path = "assets/prepare/manage_path_cell.png";
	cell_base_pressed_path = "assets/prepare/manage_path_cell_pressed.png";
	cell_base_selected_path = "assets/prepare/manage_path_cell_selected.png";
	Setting::instance()->GetSongList(list_path);
	List::init(Rect(192, 32, 496, 496), list_path.size());
	RefreshListSize();
}

void fr::SongPathList::clear()
{
	
}

void fr::SongPathList::update()
{
	List::update();
	int current_index = current_list_process / cell_h;
	for (int i = 0; i < cell.size(); i++)
	{
		if (current_index >= value_count)
		{
			current_index = 0;
		}

		if (current_index == selected_index)
		{
			cell[i]->GetText(0)->SetText(list_path[current_index]);
			cell[i]->GetText(0)->SetColor(0xFF, 0xFF, 0xFF);
		}
		else
		{
			cell[i]->GetText(0)->SetText(list_path[current_index]);
			cell[i]->GetText(0)->SetColor(0x00, 0x00, 0x00);
		}
		current_index++;
	}
}

void fr::SongPathList::render()
{
	List::render();
}

void fr::SongPathList::RefreshListSize()
{
	List::RefreshListSize();
	for (int i = 0; i < cell.size(); i++)
	{
		cell[i]->AddText("??", 24, 20, "assets/fonts/Ubuntu-M.ttf", 24, 0xFF, 0xFF, 0xFF, TEXTFORMAT_LEFT, 448);
	}
}