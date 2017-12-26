#ifndef FORCE_RUSH_USER_GAME_SETTING_STATE_H
#define FORCE_RUSH_USER_GAME_SETTING_STATE_H

#include "../state.h"
#include "../game/beatmap.h"

namespace fr
{
	class Sprite;
	class Button;
	class TextArea;

	class SettingBeatmap : public Beatmap
	{
		public:
			static SettingBeatmap *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new SettingBeatmap;
					return m_instance;
				}
				return m_instance;
			}
			void init();
			void clear();
			void update();
			void render();
			Judgement judge(int note_time, bool is_pressed = true, bool is_ln_pressing = false);

			void start();
			void stop();
			bool IsRunning();
			void ResetScale();
		private:

			SettingBeatmap() {}
			~SettingBeatmap() {}
			static SettingBeatmap *m_instance;
			bool is_waiting;
			bool is_running;
			TextArea *hit_offset_num;
			TextArea *current_offset_text;
			int average_offset;
			int beat_count;
	};

	class GameSettingState : public State
	{
		public:
			static GameSettingState *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new GameSettingState;
					return m_instance;
				}
				return m_instance;
			}
			void init();
			void clear();
			void update();
		private:
			GameSettingState() {}
			~GameSettingState() {}
			static GameSettingState *m_instance;
			Sprite *widget_base;
			Button *back;
			Button *offset_wizard_switch;
			Button *draw_scale_left;
			Button *draw_scale_right;
			Button *draw_scale_left_dual;
			Button *draw_scale_right_dual;
			Button *draw_offset_left;
			Button *draw_offset_right;
			Button *draw_offset_left_dual;
			Button *draw_offset_right_dual;
			TextArea *offset_wizard_text;
			TextArea *draw_scale_text;
			TextArea *draw_offset_text;
			TextArea *draw_scale_num;
			TextArea *draw_offset_num;
			
	};
};

#endif