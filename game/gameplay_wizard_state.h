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
			Button *camera_pos_y_left;
			Button *camera_pos_y_right;
			Button *camera_pos_y_left_dual;
			Button *camera_pos_y_right_dual;
			Button *camera_pos_z_left;
			Button *camera_pos_z_right;
			Button *camera_pos_z_left_dual;
			Button *camera_pos_z_right_dual;
			Button *camera_rotate_x_left;
			Button *camera_rotate_x_right;
			Button *camera_rotate_x_left_dual;
			Button *camera_rotate_x_right_dual;
			TextArea *offset_wizard_text;
			TextArea *camera_pos_y_text;
			TextArea *camera_pos_z_text;
			TextArea *camera_rotate_x_text;
			TextArea *camera_pos_y_num;
			TextArea *camera_pos_z_num;
			TextArea *camera_rotate_x_num;
			
	};
};

#endif