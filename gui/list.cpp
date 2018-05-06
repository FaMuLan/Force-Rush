#include "list.h"
#include "button.h"
#include "../control_handler.h"

void fr::List::init(Rect load_dest_rect, int load_value_count,  bool load_is_loop, bool load_is_double_confirm, int load_selected_index)
{
	dest_rect = load_dest_rect;
	value_count = load_value_count;
	selected_index = load_selected_index;
	last_selected_index = load_selected_index;
	is_loop = load_is_loop;
	is_double_confirm = load_is_double_confirm;
	current_list_process = 0;
	roll_speed = 0;
	is_locked = false;
	is_list_moved = false;
	is_confirmed = false;
	RefreshListSize();
}

void fr::List::clear()
{
	
}

void fr::List::update()
{
	for (int i = 0; i < ControlHandler::instance()->GetFingerCount(); i++)
	{
		Finger load_finger = ControlHandler::instance()->GetFinger(i);
		if ((load_finger.x >= dest_rect.x) && (load_finger.x <= dest_rect.x + dest_rect.w) && (load_finger.y >= dest_rect.y) && (load_finger.y <= dest_rect.y + dest_rect.w) && !is_locked)
		{
			current_list_process -= load_finger.dy;
			roll_speed = load_finger.dy;

			while (current_list_process < 0)
			{
				current_list_process += list_length;
			}
			while (current_list_process >= list_length)
			{
				current_list_process -= list_length;
			}
			//把list_process盡量控制在[0,list_length)區間內
			//艸有必要用一個笨方法來控制變量在範圍內了
		}
	}

	if (ControlHandler::instance()->GetFingerCount() == 0)
	{
		if (roll_speed < 0)
		{
			roll_speed += 2;
			roll_speed = roll_speed > 0 ? 0 : roll_speed;
		}
		else if (roll_speed > 0)
		{
			roll_speed -= 2;
			roll_speed = roll_speed < 0 ? 0 : roll_speed;
		}
		//不包含 roll_speed == 0 這個情況
		current_list_process -= roll_speed;
		while (current_list_process < 0)
		{
			current_list_process += list_length;
		}
		while (current_list_process >= list_length)
		{
			current_list_process -= list_length;
		}
	}

	is_list_moved = roll_speed != 0;
	//检测列表是否移动。

//	int list_middle_y = (dest_rect.y + dest_rect.h) / 2;
	int ergodic_value_index = current_list_process / cell_h;
	for (int i = 0; i < cell.size(); i++)
	{
		int x = dest_rect.x;
		int y = -(current_list_process % cell_h) + i * cell_h + dest_rect.y;
		cell[i]->SetPos(x, y);
		cell[i]->SetSrcRect(Rect(0, 0, cell[i]->GetW(), cell_h));
		if (i == 0)
		{
			cell[i]->SetPos(dest_rect.x, dest_rect.y);
			cell[i]->SetSize(cell[i]->GetW(), cell_h - (dest_rect.y - y));
			cell[i]->SetSrcRect(Rect(0, dest_rect.y - y, cell[i]->GetW(), cell_h - (dest_rect.y - y)));
		}
		if (i == cell.size() - 1)
		{
			cell[i]->SetSize(cell[i]->GetW(), dest_rect.y + dest_rect.h - cell[i]->GetY());
			cell[i]->SetSrcRect(Rect(0, 0, cell[i]->GetW(), dest_rect.y + dest_rect.h - cell[i]->GetY()));
		}

		if (ergodic_value_index >= value_count)
		{
			ergodic_value_index = 0;
		}
		if (ergodic_value_index == selected_index)
		{
			cell[i]->SetBaseFrame(2);
		}
		else
		{
			cell[i]->SetBaseFrame(0);
		}

		cell[i]->update();

		if (cell[i]->IsReleased() && (!is_double_confirm || ergodic_value_index == selected_index))
		{
			is_confirmed = true;
		}
		else if (cell[i]->IsReleased())
		{
			selected_index = ergodic_value_index;
		}
		ergodic_value_index++;
	}
}

void fr::List::render()
{
	for (int i = 0; i < cell.size(); i++)
	{
		cell[i]->render();
	}
}

void fr::List::RefreshListSize()
{
	cell.clear();
	Button *new_first_cell = new Button;
	new_first_cell->init(cell_base_path);
	new_first_cell->AddPressedFrame(cell_base_pressed_path);
	new_first_cell->AddFrame(cell_base_selected_path);
	cell.push_back(new_first_cell);
	cell_h = new_first_cell->GetH();
	int shown_cell_count = dest_rect.h / cell_h + 2;
	if (!is_loop && shown_cell_count > value_count)
	{
		shown_cell_count = value_count;
	}

	for (int i = 1; i < shown_cell_count; i++)
	{
		Button *new_cell = new Button;
		new_cell->init(cell_base_path);
		new_cell->AddPressedFrame(cell_base_pressed_path);
		new_cell->AddFrame(cell_base_selected_path);
		cell.push_back(new_cell);
	}
	list_length = cell_h * value_count;
}

void fr::List::SetValueCount(int load_value_count)
{
	value_count = load_value_count;
}

void fr::List::SetSelectedIndex(int load_selected_index)
{
	selected_index = load_selected_index;
}

bool fr::List::IsConfirmed()
{
	return is_confirmed;
}

void fr::List::SetPos(int x, int y)
{
	dest_rect.x = x;
	dest_rect.y = y;
}

void fr::List::SetSize(int w, int h)
{
	dest_rect.w = w;
	dest_rect.h = h;
}