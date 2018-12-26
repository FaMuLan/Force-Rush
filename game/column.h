#ifndef FORCE_RUSH_GAME_COLUMN_H
#define FORCE_RUSH_GAME_COLUMN_H

#include <vector>
#include <SDL2/SDL.h>
#include "../data.h"

namespace fr
{
	struct Note;
	struct NoteSet;
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
			void AddNote(NoteSet *load_note_set);
			bool DrawNote(Note *load_note);
			double TimeToProcess(unsigned int time);
			float GetCurrentSpeed();
		private:
			Beatmap *m_parent;
			int column_index;
			NoteSet *m_note_set;
			Sprite *s_note;
			Sprite *s_feedback;
			Sprite *s_light;
			float *note_vectrices;
			float *feedback_vectrices;
			int m_x_l;
			int m_x_r;
			SDL_Scancode keyboard_key;
			bool is_tapped;
			bool is_hold;
			bool is_released;
			bool is_pressing_ln;
			bool is_slide_in_l;
			bool is_slide_in_r;
			bool is_rotate_l;
			bool is_rotate_r;
			bool is_slide_out_l;
			bool is_slide_out_r;
			bool is_keyboard_pressed;
			bool is_touch_pressed;
			FingerID has_pressed_id;
			int current_note_index;
			int current_timeline_index;
	};
};

#endif