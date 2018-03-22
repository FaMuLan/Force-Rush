#ifndef FORCE_RUSH_GAME_COLUMN_H
#define FORCE_RUSH_GAME_COLUMN_H

#include <vector>
#include <SDL2/SDL.h>
#include "../shape.h"

namespace fr
{
	struct Note;
	class Sprite;
	class Beatmap;
	typedef SDL_FingerID FingerID;

	class Column
	{
		public:
			void init(int load_column_index, Beatmap *parent);
			void clear();
			void update();
			void render();
			void reset();
			void AddNote(Note *load_note);
			void AddNote(std::vector<Note*> &load_note_list);
			bool DrawNote(Note *load_note);
		private:
			Beatmap *m_parent;
			int column_index;
			std::vector<Note*> m_note;
			Sprite *s_note;
			Sprite *s_feedback;
			Sprite *s_light;
			int *note_vectrices;
			int *feedback_vectrices;
			int m_x_l;
			int m_x_r;
			SDL_Scancode keyboard_key;
			bool is_tapped;
			bool is_hold;
			bool is_released;
			bool is_pressing_ln;
			bool is_slide_in_l;
			bool is_slide_in_r;
			bool is_slide_out_l;
			bool is_slide_out_r;
			bool is_keyboard_pressed;
			bool is_touch_pressed;
			FingerID has_pressed_id;
			int current_note_index;
	};
};

#endif