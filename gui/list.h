#ifndef FORCE_RUSH_GUI_LIST_H
#define FORCE_RUSH_GUI_LIST_H

#include <string>
#include <vector>
#include "../shape.h"

namespace fr
{
	class Button;
	class Sprite;

	class List
	{
		public:
			void init(Rect load_dest_rect, int load_value_count, bool load_is_loop = false, bool load_is_double_confirm = false, int load_selected_index = 0);
			void clear();
			void update();
			void render();
			void RefreshListSize();
			void SetValueCount(int load_value_count);
			void SetSelectedIndex(int load_selected_index);
			bool IsConfirmed();
			void SetPos(int x, int y);
			void SetSize(int w, int h);
		protected:
			Rect dest_rect;
			int cell_h;
			int list_length;
			int value_count;
			int selected_index;
			int last_selected_index;
			int ergodic_value_index;
			int current_list_process;
			int roll_speed;
			bool is_loop;
			bool is_confirmed;
			bool is_double_confirm;
			bool is_locked;
			bool is_list_moved;
			std::vector<Button*> cell;
			std::string cell_base_path;
			std::string cell_base_pressed_path;
			std::string cell_base_selected_path;
	};
};

#endif