#include "column.h"
#include "beatmap.h"
#include "../sprite.h"
#include "../control_handler.h"
#include "../system.h"
#include "../texture_manager.h"
#include "../timer.h"
#include "../user/setting.h"
#include "../song_data.h"
#include "game_header.h"

void fr::Column::init(int load_column_index, Beatmap *parent)
{
	m_parent = parent;
	column_index = load_column_index;
	current_note_index = 0;
	current_timeline_index = 0;
	m_note_set = NULL;
	is_pressing_ln = false;
	is_hold = false;
	is_touch_pressed = false;
	is_keyboard_pressed = false;
	is_rotate_l = false;
	is_rotate_r = false;
	s_note = new Sprite;
	s_feedback = new Sprite;
	s_light = new Sprite;
	note_vectrices = new int[24];
	feedback_vectrices = new int[24];

	s_light->init("", Rect(0, 0, 320 * float(System::instance()->GetWindowWidth() / 720.f), 320 * float(System::instance()->GetWindowWidth() / 720.f)));
	s_light->AddFrame("assets/game/lighting-1.png");
	s_light->AddFrame("assets/game/lighting-2.png");
	s_light->AddFrame("assets/game/lighting-3.png");
	s_light->AddFrame("assets/game/lighting-4.png");
	s_light->AddFrame("assets/game/lighting-5.png");
	s_light->AddFrame("assets/game/lighting-6.png");
	s_light->AddFrame("assets/game/lighting-7.png");
	s_light->AddFrame("assets/game/lighting-8.png");
	s_light->AddFrame("assets/game/lighting-9.png");
	s_light->AddFrame("assets/game/lighting-10.png");
	s_light->AddFrame("assets/game/lighting-11.png");
	s_light->AddFrame("assets/game/lighting-12.png");

	s_note->init("assets/game/note.png");
	s_note->AddFrame("assets/game/note_slide_in_left.png");
	s_note->AddFrame("assets/game/note_slide_in_right.png");
	s_note->AddFrame("assets/game/note_slide_out_left.png");
	s_note->AddFrame("assets/game/note_slide_out_right.png");
	s_note->AddFrame("assets/game/note_slide_through.png");
	s_note->AddFrame("assets/game/note_slide_end_left.png");
	s_note->AddFrame("assets/game/note_slide_end_right.png");
	s_note->AddFrame("assets/game/note_long_body.png");
	s_note->AddFrame("assets/game/note_long_head.png");
	s_note->AddFrame("assets/game/note_long_end.png");
	s_note->SetMatrix("mvp");

	s_feedback->init("assets/game/feedback_front_head.png");
	s_feedback->AddFrame("assets/game/feedback_back_head.png");
	s_feedback->AddFrame("assets/game/feedback_back_body.png");
	s_feedback->SetMatrix("mvp");

	note_vectrices[1] = System::instance()->GetWindowHeigh();
	note_vectrices[3] = 1;
	note_vectrices[4] = 0;
	note_vectrices[5] = 0;
	//top left
	note_vectrices[7] = System::instance()->GetWindowHeigh();
	note_vectrices[9] = 1;
	note_vectrices[10] = s_note->GetW();
	note_vectrices[11] = 0;
	//top right
	note_vectrices[13] = System::instance()->GetWindowHeigh();
	note_vectrices[15] = 1;
	note_vectrices[16] = 0;
	note_vectrices[17] = s_note->GetH();
	//bottom left
	note_vectrices[19] = System::instance()->GetWindowHeigh();
	note_vectrices[21] = 1;
	note_vectrices[22] = s_note->GetW();
	note_vectrices[23] = s_note->GetH();
	//bottom right

	feedback_vectrices[1] = System::instance()->GetWindowHeigh();
	feedback_vectrices[2] = s_feedback->GetH() + 20;
	feedback_vectrices[3] = 1;
	feedback_vectrices[4] = 0;
	feedback_vectrices[5] = 0;
	//top left
	feedback_vectrices[7] = System::instance()->GetWindowHeigh();
	feedback_vectrices[8] = s_feedback->GetH() + 20;
	feedback_vectrices[9] = 1;
	feedback_vectrices[10] = s_feedback->GetW();
	feedback_vectrices[11] = 0;
	//top right
	feedback_vectrices[13] = System::instance()->GetWindowHeigh();
	feedback_vectrices[14] = 20;
	feedback_vectrices[15] = 1;
	feedback_vectrices[16] = 0;
	feedback_vectrices[17] = s_feedback->GetH();
	//bottom left
	feedback_vectrices[19] = System::instance()->GetWindowHeigh();
	feedback_vectrices[20] = 20;
	feedback_vectrices[21] = 1;
	feedback_vectrices[22] = s_feedback->GetW();
	feedback_vectrices[23] = s_feedback->GetH();
	//bottom right

	s_light->SetSize(320 * float(System::instance()->GetWindowWidth() / 720.f), 320 * float(System::instance()->GetWindowWidth() / 720.f));

	switch (column_index)
	{
		case 0:
			note_vectrices[0] = 20 * float(System::instance()->GetWindowWidth() / 720.f);
			note_vectrices[6] = 187 * float(System::instance()->GetWindowWidth() / 720.f);
			note_vectrices[12] = 20 * float(System::instance()->GetWindowWidth() / 720.f);
			note_vectrices[18] = 187 * float(System::instance()->GetWindowWidth() / 720.f);
			feedback_vectrices[0] = 20 * float(System::instance()->GetWindowWidth() / 720.f);
			feedback_vectrices[6] = 187 * float(System::instance()->GetWindowWidth() / 720.f);
			feedback_vectrices[12] = 20 * float(System::instance()->GetWindowWidth() / 720.f);
			feedback_vectrices[18] = 187 * float(System::instance()->GetWindowWidth() / 720.f);
			s_light->SetPos(103.5f * float(System::instance()->GetWindowWidth()) / 720.f, System::instance()->GetWindowHeigh(), 84);
			m_x_l = 20 * float(System::instance()->GetWindowWidth() / 720.f);
			m_x_r = 189 * float(System::instance()->GetWindowWidth() / 720.f);
			keyboard_key = Setting::instance()->GetKeycode(column_index);
		break;
		case 1:
			note_vectrices[0] = 191 * float(System::instance()->GetWindowWidth() / 720.f);
			note_vectrices[6] = 358 * float(System::instance()->GetWindowWidth() / 720.f);
			note_vectrices[12] = 191 * float(System::instance()->GetWindowWidth() / 720.f);
			note_vectrices[18] = 358 * float(System::instance()->GetWindowWidth() / 720.f);
			feedback_vectrices[0] = 191 * float(System::instance()->GetWindowWidth() / 720.f);
			feedback_vectrices[6] = 358 * float(System::instance()->GetWindowWidth() / 720.f);
			feedback_vectrices[12] = 191 * float(System::instance()->GetWindowWidth() / 720.f);
			feedback_vectrices[18] = 358 * float(System::instance()->GetWindowWidth() / 720.f);
			s_light->SetPos(274.5f * float(System::instance()->GetWindowWidth()) / 720.f, System::instance()->GetWindowHeigh(), 84);
			m_x_l = 189 * float(System::instance()->GetWindowWidth() / 720.f);
			m_x_r = 360 * float(System::instance()->GetWindowWidth() / 720.f);
			keyboard_key = Setting::instance()->GetKeycode(column_index);
		break;
		case 2:
			note_vectrices[0] = 362 * float(System::instance()->GetWindowWidth() / 720.f);
			note_vectrices[6] = 529 * float(System::instance()->GetWindowWidth() / 720.f);
			note_vectrices[12] = 362 * float(System::instance()->GetWindowWidth() / 720.f);
			note_vectrices[18] = 529 * float(System::instance()->GetWindowWidth() / 720.f);
			feedback_vectrices[0] = 362 * float(System::instance()->GetWindowWidth() / 720.f);
			feedback_vectrices[6] = 529 * float(System::instance()->GetWindowWidth() / 720.f);
			feedback_vectrices[12] = 362 * float(System::instance()->GetWindowWidth() / 720.f);
			feedback_vectrices[18] = 529 * float(System::instance()->GetWindowWidth() / 720.f);
			s_light->SetPos(445.5f * float(System::instance()->GetWindowWidth()) / 720.f, System::instance()->GetWindowHeigh(), 84);
			m_x_l = 360 * float(System::instance()->GetWindowWidth() / 720.f);
			m_x_r = 531 * float(System::instance()->GetWindowWidth() / 720.f);
			keyboard_key = Setting::instance()->GetKeycode(column_index);
		break;
		case 3:
			note_vectrices[0] = 533 * float(System::instance()->GetWindowWidth() / 720.f);
			note_vectrices[6] = 700 * float(System::instance()->GetWindowWidth() / 720.f);
			note_vectrices[12] = 533 * float(System::instance()->GetWindowWidth() / 720.f);
			note_vectrices[18] = 700 * float(System::instance()->GetWindowWidth() / 720.f);
			feedback_vectrices[0] = 533 * float(System::instance()->GetWindowWidth() / 720.f);
			feedback_vectrices[6] = 700 * float(System::instance()->GetWindowWidth() / 720.f);
			feedback_vectrices[12] = 533 * float(System::instance()->GetWindowWidth() / 720.f);
			feedback_vectrices[18] = 700 * float(System::instance()->GetWindowWidth() / 720.f);
			m_x_l = 531 * float(System::instance()->GetWindowWidth() / 720.f);
			m_x_r = 700 * float(System::instance()->GetWindowWidth() / 720.f);
			keyboard_key = Setting::instance()->GetKeycode(column_index);
		break;
	}
	s_feedback->SetVectrices(feedback_vectrices);
	s_light->SetPos((m_x_l + m_x_r) / 2.f, System::instance()->GetWindowHeigh(), 84);
	glm::vec4 gl_pos_l(float(m_x_l) / System::instance()->GetWindowWidth() * 2.f - 1.f, -1, -float(0) / 360.f, 1.f);
	glm::vec4 gl_pos_r(float(m_x_r) / System::instance()->GetWindowWidth() * 2.f - 1.f, -1, -float(0) / 360.f, 1.f);
	glm::mat4x4 mvp_matrix = TextureManager::instance()->GetMatrix("mvp");
	glm::vec4 converted_pos_l;
	glm::vec4 converted_pos_r;
	converted_pos_l = mvp_matrix * gl_pos_l;
	converted_pos_r = mvp_matrix * gl_pos_r;
	converted_pos_l = converted_pos_l * (1.0f / converted_pos_l.w);
	converted_pos_r = converted_pos_r * (1.0f / converted_pos_r.w);
	m_x_l = ((converted_pos_l.x + 1.f) / 2.f * System::instance()->GetWindowWidth());
	m_x_r = ((converted_pos_r.x + 1.f) / 2.f * System::instance()->GetWindowWidth());

	is_tapped = false;
}	//void fr::Column::init(int column_index)

void fr::Column::clear()
{
	m_note_set->timeline.clear();
	m_note_set->note.clear();
	delete m_note_set;
	delete [] note_vectrices;
	delete [] feedback_vectrices;
	s_note->clear();
	s_feedback->clear();
	s_light->clear();
	delete s_note;
	delete s_feedback;
	delete s_light;
}

void fr::Column::update()
{
	is_released = false;
	is_tapped = false;
	is_slide_in_l = false;
	is_slide_in_r = false;
	is_slide_out_l = false;
	is_slide_out_r = false;
	if (!is_hold)
	{
//================= Touch Device ====================
		for (int i = 0; i < ControlHandler::instance()->GetFingerCount(); i++)
		{
			Finger load_finger = ControlHandler::instance()->GetFinger(i);
			if (load_finger.x >= m_x_l && load_finger.x <= m_x_r && load_finger.y > System::instance()->GetWindowHeigh() / 2)
			{
				if (!load_finger.moved && !load_finger.released)
				{
					is_hold = true;
					is_tapped = true;
					is_touch_pressed = true;
					has_pressed_id = load_finger.id;
					if (Setting::instance()->IsSlideOut())
					{
						is_slide_in_l = true;
						is_slide_in_r = true;
					}
				}
				else if (load_finger.dx >= 1 && !load_finger.released)
				{
					is_slide_in_r = true;
					is_hold = true;
					is_touch_pressed = true;
					has_pressed_id = load_finger.id;
				}
				else if (load_finger.dx <= -1 && !load_finger.released)
				{
					is_slide_in_l = true;
					is_hold = true;
					is_touch_pressed = true;
					has_pressed_id = load_finger.id;
				}
			}
		}
//================= Keyboard Device =================
		if (ControlHandler::instance()->IsKeyDown(keyboard_key))
		{
			is_hold = true;
			if (Setting::instance()->IsSlideOut())
			{
				is_slide_in_l = true;
				is_slide_in_r = true;
			}
			is_tapped = true;
			is_keyboard_pressed = true;
		}
	}
	else
	{
//================= Touch Device ====================
		if (is_touch_pressed)
		{
			for (int i = 0; i < ControlHandler::instance()->GetFingerCount(); i++)
			{
				Finger load_finger = ControlHandler::instance()->GetFinger(i);
				if (load_finger.id == has_pressed_id)
				{
					if ((load_finger.x < m_x_l || load_finger.x > m_x_r) || load_finger.y < System::instance()->GetWindowHeigh() / 2)
					{
						is_hold = false;
						is_released = true;
						is_touch_pressed = false;
						if (load_finger.dx >= 1)
						{
							is_slide_out_r = true;
						}
						if (load_finger.dx <= -1)
						{
							is_slide_out_l = true;
						}
					}
					if (load_finger.released)
					{
						is_released = true;
						if (Setting::instance()->IsSlideOut())
						{
							is_slide_out_l = true;
							is_slide_out_r = true;
						}
						is_hold = false;
						is_touch_pressed = false;
					}
				}
			}
		}
//================= Keyboard Device =================
		if (!ControlHandler::instance()->IsKeyDown(keyboard_key) && is_keyboard_pressed)

		{
			is_hold = false;
			is_released = true;
			if (Setting::instance()->IsSlideOut())
			{
				is_slide_out_l = true;
				is_slide_out_r = true;
			}
			is_keyboard_pressed = false;
		}
	}
//================ Auto Mod =========
	if (Setting::instance()->IsAuto())
	{
		if (current_note_index < m_note_set->note.size())
		{
			if (is_pressing_ln)
			{
				if (m_note_set->note[current_note_index]->time_end - Setting::instance()->GetOffset() < Timer::instance()->GetTime("game"))
				{
					is_released = true;
					is_slide_out_l = true;
					is_slide_out_r = true;
				}
			}
			if (m_note_set->note[current_note_index]->time - Setting::instance()->GetOffset() < Timer::instance()->GetTime("game") && !is_pressing_ln)
			{
				if (m_note_set->note[current_note_index]->type == NOTETYPE_SLIDE_THROUGH)
				{
					is_slide_out_l = true;
					is_slide_out_r = true;
				}
				else
				{
					is_tapped = true;
					is_slide_in_l = true;
					is_slide_in_r = true;
				}
			}
		}
	}
//=================== End ===========
	if (!Timer::instance()->IsPaused("game"))
	{
		if (current_note_index < m_note_set->note.size())
		//檢測防止下標越界而導致段錯誤
		{
			if (is_tapped || is_slide_in_l || is_slide_in_r || (m_note_set->note[current_note_index]->type == NOTETYPE_SLIDE_THROUGH && (is_slide_out_l || is_slide_out_r)))
			{
				Judgement current_judgement;
				if (m_note_set->note[current_note_index]->type == NOTETYPE_NORMAL)
				{
					current_judgement = m_parent->judge(m_note_set->note[current_note_index]->time);
				}
				else if ((m_note_set->note[current_note_index]->type == NOTETYPE_SLIDE_IN_LEFT && is_slide_in_l) || (m_note_set->note[current_note_index]->type == NOTETYPE_SLIDE_IN_RIGHT && is_slide_in_r))
				{
					current_judgement = m_parent->judge(m_note_set->note[current_note_index]->time, true, false, true);
				}
				else if (m_note_set->note[current_note_index]->type == NOTETYPE_SLIDE_END_LEFT && is_slide_in_l)
				{
					current_judgement = m_parent->judge(m_note_set->note[current_note_index]->time, true, false, true);
					if (current_judgement != JUDGEMENT_ER && current_judgement != JUDGEMENT_NONE)
					{
						is_rotate_l = true;
					}
					Timer::instance()->ResetTimer("slide_rotate_l");
					Timer::instance()->RunTimer("slide_rotate_l");
				}
				else if (m_note_set->note[current_note_index]->type == NOTETYPE_SLIDE_END_RIGHT && is_slide_in_r)
				{
					current_judgement = m_parent->judge(m_note_set->note[current_note_index]->time, true, false, true);
					if (current_judgement != JUDGEMENT_ER && current_judgement != JUDGEMENT_NONE)
					{
						is_rotate_r = true;
					}
					Timer::instance()->ResetTimer("slide_rotate_r");
					Timer::instance()->RunTimer("slide_rotate_r");
				}
				else if (m_note_set->note[current_note_index]->type == NOTETYPE_SLIDE_THROUGH && (is_slide_out_l || is_slide_out_r))
				{
					current_judgement = m_parent->judge(m_note_set->note[current_note_index]->time, true, false, true);
				}
				else
				{
					current_judgement = JUDGEMENT_NONE;
				}

				if (current_judgement != JUDGEMENT_ER && current_judgement != JUDGEMENT_NONE)
				{
					if (m_note_set->note[current_note_index]->time != m_note_set->note[current_note_index]->time_end)
					{
						is_pressing_ln = true;
						//有長條的情況下先不跳過
					}
					else
					{
						current_note_index++;
						//正常note可跳過
					}
					s_light->SetAnimate(1, 12, 200);
				}
				else if (current_judgement == JUDGEMENT_ER)
				{
					current_note_index++;
					//誤觸就ERROR的話，別說了，下一個note，請
				}
			}

			if (is_released || is_slide_out_l || is_slide_out_r)
			{
				if (is_pressing_ln && m_note_set->note[current_note_index]->time != m_note_set->note[current_note_index]->time_end)
				{
					Judgement current_judgement;
					if (m_note_set->note[current_note_index]->type_end == NOTETYPE_NORMAL)
					{
						current_judgement = m_parent->judge(m_note_set->note[current_note_index]->time_end, true, true, false);
					}
					else if ((m_note_set->note[current_note_index]->type_end == NOTETYPE_SLIDE_OUT_LEFT && is_slide_out_l) || (m_note_set->note[current_note_index]->type_end == NOTETYPE_SLIDE_OUT_RIGHT && is_slide_out_r))
					{
						current_judgement = m_parent->judge(m_note_set->note[current_note_index]->time_end, true, true, false);
					}
					else
					{
						current_judgement = JUDGEMENT_ER;
					}

					if (current_judgement != JUDGEMENT_ER)
					{
						s_light->SetAnimate(1, 12, 200);
					}
					current_note_index++;
				}
				is_pressing_ln = false;
			}

			if (!is_pressing_ln && current_note_index < m_note_set->note.size())
			//以防檢測開頭而導致意外ERROR
			//注意因為之前current_note_index可能執行過++所以還是需要做檢測
			{
				if (m_parent->judge(m_note_set->note[current_note_index]->time, false) == JUDGEMENT_ER)
				{
					current_note_index++;
				}
			}

			if (is_pressing_ln)
			{
				if (s_light->GetCurrentIndex() == 0)
				{
					s_light->SetAnimate(1, 12, 200);
				}
				if (m_parent->judge(m_note_set->note[current_note_index]->time_end, false) == JUDGEMENT_ER)
				{
					current_note_index++;
					is_pressing_ln = false;
				}
				//长条尾未松开手指
				switch (column_index)
				{
					case 0:
						m_parent->AddForce(-Setting::instance()->GetForceAngle());
					break;
					case 1:
						m_parent->AddForce(-Setting::instance()->GetForceAngle() / 2.f);
					break;
					case 2:
						m_parent->AddForce(Setting::instance()->GetForceAngle() / 2.f);
					break;
					case 3:
						m_parent->AddForce(Setting::instance()->GetForceAngle());
					break;
				}
			}
			if (is_rotate_l)
			{
				m_parent->AddForce(-Setting::instance()->GetForceAngle() / 2.f);
			}
			if (is_rotate_r)
			{
				m_parent->AddForce(Setting::instance()->GetForceAngle() / 2.f);
			}
			if (Timer::instance()->GetTime("slide_rotate_l") >= 300)
			{
				is_rotate_l = false;
			}
			if (Timer::instance()->GetTime("slide_rotate_r") >= 300)
			{
				is_rotate_r = false;
			}
		}
	}
	s_light->update();

	if (current_timeline_index < m_note_set->timeline.size())
	{
		while (m_note_set->timeline[current_timeline_index]->end_time - Setting::instance()->GetOffset() < Timer::instance()->GetTime("game") && m_note_set->timeline[current_timeline_index]->end_time != 0)
		{
			current_timeline_index++;
		}
	}

	if (System::instance()->IsWindowModified())
	{
		s_light->SetSize(320 * float(System::instance()->GetWindowWidth() / 720.f), 320 * float(System::instance()->GetWindowWidth() / 720.f));
		note_vectrices[1] = System::instance()->GetWindowHeigh();
		note_vectrices[7] = System::instance()->GetWindowHeigh();
		note_vectrices[13] = System::instance()->GetWindowHeigh();
		note_vectrices[19] = System::instance()->GetWindowHeigh();
		feedback_vectrices[1] = System::instance()->GetWindowHeigh();
		feedback_vectrices[7] = System::instance()->GetWindowHeigh();
		feedback_vectrices[13] = System::instance()->GetWindowHeigh();
		feedback_vectrices[19] = System::instance()->GetWindowHeigh();
		switch (column_index)
		{
			case 0:
				note_vectrices[0] = 20 * float(System::instance()->GetWindowWidth() / 720.f);
				note_vectrices[6] = 187 * float(System::instance()->GetWindowWidth() / 720.f);
				note_vectrices[12] = 20 * float(System::instance()->GetWindowWidth() / 720.f);
				note_vectrices[18] = 187 * float(System::instance()->GetWindowWidth() / 720.f);
				feedback_vectrices[0] = 20 * float(System::instance()->GetWindowWidth() / 720.f);
				feedback_vectrices[6] = 187 * float(System::instance()->GetWindowWidth() / 720.f);
				feedback_vectrices[12] = 20 * float(System::instance()->GetWindowWidth() / 720.f);
				feedback_vectrices[18] = 187 * float(System::instance()->GetWindowWidth() / 720.f);
				m_x_l = 20 * float(System::instance()->GetWindowWidth() / 720.f);
				m_x_r = 189 * float(System::instance()->GetWindowWidth() / 720.f);
			break;
			case 1:
				note_vectrices[0] = 191 * float(System::instance()->GetWindowWidth() / 720.f);
				note_vectrices[6] = 358 * float(System::instance()->GetWindowWidth() / 720.f);
				note_vectrices[12] = 191 * float(System::instance()->GetWindowWidth() / 720.f);
				note_vectrices[18] = 358 * float(System::instance()->GetWindowWidth() / 720.f);
				feedback_vectrices[0] = 191 * float(System::instance()->GetWindowWidth() / 720.f);
				feedback_vectrices[6] = 358 * float(System::instance()->GetWindowWidth() / 720.f);
				feedback_vectrices[12] = 191 * float(System::instance()->GetWindowWidth() / 720.f);
				feedback_vectrices[18] = 358 * float(System::instance()->GetWindowWidth() / 720.f);
				m_x_l = 189 * float(System::instance()->GetWindowWidth() / 720.f);
				m_x_r = 360 * float(System::instance()->GetWindowWidth() / 720.f);
			break;
			case 2:
				note_vectrices[0] = 362 * float(System::instance()->GetWindowWidth() / 720.f);
				note_vectrices[6] = 529 * float(System::instance()->GetWindowWidth() / 720.f);
				note_vectrices[12] = 362 * float(System::instance()->GetWindowWidth() / 720.f);
				note_vectrices[18] = 529 * float(System::instance()->GetWindowWidth() / 720.f);
				feedback_vectrices[0] = 362 * float(System::instance()->GetWindowWidth() / 720.f);
				feedback_vectrices[6] = 529 * float(System::instance()->GetWindowWidth() / 720.f);
				feedback_vectrices[12] = 362 * float(System::instance()->GetWindowWidth() / 720.f);
				feedback_vectrices[18] = 529 * float(System::instance()->GetWindowWidth() / 720.f);
				m_x_l = 360 * float(System::instance()->GetWindowWidth() / 720.f);
				m_x_r = 531 * float(System::instance()->GetWindowWidth() / 720.f);
			break;
			case 3:
				note_vectrices[0] = 533 * float(System::instance()->GetWindowWidth() / 720.f);
				note_vectrices[6] = 700 * float(System::instance()->GetWindowWidth() / 720.f);
				note_vectrices[12] = 533 * float(System::instance()->GetWindowWidth() / 720.f);
				note_vectrices[18] = 700 * float(System::instance()->GetWindowWidth() / 720.f);
				feedback_vectrices[0] = 533 * float(System::instance()->GetWindowWidth() / 720.f);
				feedback_vectrices[6] = 700 * float(System::instance()->GetWindowWidth() / 720.f);
				feedback_vectrices[12] = 533 * float(System::instance()->GetWindowWidth() / 720.f);
				feedback_vectrices[18] = 700 * float(System::instance()->GetWindowWidth() / 720.f);
				m_x_l = 531 * float(System::instance()->GetWindowWidth() / 720.f);
				m_x_r = 700 * float(System::instance()->GetWindowWidth() / 720.f);
			break;
		}
		s_feedback->SetVectrices(feedback_vectrices);
		s_light->SetPos((m_x_l + m_x_r) / 2.f, System::instance()->GetWindowHeigh(), 84);
		glm::vec4 gl_pos_l(float(m_x_l) / System::instance()->GetWindowWidth() * 2.f - 1.f, -1, -float(0) / 360.f, 1.f);
		glm::vec4 gl_pos_r(float(m_x_r) / System::instance()->GetWindowWidth() * 2.f - 1.f, -1, -float(0) / 360.f, 1.f);
		glm::mat4x4 mvp_matrix = TextureManager::instance()->GetMatrix("mvp");
		glm::vec4 converted_pos_l;
		glm::vec4 converted_pos_r;
		converted_pos_l = mvp_matrix * gl_pos_l;
		converted_pos_r = mvp_matrix * gl_pos_r;
		converted_pos_l = converted_pos_l * (1.0f / converted_pos_l.w);
		converted_pos_r = converted_pos_r * (1.0f / converted_pos_r.w);
		m_x_l = ((converted_pos_l.x + 1.f) / 2.f * System::instance()->GetWindowWidth());
		m_x_r = ((converted_pos_r.x + 1.f) / 2.f * System::instance()->GetWindowWidth());
	}
}

void fr::Column::render()
{
	int i = current_note_index;
	bool is_note_in_screen = i < m_note_set->note.size();
	int feedback_z = s_feedback->GetH() + 20;

	if (is_hold || is_tapped || is_pressing_ln)
	{
		while (feedback_z < System::instance()->GetWindowDepth())
		{
			feedback_vectrices[2] = feedback_z + s_feedback->GetH();
			feedback_vectrices[8] = feedback_z + s_feedback->GetH();
			feedback_vectrices[14] = feedback_z;
			feedback_vectrices[20] = feedback_z;
			s_feedback->SetVectrices(feedback_vectrices);
			s_feedback->render(2);
			feedback_z += s_feedback->GetH();
		}
		feedback_z = 20;
		feedback_vectrices[2] = feedback_z + s_feedback->GetH();
		feedback_vectrices[8] = feedback_z + s_feedback->GetH();
		feedback_vectrices[14] = feedback_z;
		feedback_vectrices[20] = feedback_z;
		s_feedback->SetVectrices(feedback_vectrices);
		s_feedback->render(1);
	}
	while (is_note_in_screen)
	{
		is_note_in_screen = DrawNote(m_note_set->note[i]);
		i++;
		is_note_in_screen = is_note_in_screen && i < m_note_set->note.size();
	}
	s_light->render();
	if (is_hold || is_tapped || is_pressing_ln)
	{
		s_feedback->render(0);
	}
}

void fr::Column::reset()
{
	current_note_index = 0;
	current_timeline_index = 0;
}


void fr::Column::AddNote(NoteSet *load_note_set)
{
	m_note_set = load_note_set;
}

bool fr::Column::DrawNote(Note *load_note)
{
	double process = TimeToProcess(load_note->time);
	int note_z = (System::instance()->GetWindowDepth() - 20 - s_note->GetH()) * (1.f - process) + 20 + s_note->GetH();
	//note時間與當前時間的時間差

	if (process < 0)
	{
		//檢測當前note是否在屏幕外面
		return false;
	}

	if (load_note->time != load_note->time_end)
	//是否長條
	{
		double process_end = TimeToProcess(load_note->time_end);
		int note_z_end = (System::instance()->GetWindowDepth() - 20 - s_note->GetH()) * (1.f - process_end) + 20 + s_note->GetH();
		//時間差轉換成Z坐標 * 2

//		if (is_pressing_ln && load_note->time == m_note_set->note[current_note_index]->time && load_note->time_end == m_note_set->note[current_note_index]->time_end)
		if (is_pressing_ln && load_note == m_note_set->note[current_note_index])
		//用超智障的方法來確認這是繪製的第一個長條
		{
			process = 1;
			note_z = 20 + s_note->GetH();
		}

		double ln_piece_process = process;
		while (ln_piece_process > (process_end < 0 ? 0 : process_end))
		//長條身不超過屏幕 且 不超過尾部 時畫出來，循環
		{
			int note_z_piece = (System::instance()->GetWindowDepth() - 20 - s_note->GetH()) * (1.f - ln_piece_process) + 20 + s_note->GetH();
			note_vectrices[2] = note_z_piece;
			note_vectrices[8] = note_z_piece;
			note_vectrices[14] = note_z_piece - s_note->GetH();
			note_vectrices[20] = note_z_piece - s_note->GetH();
			s_note->SetVectrices(note_vectrices);
			s_note->render(8);
			//將長條身往上挪動
			ln_piece_process -= s_note->GetH() / float(System::instance()->GetWindowDepth() - 20 - s_note->GetH());
		}
/*
		{
			if (current_y * m_parent->GetScaleH() < System::instance()->GetWindowHeigh())
			{
				s_note->SetPos(current_x, current_y);
				s_note->SetScale(current_scale);
				s_note->render();
			}
			return false;
		}
		else
*/
		if (ln_piece_process > 0)
		{
			note_vectrices[2] = note_z_end;
			note_vectrices[8] = note_z_end;
			note_vectrices[14] = note_z_end - s_note->GetH();
			note_vectrices[20] = note_z_end - s_note->GetH();
			s_note->SetVectrices(note_vectrices);
			s_note->render((load_note->type_end == NOTETYPE_NORMAL || Setting::instance()->IsSlideOut()) ? 10 : load_note->type_end);
		}
		//畫長條尾
	}

	note_vectrices[2] = note_z;
	note_vectrices[8] = note_z;
	note_vectrices[14] = note_z - s_note->GetH();
	note_vectrices[20] = note_z - s_note->GetH();
	s_note->SetVectrices(note_vectrices);
	s_note->render(((is_pressing_ln && load_note == m_note_set->note[current_note_index]) || Setting::instance()->IsSlideOut()) ? NOTETYPE_NORMAL : load_note->type);

	return true;
	//畫note
}

double fr::Column::TimeToProcess(unsigned int time)
{
	int previous_time = Setting::instance()->GetOffset() + Timer::instance()->GetTime("game");
	int next_time = (time < m_note_set->timeline[current_timeline_index]->end_time || m_note_set->timeline[current_timeline_index]->end_time == 0) ? time : m_note_set->timeline[current_timeline_index]->end_time;
	int timeline_index_offset = 1;
	int time_diff = next_time - previous_time;
	double process;
	if (m_note_set->timeline[current_timeline_index]->speed != 0)
	{
		process = 1.f - double(time_diff) / double(50000.f / (Setting::instance()->GetSpeed() * m_note_set->timeline[current_timeline_index]->speed));
	}
	else
	{
		process = 1.f;
	}

	if (current_timeline_index + timeline_index_offset < m_note_set->timeline.size())
	{
		while (time > m_note_set->timeline[current_timeline_index + timeline_index_offset]->start_time)
		{
			previous_time = m_note_set->timeline[current_timeline_index + timeline_index_offset]->start_time;
			next_time = (time < m_note_set->timeline[current_timeline_index + timeline_index_offset]->end_time || m_note_set->timeline[current_timeline_index + timeline_index_offset]->end_time == 0) ? time : m_note_set->timeline[current_timeline_index + timeline_index_offset]->end_time;
			time_diff = next_time - previous_time;
			if (m_note_set->timeline[current_timeline_index + timeline_index_offset]->speed != 0)
			{
				process -= double(time_diff) / double(50000.f / (Setting::instance()->GetSpeed() * m_note_set->timeline[current_timeline_index + timeline_index_offset]->speed));
			}
			timeline_index_offset++;
			if (current_timeline_index + timeline_index_offset >= m_note_set->timeline.size())
			{
				break;
			}
		}
	}
	return process;
}

float fr::Column::GetCurrentSpeed()
{
	if (m_note_set->timeline.size() != 0)
	{
		return m_note_set->timeline[current_timeline_index]->speed;
	}
	return 1;
}