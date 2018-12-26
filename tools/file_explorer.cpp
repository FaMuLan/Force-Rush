#include "../gui/text_area.h"
#include "../gui/button.h"
#include "../sprite.h"
#include "../system.h"
#include "../animator.h"
#include "../control_handler.h"
#include "../file_system.h"
#include <regex>
#include "file_explorer.h"

fr::FileExplorerWidget *fr::FileExplorerWidget::m_instance = 0;
fr::FileList *fr::FileList::m_instance = 0;

void fr::FileExplorerWidget::init()
{
	widget_base_head = new Sprite;
	widget_base_body = new Sprite;
	widget_base_foot = new Sprite;
	confirm = new Button;
	current_path = new TextArea;
	parent_dir = new Button;
	widget_base_head->init("assets/base/long_widget_head.png");
	widget_base_body->init("assets/base/long_widget_body.png");
	widget_base_foot->init("assets/base/long_widget_foot.png");
	confirm->init("assets/base/widget_button_single.png");
	confirm->AddPressedFrame("assets/base/widget_button_single_pressed.png");
	confirm->AddText("CONFIRM", Point2Di(confirm->GetW() / 2, confirm->GetH() / 2), "assets/fonts/Audiowide.ttf", 30, Color(0x00, 0x00, 0x00));
	current_path->init("NULL", Point2Di(widget_base_head->GetX() + 120, widget_base_head->GetY() + 52), "assets/fonts/Ubuntu-M.ttf", 24, Color(0x00, 0x00, 0x00), TEXTFORMAT_LEFT, 544);
	parent_dir->init("assets/base/arrow_back.png");
	parent_dir->AddPressedFrame("assets/base/arrow_back_pressed.png");
	body_count = (System::instance()->GetWindowHeigh() - 560) / 140;
	widget_h = 560 + 140 * body_count;
	widget_y = (System::instance()->GetWindowHeigh() / 2) - (widget_h) / 2;
	FileList::instance()->init();
	FileList::instance()->SetPos(widget_base_head->GetX() + 32, widget_base_head->GetY() + 96);
	FileList::instance()->SetSize(656, widget_h - 176);
	FileList::instance()->RefreshListSize();

	is_shown = false;
	is_entered = false;
	is_exited = true;
	Animator::instance()->AddAnimation("file_explorer_enter", ANIMATIONTYPE_UNIFORMLY_DECELERATED, 300);
	Animator::instance()->AddAnimation("file_explorer_exit", ANIMATIONTYPE_UNIFORMLY_ACCELERATED, 300);
}

void fr::FileExplorerWidget::clear()
{
	
}

void fr::FileExplorerWidget::update()
{
	is_outputed = false;
	if (is_shown && is_entered)
	{
		if (System::instance()->IsWindowModified())
		{
			body_count = (System::instance()->GetWindowHeigh() - 560) / 140;
			widget_y = (System::instance()->GetWindowHeigh() / 2) - (280 + 70 * body_count);
			widget_base_head->SetPos(System::instance()->GetWindowWidth() / 2 - widget_base_head->GetW() / 2, widget_y);
			widget_base_foot->SetPos(widget_base_head->GetX(), widget_y + 280 + body_count * 140);
			FileList::instance()->SetPos(widget_base_head->GetX() + 32, widget_base_head->GetY() + 96);
			FileList::instance()->SetSize(656, widget_h - 176);
			FileList::instance()->RefreshListSize();
			confirm->SetPos(widget_base_foot->GetX() + 8, widget_base_foot->GetY() + 224);
			parent_dir->SetPos(widget_base_head->GetX() + 32, widget_base_head->GetY() + 32);
			current_path->SetPos(widget_base_head->GetX() + 120, widget_base_head->GetY() + 52);
		}
		if (ControlHandler::instance()->IsKeyDown(SDL_SCANCODE_AC_BACK))
		{
			SwitchShown();
		}

		confirm->update();
		parent_dir->update();
		FileList::instance()->update();

		if (parent_dir->IsReleased())
		{
			FileList::instance()->PathBack();
		}
		if (confirm->IsReleased())
		{
			is_outputed = true;
			SwitchShown();
		}
		current_path->SetText(FileList::instance()->GetCurrentPath());
	}
	else if (is_shown)
	{
		OnEnter();
		FileList::instance()->update();
	}
	else if (!is_shown && !is_exited)
	{
		OnExit();
		FileList::instance()->update();
	}
}

void fr::FileExplorerWidget::render()
{
	if (!is_exited)
	{
		widget_base_head->render();
		widget_base_foot->render();
		for (int i = 0; i < body_count; i++)
		{
			widget_base_body->SetPos(widget_base_head->GetX(), widget_y + 280 + 140 * i);
			widget_base_body->render();
		}
		confirm->render();
		parent_dir->render();
		current_path->render();
		FileList::instance()->render();
	}
}

void fr::FileExplorerWidget::SwitchShown()
{
	is_shown = !is_shown;
	if (is_shown)
	{
		Animator::instance()->Animate("file_explorer_enter");
		is_exited = false;
	}
	else
	{
		Animator::instance()->Animate("file_explorer_exit");
	}
}

bool fr::FileExplorerWidget::IsShown()
{
	return is_shown || !is_exited;
}

bool fr::FileExplorerWidget::GetOutput(std::string &result)
{
	result = FileList::instance()->GetCurrentPath();
	return is_outputed;
}

void fr::FileExplorerWidget::OnEnter()
{
	int widget_x = System::instance()->GetWindowWidth() - (System::instance()->GetWindowWidth() / 2 + widget_base_head->GetW() / 2) * Animator::instance()->GetProcess("file_explorer_enter");
	widget_base_head->SetPos(widget_x, widget_y);
	widget_base_foot->SetPos(widget_x, widget_y + 280 + body_count * 140);
	FileList::instance()->SetPos(widget_base_head->GetX() + 32, widget_base_head->GetY() + 96);
	confirm->SetPos(widget_base_foot->GetX() + 8, widget_base_foot->GetY() + 224);
	parent_dir->SetPos(widget_base_head->GetX() + 32, widget_base_head->GetY() + 32);
	current_path->SetPos(widget_base_head->GetX() + 120, widget_base_head->GetY() + 52);

	if (Animator::instance()->IsTimeUp("file_explorer_enter"))
	{
		Animator::instance()->ResetAnimation("file_explorer_enter");
		is_entered = true;
	}
}

void fr::FileExplorerWidget::OnExit()
{
	int widget_x = System::instance()->GetWindowWidth() / 2 - widget_base_head->GetW() / 2 + (System::instance()->GetWindowWidth() / 2 + widget_base_head->GetW() / 2) * Animator::instance()->GetProcess("file_explorer_exit");
	widget_base_head->SetPos(widget_x, widget_y);
	widget_base_foot->SetPos(widget_x, widget_y + 280 + body_count * 140);
	FileList::instance()->SetPos(widget_base_head->GetX() + 32, widget_base_head->GetY() + 96);
	confirm->SetPos(widget_base_foot->GetX() + 8, widget_base_foot->GetY() + 224);
	parent_dir->SetPos(widget_base_head->GetX() + 32, widget_base_head->GetY() + 32);
	current_path->SetPos(widget_base_head->GetX() + 120, widget_base_head->GetY() + 52);

	if (Animator::instance()->IsTimeUp("file_explorer_exit"))
	{
		Animator::instance()->ResetAnimation("file_explorer_exit");
		is_entered = false;
		is_exited = true;
	}
}

void fr::FileList::init()
{
	cell_base_path = "assets/base/wide_list_cell.png";
	cell_base_pressed_path = "assets/base/wide_list_cell_pressed.png";
	cell_base_selected_path = "assets/base/wide_list_cell_selected.png";
	no_dir_label = new TextArea;
	no_dir_label->init("EMPTY", Point2Di(dest_rect.x + dest_rect.w / 2, dest_rect.y + dest_rect.h / 2), "assets/fonts/Audiowide.ttf", 36, Color(0x00, 0x00, 0x00));
	current_path = "/sdcard";
	std::vector<File*> file_list;
	ListDir(current_path, file_list);
	dir_list.clear();
	const std::regex filename_pattern(".*/(.*?)");
	for (int i = 0; i < file_list.size(); i++)
	{
		if (file_list[i]->type == FILETYPE_DIR)
		{
			dir_list.push_back(std::regex_replace(file_list[i]->name, filename_pattern, "$1"));
		}
	}
	List::init(Rect(0, 0, 0, 0), dir_list.size());
	RefreshListSize();
}

void fr::FileList::clear()
{
	
}

void fr::FileList::update()
{
	List::update();
	if (IsConfirmed())
	{
		is_confirmed = false;
		current_path += "/" + dir_list[selected_index];
		RefreshList();
	}
}

void fr::FileList::render()
{
	if (dir_list.size() != 0)
	{
		List::render();
	}
	else
	{
		no_dir_label->render();
	}
}

void fr::FileList::PushText()
{
	int current_index = current_list_process / cell_h;
	for (int i = 0; i < cell.size(); i++)
	{
		if (current_index >= value_count)
		{
			current_index = 0;
		}

		if (current_index == selected_index)
		{
			cell[i]->GetText(0)->SetText(dir_list[current_index]);
			cell[i]->GetText(0)->SetColor(Color(0xFF, 0xFF, 0xFF));
		}
		else
		{
			cell[i]->GetText(0)->SetText(dir_list[current_index]);
			cell[i]->GetText(0)->SetColor(Color(0x00, 0x00, 0x00));
		}
		current_index++;
	}
}

void fr::FileList::RefreshList()
{
	std::vector<File*> file_list;
	ListDir(current_path, file_list);
	dir_list.clear();
	const std::regex filename_pattern(".*/(.*?)");
	for (int i = 0; i < file_list.size(); i++)
	{
		if (file_list[i]->type == FILETYPE_DIR)
		{
			dir_list.push_back(std::regex_replace(file_list[i]->name, filename_pattern, "$1"));
		}
	}
	SetValueCount(dir_list.size());
	RefreshListSize();
}

void fr::FileList::RefreshListSize()
{
	List::RefreshListSize();
	for (int i = 0; i < cell.size(); i++)
	{
		cell[i]->AddText("??", Point2Di(24, 20), "assets/fonts/Ubuntu-M.ttf", 24, Color(0xFF, 0xFF, 0xFF), TEXTFORMAT_LEFT, 608);
	}
	no_dir_label->SetPos(dest_rect.x + dest_rect.w / 2, dest_rect.y + dest_rect.h / 2);
}

std::string fr::FileList::GetCurrentPath()
{
	return current_path;
}

void fr::FileList::PathBack()
{
	const std::regex parent_dir_pattern("(.*)/[^/]*");
	current_path = regex_replace(current_path, parent_dir_pattern, "$1");
	RefreshList();
}