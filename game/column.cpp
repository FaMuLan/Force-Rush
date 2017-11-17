#include "column.h"
#include "beatmap.h"
#include "../sprite.h"
#include "../control_handler.h"
#include "../system.h"
#include "../timer.h"

void lm::Column::init(int column_index)
{
	m_h = 45;
	current_note_index = 0;
	is_pressing_ln = false;
	s_note = new Sprite;
	s_light = new Sprite;
	int current_w, current_h;

	s_light->init("", 0, 0, 473, 473);
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
			current_w = 173.0f / 720.0f * System::instance()->GetWindowWidth();
			current_h = 45.0f / 1280.0f * System::instance()->GetWindowHeigh();
			start_x = 333;
			start_y = 387;
			end_x = 36;
			end_y = 1088;
			m_x = 0;
			m_w = 166;
			start_scale = 0.0828f;
			s_note->init("assets/game/note_1.png", 0, 0, current_w, current_h);
			s_light->SetPos(-114, 872);
		break;
		case 1:
			start_x = 347;
			start_y = 387;
			end_x = 197;
			end_y = 1088;
			m_x = 166;
			m_w = 194;
			start_scale = 0.0828f;
			current_w = 165.0f / 720.0f * System::instance()->GetWindowWidth();
			current_h = 45.0f / 1280.0f * System::instance()->GetWindowHeigh();
			s_note->init("assets/game/note_2.png", 0, 0, current_w, current_h);
			s_light->SetPos(43, 872);
		break;
		case 2:
			start_x = 360;
			start_y = 387;
			end_x = 360;
			end_y = 1088;
			m_x = 360;
			m_w = 194;
			start_scale = 0.0828f;
			current_w = 165.0f / 720.0f * System::instance()->GetWindowWidth();
			current_h = 45.0f / 1280.0f * System::instance()->GetWindowHeigh();
			s_note->init("assets/game/note_3.png", 0, 0, current_w, current_h);
			s_light->SetPos(206, 872);
		break;
		case 3:
			start_x = 371;
			start_y = 387;
			end_x = 515;
			end_y = 1088;
			m_x = 515;
			m_w = 166;
			start_scale = 0.0828f;
			current_w = 173.0f / 720.0f * System::instance()->GetWindowWidth();
			current_h = 45.0f / 1280.0f * System::instance()->GetWindowHeigh();
			s_note->init("assets/game/note_4.png", 0, 0, current_w, current_h);
			s_light->SetPos(367, 872);
		break;
	}

	is_tapped = false;
}	//void lm::Column::init(int column_index)

void lm::Column::clear()
{
	std::vector<Note*>(m_note).swap(m_note);
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
//================ Auto Mod =========

	if (current_note_index < m_note.size())
	{
		if (is_pressing_ln)
		{
			if (m_note[current_note_index]->time_end < Timer::instance()->GetTime("game"))
			{
				is_released = true;
			}
		}
		if (m_note[current_note_index]->time < Timer::instance()->GetTime("game") && !is_pressing_ln)
		{
			is_tapped = true;
		}
	}

//=================== End ===========
	if (current_note_index < m_note.size())
	//檢測防止下標越界而導致段錯誤
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
				if (Beatmap::instance()->judge(m_note[current_note_index]->time_end, true, true) != JUDGEMENT_ER)
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
			if (Beatmap::instance()->judge(m_note[current_note_index]->time, false) == JUDGEMENT_ER)
			{
				current_note_index++;
			}
		}

		if (is_pressing_ln)
		{
			if (Beatmap::instance()->judge(m_note[current_note_index]->time_end, false) == JUDGEMENT_ER)
			{
				current_note_index++;
				is_pressing_ln = false;
			}
			//长条尾未松开手指
		}
	}
	s_light->update();
}

void lm::Column::render()
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

void lm::Column::AddNote(Note *load_note)
{
	m_note.push_back(load_note);
}

bool lm::Column::DrawNote(int time, int time_end)
{
	int time_diff = time - Timer::instance()->GetTime("game");
	double process = float(Beatmap::instance()->GetDuration() - time_diff) / double(Beatmap::instance()->GetDuration());
	process *= process;
	int current_x = start_x + (end_x - start_x) * process;
	int current_y = start_y + (end_y - start_y) * process;
	float current_scale = start_scale + (1.0f - start_scale) * process;
	//note時間與當前時間的時間差

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
		double process_end = float(Beatmap::instance()->GetDuration() - time_diff_end) / double(Beatmap::instance()->GetDuration());
		process_end *= process_end;
		//時間差轉換成Y坐標 * 2

		if (is_pressing_ln && time == m_note[current_note_index]->time && time_end == m_note[current_note_index]->time_end)
		//用超智障的方法來確認這是繪製的第一個長條
		{
			process = 1;
			time_diff = 0;
			current_x = start_x + (end_x - start_x) * process;
			current_y = start_y + (end_y - start_y) * process;
			current_scale = start_scale + (1.0f - start_scale) * process;
		}

		double ln_piece_process = process;
		while (ln_piece_process > (time_diff_end > Beatmap::instance()->GetDuration() ? 0 : process_end))
		//長條身不超過屏幕 且 不超過尾部 時畫出來，循環
		{
			int current_x_piece = start_x + (end_x - start_x) * ln_piece_process;
			int current_y_piece = start_y + (end_y - start_y) * ln_piece_process;
			float current_scale_piece = start_scale + (1.0f - start_scale) * ln_piece_process;
			s_note->SetPos(current_x_piece, current_y_piece);
			s_note->SetScale(current_scale_piece);
			s_note->render();
			//將長條身往上挪動
			ln_piece_process -= 0.01f;
		}

		if (ln_piece_process < 0)
		{
			if (current_y < System::instance()->GetWindowHeigh())
			{
				s_note->SetPos(current_x, current_y);
				s_note->SetScale(current_scale);
				s_note->render();
			}
			return false;
		}
		else
		{
			int current_x_end = start_x + (end_x - start_x) * process_end;
			int current_y_end = start_y + (end_y - start_y) * process_end;
			float current_scale_end = start_scale + (1.0f - start_scale) * process_end;
			s_note->SetPos(current_x_end, current_y_end);
			s_note->SetScale(current_scale_end);
			s_note->render();
		}
		//畫長條尾
	}

	if (current_y < System::instance()->GetWindowHeigh())
	{
		s_note->SetPos(current_x, current_y);
		s_note->SetScale(current_scale);
		s_note->render();
	}

	return true;
	//畫note
}