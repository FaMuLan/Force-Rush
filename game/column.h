#ifndef LUNATIC_MELODY_GAME_COLUMN_H
#define LUNATIC_MELODY_GAME_COLUMN_H

#include <vector>
#include <SDL2/SDL.h>

namespace lm
{
	struct Note;
	class Sprite;
	typedef SDL_FingerID FingerID;

	class Column
	{
		public:
			void init(int column_index);
			void clear();
			void update();
			void render();
			void AddNote(Note *load_note);
			bool DrawNote(int time, int time_end);
		private:
			std::vector<Note*> m_note;
			Sprite *s_note;
			Sprite *s_light;
			int start_x;
			int start_y;
			float start_scale;
			int end_x;
			int end_y;
			int m_x;
			int m_w;
			int m_h;
			bool is_tapped;
			bool is_hold;
			bool is_released;
			bool is_pressing_ln;
			bool is_ended;
			FingerID has_pressed_id;
			int current_note_index;
	};
};

#endif