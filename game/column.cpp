#include "column.h"
#include "beatmap.h"
#include "../sprite.h"
#include "../control_handler.h"
#include "../system.h"
#include "../timer.h"
#include "../user/setting.h"

void fr::Column::init(int load_column_index, Beatmap *parent)
{
	m_parent = parent;
	column_index = load_column_index;
	m_h = 45;
	current_note_index = 0;
	is_pressing_ln = false;
	s_note = new Sprite;
	s_light = new Sprite;
	int current_w, current_h;

	s_light->init("", 0, 0, 473 * m_parent->GetScaleW(), 473 * m_parent->GetScaleW());
	s_light->AddFrame("assets/game/lightingN-0.png");
	s_light->AddFrame("assets/game/lightingN-1.png");
	s_light->AddFrame("assets/game/lightingN-2.png");
	s_light->AddFrame("assets/game/lightingN-3.png");
	s_light->AddFrame("assets/game/lightingN-4.png");
	s_light->AddFrame("assets/game/lightingN-5.png");
	s_light->AddFrame("assets/game/lightingN-6.png");
	s_light->AddFrame("assets/game/lightingN-7.png");
	s_light->AddFrame("assets/game/lightingN-8.png");
	s_light->AddFrame("assets/game/lightingN-9.png");
	s_light->AddFrame("assets/game/lightingN-10.png");
	s_light->AddFrame("assets/game/lightingN-11.png");

	switch (column_index)
	{
		case 0:
			current_w = 173.0f * m_parent->GetScaleW();
			current_h = 45.0f * m_parent->GetScaleH();
			start_x = 333;
			start_y = 387;
			end_x = 36;
			end_y = 1088;
			m_x = 0;
			m_w = 166 * m_parent->GetScaleW();
			keyboard_key = Setting::instance()->GetKeycode(column_index);
			start_scale = 0.0828f;
			s_note->init("assets/game/note_1.png", 0, 0, current_w, current_h);
			s_light->SetPos(-114 * m_parent->GetScaleW(), 1108 * m_parent->GetScaleH() - s_light->GetH() / 2);
		break;
		case 1:
			current_w = 165.0f * m_parent->GetScaleW();
			current_h = 45.0f * m_parent->GetScaleH();
			start_x = 347;
			start_y = 387;
			end_x = 197;
			end_y = 1088;
			m_x = 166 * m_parent->GetScaleW();
			m_w = 194 * m_parent->GetScaleW();
			keyboard_key = Setting::instance()->GetKeycode(column_index);
			start_scale = 0.0828f;
			s_note->init("assets/game/note_2.png", 0, 0, current_w, current_h);
			s_light->SetPos(43 * m_parent->GetScaleW(), 1108 * m_parent->GetScaleH() - s_light->GetH() / 2);
		break;
		case 2:
			current_w = 165.0f * m_parent->GetScaleW();
			current_h = 45.0f * m_parent->GetScaleH();
			start_x = 360;
			start_y = 387;
			end_x = 360;
			end_y = 1088;
			m_x = 360 * m_parent->GetScaleW();
			m_w = 194 * m_parent->GetScaleW();
			keyboard_key = Setting::instance()->GetKeycode(column_index);
			start_scale = 0.0828f;
			s_note->init("assets/game/note_3.png", 0, 0, current_w, current_h);
			s_light->SetPos(206 * m_parent->GetScaleW(), 1108 * m_parent->GetScaleH() - s_light->GetH() / 2);
		break;
		case 3:
			current_w = 173.0f * m_parent->GetScaleW();
			current_h = 45.0f * m_parent->GetScaleH();
			start_x = 371;
			start_y = 387;
			end_x = 515;
			end_y = 1088;
			m_x = 515 * m_parent->GetScaleW();
			m_w = 166 * m_parent->GetScaleW();
			keyboard_key = Setting::instance()->GetKeycode(column_index);
			start_scale = 0.0828f;
			s_note->init("assets/game/note_4.png", 0, 0, current_w, current_h);
			s_light->SetPos(367 * m_parent->GetScaleW(), 1108 * m_parent->GetScaleH() - s_light->GetH() / 2);
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
	if (!is_hold)
	{
//================= Touch Device ====================
		for (int i = 0; i < ControlHandler::instance()->GetFingerCount(); i++)
		{
			Finger load_finger = ControlHandler::instance()->GetFinger(i);
			if (load_finger.x >= m_x && load_finger.x <= (m_x + m_w) && load_finger.y > System::instance()->GetWindowWidth() / 2)
			{
				if (!load_finger.moved)
				{
					is_hold = true;
					is_tapped = true;
					is_touch_pressed = true;
					has_pressed_id = load_finger.id;
				}
			}
		}
//================= Keyboard Device =================
		if (ControlHandler::instance()->IsKeyDown(keyboard_key))
		{
			is_hold = true;
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
					if ((load_finger.x < m_x || load_finger.x > (m_x + m_w)) || load_finger.y < System::instance()->GetWindowWidth() / 2)
					{
						is_hold = false;
						is_touch_pressed = false;
					}
					if (load_finger.released)
					{
						is_released = true;
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
				}
			}
			if (m_note[current_note_index]->time - Setting::instance()->GetOffset() < Timer::instance()->GetTime("game") && !is_pressing_ln)
			{
				is_tapped = true;
			}
		}
	}
//=================== End ===========
	if (current_note_index < m_note.size())
	//檢測防止下標越界而導致段錯誤
	{
		if (is_tapped)
		{
			Judgement current_judgement = m_parent->judge(m_note[current_note_index]->time);
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
				s_light->SetAnimate(1, 12, 300);
			}
			else if (current_judgement == JUDGEMENT_ER)
			{
				current_note_index++;
				//誤觸就ERROR的話，別說了，下一個note，請
			}
		}

		if (is_released)
		{
			if (is_pressing_ln && m_note[current_note_index]->time != m_note[current_note_index]->time_end)
			{
				if (m_parent->judge(m_note[current_note_index]->time_end, true, true) != JUDGEMENT_ER)
				{
					s_light->SetAnimate(1, 12, 300);
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
			if (m_parent->judge(m_note[current_note_index]->time_end, false) == JUDGEMENT_ER)
			{
				current_note_index++;
				is_pressing_ln = false;
			}
			//长条尾未松开手指
		}
	}
	s_light->update();

	if (System::instance()->IsWindowModified())
	{
		s_light->SetSize(473 * m_parent->GetScaleW(), 473 * m_parent->GetScaleW());
		switch (column_index)
		{
			case 0:
				s_note->SetSize(173.0f * m_parent->GetScaleW(), 45.0f * m_parent->GetScaleH());
				s_light->SetPos(-114 * m_parent->GetScaleW(), 1108 * m_parent->GetScaleH() - s_light->GetH() / 2);
				m_x = 0 * m_parent->GetScaleW();
				m_w = 166 * m_parent->GetScaleW();
			break;
			case 1:
				s_note->SetSize(165.0f * m_parent->GetScaleW(), 45.0f * m_parent->GetScaleH());
				s_light->SetPos(43 * m_parent->GetScaleW(), 1108 * m_parent->GetScaleH() - s_light->GetH() / 2);
				m_x = 166 * m_parent->GetScaleW();
				m_w = 194 * m_parent->GetScaleW();
			break;
			case 2:
				s_note->SetSize(165.0f * m_parent->GetScaleW(), 45.0f * m_parent->GetScaleH());
				s_light->SetPos(206 * m_parent->GetScaleW(), 1108 * m_parent->GetScaleH() - s_light->GetH() / 2);
				m_x = 360 * m_parent->GetScaleW();
				m_w = 194 * m_parent->GetScaleW();
			break;
			case 3:
				s_note->SetSize(173.0f * m_parent->GetScaleW(), 45.0f * m_parent->GetScaleH());
				s_light->SetPos(367 * m_parent->GetScaleW(), 1108 * m_parent->GetScaleH() - s_light->GetH() / 2);
				m_x = 515 * m_parent->GetScaleW();
				m_w = 166 * m_parent->GetScaleW();
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
		is_note_in_screen = DrawNote(m_note[i]->time, m_note[i]->time_end);
		i++;
		is_note_in_screen = is_note_in_screen && i < m_note.size();
	}
	s_light->render();
}

void fr::Column::reset()
{
	column_index = 0;
}

void fr::Column::AddNote(Note *load_note)
{
	m_note.push_back(load_note);
}

bool fr::Column::DrawNote(int time, int time_end)
{
	int time_diff = time - Setting::instance()->GetOffset() - Timer::instance()->GetTime("game");
	double process = double(Setting::instance()->GetDuration() - time_diff) / double(Setting::instance()->GetDuration());
	double process_sq = process * process;
	int current_x = start_x + (end_x - start_x) * process_sq;
	int current_y = start_y + (end_y - start_y) * process_sq;
	float current_scale = start_scale + (1.0f - start_scale) * process_sq;
	//note時間與當前時間的時間差

	if (time_diff > Setting::instance()->GetDuration())
	{
		//檢測當前note是否在屏幕外面
		return false;
	}

	if (time != time_end)
	//是否長條
	{
		int time_diff_end = time_end - Setting::instance()->GetOffset() - Timer::instance()->GetTime("game");
		//長條尾時間與當前時間的時間差
		double process_end = double(Setting::instance()->GetDuration() - time_diff_end) / double(Setting::instance()->GetDuration());
		double process_end_sq = process_end * process_end;
		//時間差轉換成Y坐標 * 2

		if (is_pressing_ln && time == m_note[current_note_index]->time && time_end == m_note[current_note_index]->time_end)
		//用超智障的方法來確認這是繪製的第一個長條
		{
			process = 1;
			process_sq = 1;
			time_diff = 0;
			current_x = start_x + (end_x - start_x) * process_sq;
			current_y = start_y + (end_y - start_y) * process_sq;
			current_scale = start_scale + (1.0f - start_scale) * process_sq;
		}

		double ln_piece_process = process;
		double ln_piece_process_sq = ln_piece_process * ln_piece_process;
		while (ln_piece_process > (time_diff_end > Setting::instance()->GetDuration() ? 0 : process_end))
		//長條身不超過屏幕 且 不超過尾部 時畫出來，循環
		{
			int current_x_piece = start_x + (end_x - start_x) * ln_piece_process_sq;
			int current_y_piece = start_y + (end_y - start_y) * ln_piece_process_sq;
			float current_scale_piece = start_scale + (1.0f - start_scale) * ln_piece_process_sq;
			s_note->SetPos(current_x_piece * m_parent->GetScaleW(), current_y_piece * m_parent->GetScaleH());
			s_note->SetScale(current_scale_piece);
			s_note->render();
			//將長條身往上挪動
			ln_piece_process -= 0.01f;
			ln_piece_process_sq = ln_piece_process * ln_piece_process;
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
			int current_x_end = start_x + (end_x - start_x) * process_end_sq;
			int current_y_end = start_y + (end_y - start_y) * process_end_sq;
			float current_scale_end = start_scale + (1.0f - start_scale) * process_end_sq;
			s_note->SetPos(current_x_end * m_parent->GetScaleW(), current_y_end * m_parent->GetScaleH());
			s_note->SetScale(current_scale_end);
			s_note->render();
		}
		//畫長條尾
	}

	if (current_y * m_parent->GetScaleH() < System::instance()->GetWindowHeigh())
	{
		s_note->SetPos(current_x * m_parent->GetScaleW(), current_y * m_parent->GetScaleH());
		s_note->SetScale(current_scale);
		s_note->render();
	}

	return true;
	//畫note
}