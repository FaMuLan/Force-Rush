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
	m_h = 106;
	current_note_index = 0;
	is_pressing_ln = false;
	is_hold = false;
	is_touch_pressed = false;
	is_keyboard_pressed = false;
	s_note = new Sprite;
//	s_feedback = new Sprite;
	s_light = new Sprite;
	note_vectrices = new int[24];

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

	light_pos.y = System::instance()->GetWindowHeigh();
	light_pos.z = 84;
	light_pos.w = 1;
	s_light->SetSize(320 * float(System::instance()->GetWindowWidth() / 720.f), 320 * float(System::instance()->GetWindowWidth() / 720.f));

	switch (column_index)
	{
		case 0:
			note_vectrices[0] = 20 * float(System::instance()->GetWindowWidth() / 720.f);
			note_vectrices[6] = 187 * float(System::instance()->GetWindowWidth() / 720.f);
			note_vectrices[12] = 20 * float(System::instance()->GetWindowWidth() / 720.f);
			note_vectrices[18] = 187 * float(System::instance()->GetWindowWidth() / 720.f);
			s_light->SetPos(103.5f * float(System::instance()->GetWindowWidth()) / 720.f, System::instance()->GetWindowHeigh(), 84);
			m_x = 0;
			m_w = 166 * float(System::instance()->GetWindowWidth() / 720.f);
			keyboard_key = Setting::instance()->GetKeycode(column_index);
//			s_light->SetPos(-38 * float(System::instance()->GetWindowWidth() / 720.f), 1078 * m_parent->GetScaleH() - s_light->GetH() / 2 + Setting::instance()->GetDrawOffset());
		break;
		case 1:
			note_vectrices[0] = 191 * float(System::instance()->GetWindowWidth() / 720.f);
			note_vectrices[6] = 358 * float(System::instance()->GetWindowWidth() / 720.f);
			note_vectrices[12] = 191 * float(System::instance()->GetWindowWidth() / 720.f);
			note_vectrices[18] = 358 * float(System::instance()->GetWindowWidth() / 720.f);
			s_light->SetPos(274.5f * float(System::instance()->GetWindowWidth()) / 720.f, System::instance()->GetWindowHeigh(), 84);
			m_x = 166 * float(System::instance()->GetWindowWidth() / 720.f);
			m_w = 194 * float(System::instance()->GetWindowWidth() / 720.f);
			keyboard_key = Setting::instance()->GetKeycode(column_index);
//			s_light->SetPos(119 * float(System::instance()->GetWindowWidth() / 720.f), 1078 * m_parent->GetScaleH() - s_light->GetH() / 2 + Setting::instance()->GetDrawOffset());
		break;
		case 2:
			note_vectrices[0] = 362 * float(System::instance()->GetWindowWidth() / 720.f);
			note_vectrices[6] = 529 * float(System::instance()->GetWindowWidth() / 720.f);
			note_vectrices[12] = 362 * float(System::instance()->GetWindowWidth() / 720.f);
			note_vectrices[18] = 529 * float(System::instance()->GetWindowWidth() / 720.f);
			s_light->SetPos(445.5f * float(System::instance()->GetWindowWidth()) / 720.f, System::instance()->GetWindowHeigh(), 84);
			m_x = 360 * float(System::instance()->GetWindowWidth() / 720.f);
			m_w = 194 * float(System::instance()->GetWindowWidth() / 720.f);
			keyboard_key = Setting::instance()->GetKeycode(column_index);
//			s_light->SetPos(282 * float(System::instance()->GetWindowWidth() / 720.f), 1078 * m_parent->GetScaleH() - s_light->GetH() / 2 + Setting::instance()->GetDrawOffset());
		break;
		case 3:
			note_vectrices[0] = 533 * float(System::instance()->GetWindowWidth() / 720.f);
			note_vectrices[6] = 700 * float(System::instance()->GetWindowWidth() / 720.f);
			note_vectrices[12] = 533 * float(System::instance()->GetWindowWidth() / 720.f);
			note_vectrices[18] = 700 * float(System::instance()->GetWindowWidth() / 720.f);
			s_light->SetPos(616.5f * float(System::instance()->GetWindowWidth()) / 720.f, System::instance()->GetWindowHeigh(), 84);
			m_x = 515 * float(System::instance()->GetWindowWidth() / 720.f);
			m_w = 166 * float(System::instance()->GetWindowWidth() / 720.f);
			keyboard_key = Setting::instance()->GetKeycode(column_index);
//			s_light->SetPos(443 * float(System::instance()->GetWindowWidth() / 720.f), 1078 * m_parent->GetScaleH() - s_light->GetH() / 2 + Setting::instance()->GetDrawOffset());
		break;
	}

	is_tapped = false;
}	//void fr::Column::init(int column_index)

void fr::Column::clear()
{
	std::vector<Note*>(m_note).swap(m_note);
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
			if (load_finger.x >= m_x && load_finger.x <= (m_x + m_w) && load_finger.y > System::instance()->GetWindowHeigh() / 2)
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
					if ((load_finger.x < m_x || load_finger.x > (m_x + m_w)) || load_finger.y < System::instance()->GetWindowHeigh() / 2)
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
		if (current_note_index < m_note.size())
		{
			if (is_pressing_ln)
			{
				if (m_note[current_note_index]->time_end - Setting::instance()->GetOffset() < Timer::instance()->GetTime("game"))
				{
					is_released = true;
					is_slide_out_l = true;
					is_slide_out_r = true;
				}
			}
			if (m_note[current_note_index]->time - Setting::instance()->GetOffset() < Timer::instance()->GetTime("game") && !is_pressing_ln)
			{
				if (m_note[current_note_index]->type == NOTETYPE_SLIDE_THROUGH)
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
	if (!GameHeader::instance()->IsPaused())
	{
		if (current_note_index < m_note.size())
		//檢測防止下標越界而導致段錯誤
		{
			if (is_tapped || is_slide_in_l || is_slide_in_r || (m_note[current_note_index]->type == NOTETYPE_SLIDE_THROUGH && (is_slide_out_l || is_slide_out_r)))
			{
				Judgement current_judgement;
				if (m_note[current_note_index]->type == NOTETYPE_NORMAL)
				{
					current_judgement = m_parent->judge(m_note[current_note_index]->time);
				}
				else if ((m_note[current_note_index]->type == NOTETYPE_SLIDE_IN_LEFT && is_slide_in_l) || (m_note[current_note_index]->type == NOTETYPE_SLIDE_IN_RIGHT && is_slide_in_r))
				{
					current_judgement = m_parent->judge(m_note[current_note_index]->time, true, false, true);
				}
				else if ((m_note[current_note_index]->type == NOTETYPE_SLIDE_END_LEFT && is_slide_in_l) || (m_note[current_note_index]->type == NOTETYPE_SLIDE_END_RIGHT && is_slide_in_r))
				{
					current_judgement = m_parent->judge(m_note[current_note_index]->time, true, false, true);
				}
				else if (m_note[current_note_index]->type == NOTETYPE_SLIDE_THROUGH && (is_slide_out_l || is_slide_out_r))
				{
					current_judgement = m_parent->judge(m_note[current_note_index]->time, true, false, true);
				}
				else
				{
					current_judgement = JUDGEMENT_NONE;
				}

				if (current_judgement != JUDGEMENT_ER && current_judgement != JUDGEMENT_NONE)
				{
					if (m_note[current_note_index]->time != m_note[current_note_index]->time_end)
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
				if (is_pressing_ln && m_note[current_note_index]->time != m_note[current_note_index]->time_end)
				{
					Judgement current_judgement;
					if (m_note[current_note_index]->type_end == NOTETYPE_NORMAL)
					{
						current_judgement = m_parent->judge(m_note[current_note_index]->time_end, true, true, false);
					}
					else if ((m_note[current_note_index]->type_end == NOTETYPE_SLIDE_OUT_LEFT && is_slide_out_l) || (m_note[current_note_index]->type_end == NOTETYPE_SLIDE_OUT_RIGHT && is_slide_out_r))
					{
						current_judgement = m_parent->judge(m_note[current_note_index]->time_end, true, true, false);
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

			if (!is_pressing_ln && current_note_index < m_note.size())
			//以防檢測開頭而導致意外ERROR
			//注意因為之前current_note_index可能執行過++所以還是需要做檢測
			{
				if (m_parent->judge(m_note[current_note_index]->time, false) == JUDGEMENT_ER)
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
				if (m_parent->judge(m_note[current_note_index]->time_end, false) == JUDGEMENT_ER)
				{
					current_note_index++;
					is_pressing_ln = false;
				}
				//长条尾未松开手指
			}
		}
	}
	s_light->update();

	if (System::instance()->IsWindowModified())
	{
		s_light->SetSize(320 * float(System::instance()->GetWindowWidth() / 720.f), 320 * float(System::instance()->GetWindowWidth() / 720.f));
		note_vectrices[1] = System::instance()->GetWindowHeigh();
		note_vectrices[7] = System::instance()->GetWindowHeigh();
		note_vectrices[13] = System::instance()->GetWindowHeigh();
		note_vectrices[19] = System::instance()->GetWindowHeigh();
		int light_pos_x;
		switch (column_index)
		{
			case 0:
//				s_light->SetPos(-38 * float(System::instance()->GetWindowWidth() / 720.f), 1078 * m_parent->GetScaleH() - s_light->GetH() / 2 + Setting::instance()->GetDrawOffset());
				note_vectrices[0] = 20 * float(System::instance()->GetWindowWidth() / 720.f);
				note_vectrices[6] = 187 * float(System::instance()->GetWindowWidth() / 720.f);
				note_vectrices[12] = 20 * float(System::instance()->GetWindowWidth() / 720.f);
				note_vectrices[18] = 187 * float(System::instance()->GetWindowWidth() / 720.f);
				s_light->SetPos(103.5f * float(System::instance()->GetWindowWidth()) / 720.f, System::instance()->GetWindowHeigh(), 84);
				m_x = 0 * float(System::instance()->GetWindowWidth() / 720.f);
				m_w = 166 * float(System::instance()->GetWindowWidth() / 720.f);
			break;
			case 1:
//				s_light->SetPos(119 * float(System::instance()->GetWindowWidth() / 720.f), 1078 * m_parent->GetScaleH() - s_light->GetH() / 2 + Setting::instance()->GetDrawOffset());
				note_vectrices[0] = 191 * float(System::instance()->GetWindowWidth() / 720.f);
				note_vectrices[6] = 358 * float(System::instance()->GetWindowWidth() / 720.f);
				note_vectrices[12] = 191 * float(System::instance()->GetWindowWidth() / 720.f);
				note_vectrices[18] = 358 * float(System::instance()->GetWindowWidth() / 720.f);
				s_light->SetPos(274.5f * float(System::instance()->GetWindowWidth()) / 720.f, System::instance()->GetWindowHeigh(), 84);
				m_x = 166 * float(System::instance()->GetWindowWidth() / 720.f);
				m_w = 194 * float(System::instance()->GetWindowWidth() / 720.f);
			break;
			case 2:
//				s_light->SetPos(282 * float(System::instance()->GetWindowWidth() / 720.f), 1078 * m_parent->GetScaleH() - s_light->GetH() / 2 + Setting::instance()->GetDrawOffset());
				note_vectrices[0] = 362 * float(System::instance()->GetWindowWidth() / 720.f);
				note_vectrices[6] = 529 * float(System::instance()->GetWindowWidth() / 720.f);
				note_vectrices[12] = 362 * float(System::instance()->GetWindowWidth() / 720.f);
				note_vectrices[18] = 529 * float(System::instance()->GetWindowWidth() / 720.f);
				s_light->SetPos(445.5f * float(System::instance()->GetWindowWidth()) / 720.f, System::instance()->GetWindowHeigh(), 84);
				m_x = 360 * float(System::instance()->GetWindowWidth() / 720.f);
				m_w = 194 * float(System::instance()->GetWindowWidth() / 720.f);
			break;
			case 3:
//				s_light->SetPos(443 * float(System::instance()->GetWindowWidth() / 720.f), 1078 * m_parent->GetScaleH() - s_light->GetH() / 2 + Setting::instance()->GetDrawOffset());
				note_vectrices[0] = 533 * float(System::instance()->GetWindowWidth() / 720.f);
				note_vectrices[6] = 700 * float(System::instance()->GetWindowWidth() / 720.f);
				note_vectrices[12] = 533 * float(System::instance()->GetWindowWidth() / 720.f);
				note_vectrices[18] = 700 * float(System::instance()->GetWindowWidth() / 720.f);
				s_light->SetPos(616.5f * float(System::instance()->GetWindowWidth()) / 720.f, System::instance()->GetWindowHeigh(), 84);
				m_x = 515 * float(System::instance()->GetWindowWidth() / 720.f);
				m_w = 166 * float(System::instance()->GetWindowWidth() / 720.f);
			break;
		}
	}
}

void fr::Column::render()
{
	int i = current_note_index;
	bool is_note_in_screen = i < m_note.size();

	while (is_note_in_screen)
	{
		is_note_in_screen = DrawNote(m_note[i]);
		i++;
		is_note_in_screen = is_note_in_screen && i < m_note.size();
	}
	if (is_hold)
	{
//		s_feedback->render();
	}
	s_light->render();
}

void fr::Column::reset()
{
	current_note_index = 0;
}

void fr::Column::AddNote(Note *load_note)
{
	m_note.push_back(load_note);
}

void fr::Column::AddNote(std::vector<Note*> &load_note_list)
{
	m_note.swap(load_note_list);
}

bool fr::Column::DrawNote(Note *load_note)
{
	int time_diff = load_note->time - Setting::instance()->GetOffset() - Timer::instance()->GetTime("game");
	double process = double(Setting::instance()->GetDuration() - time_diff) / double(Setting::instance()->GetDuration());
	int note_z = (System::instance()->GetWindowDepth() - 20 - s_note->GetH()) * (1.f - process) + 20 + s_note->GetH();
	//note時間與當前時間的時間差

	if (time_diff > Setting::instance()->GetDuration())
	{
		//檢測當前note是否在屏幕外面
		return false;
	}

	if (load_note->time != load_note->time_end)
	//是否長條
	{
		int time_diff_end = load_note->time_end - Setting::instance()->GetOffset() - Timer::instance()->GetTime("game");
		//長條尾時間與當前時間的時間差
		double process_end = double(Setting::instance()->GetDuration() - time_diff_end) / double(Setting::instance()->GetDuration());
		int note_z_end = (System::instance()->GetWindowDepth() - 20 - s_note->GetH()) * (1.f - process_end) + 20 + s_note->GetH();
		//時間差轉換成Z坐標 * 2

//		if (is_pressing_ln && load_note->time == m_note[current_note_index]->time && load_note->time_end == m_note[current_note_index]->time_end)
		if (is_pressing_ln && load_note == m_note[current_note_index])
		//用超智障的方法來確認這是繪製的第一個長條
		{
			process = 1;
			time_diff = 0;
			note_z = 20 + s_note->GetH();
		}

		double ln_piece_process = process;
		while (ln_piece_process > (time_diff_end > Setting::instance()->GetDuration() ? 0 : process_end))
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
	s_note->render(((is_pressing_ln && load_note == m_note[current_note_index]) || Setting::instance()->IsSlideOut()) ? NOTETYPE_NORMAL : load_note->type);

	return true;
	//畫note
}