#ifndef FORCE_RUSH_USER_GAMEPLAY_WIZARD_STATE_H
#define FORCE_RUSH_USER_GAMEPLAY_WIZARD_STATE_H

#include "../state.h"
#include "beatmap.h"

namespace fr
{
	class Sprite;
	class Button;
	class TextArea;

	class GameplayWizardBeatmap : public Beatmap
	{
		public:
			static GameplayWizardBeatmap *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new GameplayWizardBeatmap;
					return m_instance;
				}
				return m_instance;
			}
			void init();
			void clear();
			void update();
			void render();
			Judgement judge(int note_time, bool is_pressed = true, bool is_ln_pressing = false, bool error_only = false);

			void start();
			void stop();
			bool IsRunning();
			void ResetScale();
		private:

			GameplayWizardBeatmap() {}
			~GameplayWizardBeatmap() {}
			static GameplayWizardBeatmap *m_instance;
			bool is_waiting;
			bool is_running;
			TextArea *hit_offset_num;
			TextArea *current_offset_text;
			int average_offset;
			int beat_count;
	};

	class GameplayWizardState : public State
	{
		public:
			static GameplayWizardState *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new GameplayWizardState;
					return m_instance;
				}
				return m_instance;
			}
			void init();
			void clear();
			void update();
		private:
			GameplayWizardState() {}
			~GameplayWizardState() {}
			static GameplayWizardState *m_instance;
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