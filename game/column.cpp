#include "column.h"
#include "beatmap.h"
#include "../sprite.h"
#include "../control_handler.h"
#include "../system.h"
#include "../timer.h"

void lm::Column::init(int x, int w)
{
	m_x = x;
	m_w = w;
	m_h = 80;
	current_note_index = 0;
	is_pressing_ln = false;
	s_note = new Sprite;
	s_note_ln_body = new Sprite;
	s_note_ln_end = new Sprite;
	s_judge_line = new Sprite;
	s_note->init("assets/whiteblue/Key-Mod/Note/White.png", m_x, 0, m_w, m_h);
	s_note_ln_body->init("assets/whiteblue/Key-Mod/Note/wHoldBody.png", m_x, 0, m_w, m_h);
	s_note_ln_end->init("assets/whiteblue/Key-Mod/Note/wHoldEnd.png", m_x, 0, m_w, m_h);
	s_judge_line->init("assets/whiteblue/Key-Mod/Key/White-Up.png", m_x, System::instance()->GetWindowHeigh() - m_h, m_w, m_h);
	is_tapped = false;
}

void lm::Column::clear()
{
	m_note.clear();
}

void lm::Column::update()
{
	is_released = false;
	is_tapped = false;
	if (!is_hold)
	{
		for (int i = 0; i < ControlHandler::instance()->GetFingerCount(); i++)
		{
			Finger load_finger = ControlHandler::instance()->GetFinger(i);
			if (load_finger.x >= m_x && load_finger.x <= (m_x + m_w))
			{
				if (!load_finger.moved)
				{
					is_hold = true;
					is_tapped = true;
					has_pressed_id = load_finger.id;
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < ControlHandler::instance()->GetFingerCount(); i++)
		{
			Finger load_finger = ControlHandler::instance()->GetFinger(i);
			if (load_finger.id == has_pressed_id)
			{
				if (load_finger.x < m_x || load_finger.x > (m_x + m_w))
				{
					is_hold = false;
				}
				if (load_finger.released)
				{
					is_released = true;
					is_hold = false;
				}
			}
		}
	}

	if (current_note_index < m_note.size())
	{
		if (is_tapped)
		{
			Judgement current_judgement = Beatmap::instance()->judge(m_note[current_note_index]->time);
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
			}
			else if (current_judgement == JUDGEMENT_ER)
			{
				current_note_index++;
				//誤觸就ERROR的話，別說了，下一個note，請
			}
		}

		if (is_released)
		{
			is_pressing_ln = false;
			if (is_pressing_ln && m_note[current_note_index]->time != m_note[current_note_index]->time_end)
			{
				Beatmap::instance()->judge(m_note[current_note_index]->time_end, true, true);
				current_note_index++;
			}
		}

		if (!is_pressing_ln)
		//以防檢測開頭而導致意外ERROR
		{
			if (Beatmap::instance()->judge(m_note[current_note_index]->time, false) == JUDGEMENT_ER)
			{
				current_note_index++;
			}
		}
	}
}

void lm::Column::render()
{
	s_judge_line->render();
	int i = current_note_index;
	bool is_note_in_screen = i < m_note.size();

	while (is_note_in_screen)
	{
		is_note_in_screen = DrawNote(m_note[i]->time, m_note[i]->time_end);
		is_note_in_screen = is_note_in_screen && i < m_note.size();
		i++;
	}
}

void lm::Column::AddNote(Note *load_note)
{
	m_note.push_back(load_note);
}

bool lm::Column::DrawNote(int time, int time_end)
{
	int time_diff = time - Timer::instance()->GetTime("game");
	//note時間與當前時間的時間差
	int current_y = System::instance()->GetWindowHeigh() * (float(Beatmap::instance()->GetDuration() - time_diff) / float(Beatmap::instance()->GetDuration())) - m_h;
	//時間差轉換成Y坐標
	//屏幕頂端到判定線距離 * 屏幕頂端到判定線所用時間與note出現時間之比 - 偏移

	if (time_diff > Beatmap::instance()->GetDuration())
	{
		//檢測當前note是否在屏幕外面
		return false;
	}

	if (time != time_end)
	//是否長條
	{
		int time_diff_end = time_end - Timer::instance()->GetTime("game");
		//長條尾時間與當前時間的時間差
		int current_y_end = System::instance()->GetWindowHeigh() * (float(Beatmap::instance()->GetDuration() - time_diff_end) / float(Beatmap::instance()->GetDuration())) - m_h;
		//時間差轉換成Y坐標 * 2
		if (is_pressing_ln)
		{
			current_y = System::instance()->GetWindowHeigh() - m_h;
		}
		int ln_piece_pos = current_y - m_h / 2;
		//長條身Y坐標
		while (ln_piece_pos > -m_h && ln_piece_pos > current_y_end)
		//長條身不超過屏幕 且 不超過尾部 時畫出來，循環
		{
			if (ln_piece_pos < System::instance()->GetWindowHeigh())
			{
				s_note_ln_body->SetPos(m_x, ln_piece_pos);
				s_note_ln_body->render();
				//將長條身往上挪動
			}
			ln_piece_pos -= m_h;
		}

		if (ln_piece_pos < -m_h)
		{
			if (current_y < System::instance()->GetWindowHeigh())
			{
				s_note->SetPos(m_x, current_y);
				s_note->render();
			}
			return false;
		}
		else
		{
			s_note_ln_end->SetPos(m_x, current_y_end);
			s_note_ln_end->render();
		}
		//畫長條尾
	}

	if (current_y < System::instance()->GetWindowHeigh())
	{
		s_note->SetPos(m_x, current_y);
		s_note->render();
	}
	return true;
	//畫note
}