#include "beatmap.h"
#include <string>
#include "column.h"
#include "game_state.h"
#include "result_state.h"
#include "../prepare/song_list.h"
#include "../loading/loading_state.h"
#include "../user/setting.h"
#include "../control_handler.h"
#include "../sound_manager.h"
#include "../song_data.h"
#include "../texture_manager.h"
#include "../file_system.h"
#include "../system.h"
#include "../timer.h"
#include "../text_area.h"
#include "../sprite.h"
#include "../animator.h"

fr::GameBeatmap *fr::GameBeatmap::m_instance = 0;

void fr::Beatmap::init()
{
	play_base = new Sprite;
	play_base->init("assets/game/play_base.png", Rect(0, Setting::instance()->GetDrawOffset(), System::instance()->GetWindowWidth(), System::instance()->GetWindowHeigh() * Setting::instance()->GetDrawScale()));
	scale_w = System::instance()->GetWindowWidth() / 720.f;
	scale_h = System::instance()->GetWindowHeigh() / 1280.f * Setting::instance()->GetDrawScale();
	for (int i = 0; i < 4; i++)
	{
		Column *new_column = new Column;
		new_column->init(i, this);
		m_column.push_back(new_column);
	}
}

void fr::Beatmap::clear()
{
	m_column.clear();
	if (SoundManager::instance()->IsPlayingMusic())
	{
		SoundManager::instance()->stop();
	}
	SoundManager::instance()->clear(audio_path, SOUNDTYPE_MUSIC);
}

void fr::Beatmap::update()
{
	if (System::instance()->IsWindowModified())
	{
		play_base->SetSize(System::instance()->GetWindowWidth(), System::instance()->GetWindowHeigh() * Setting::instance()->GetDrawScale());
		play_base->SetPos(0, Setting::instance()->GetDrawOffset());
		scale_w = System::instance()->GetWindowWidth() / 720.f;
		scale_h = System::instance()->GetWindowHeigh() / 1280.f * Setting::instance()->GetDrawScale();
	}

	for (int i = 0; i < ControlHandler::instance()->GetFingerCount(); i++)
	{
		Finger load_finger = ControlHandler::instance()->GetFinger(i);
		if (load_finger.y < System::instance()->GetWindowHeigh() / 2)
		{
			Setting::instance()->SetDuration(Setting::instance()->GetDuration() - load_finger.dy);
		}
	}

	for (int i = 0; i < m_column.size(); i++)
	{
		m_column[i]->update();
	}
}

void fr::Beatmap::render()
{
	play_base->render();
	for (int i = 0; i < m_column.size(); i++)
	{
		m_column[i]->render();
	}
}

float fr::Beatmap::GetScaleW()
{
	return scale_w;
}

float fr::Beatmap::GetScaleH()
{
	return scale_h;
}

void fr::GameBeatmap::load(fr::SongInformation *load_information)
{
	Beatmap::init();
	m_information = load_information;
	m_score = new Score;
	m_score->pure = 0;
	m_score->great = 0;
	m_score->good = 0;
	m_score->error = 0;
	m_score->score = 0;
	m_score->combo = 0;
	for (int i = 0; i < 4; i++)
	{
		Column *new_column = new Column;
		new_column->init(i, this);
		m_column.push_back(new_column);
	}

	combo_text = new TextArea;
	s_light = new Sprite;
	combo_text->init("READY?", System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2, "assets/fonts/Audiowide.ttf", 140, 0xBB, 0xBB, 0xBB);
	s_light->init("assets/game/light.png", Rect(System::instance()->GetWindowWidth() / 2 - 48, 302 * scale_h + Setting::instance()->GetDrawOffset(), 96, 96 * scale_h));
	TextureManager::instance()->loadfont("assets/fonts/Audiowide.ttf", 140);
	Animator::instance()->AddAnimation("combo", ANIMATIONTYPE_UNIFORMLY_DECELERATED, 300);
	Animator::instance()->ResetAnimation("combo");

	audio_path = m_information->audio_path;
	bool is_mapped = false;
	is_waiting = true;
	is_ended = false;

	std::string text;
	ReadFile(m_information->file_path, text);
	std::vector<Note*> note_list1;
	std::vector<Note*> note_list2;
	std::vector<Note*> note_list3;
	std::vector<Note*> note_list4;

	LoadBeatmapFile(m_information->file_path, NULL, &note_list1, &note_list2, &note_list3, &note_list4);

	m_column[0]->AddNote(note_list1);
	m_column[1]->AddNote(note_list2);
	m_column[2]->AddNote(note_list3);
	m_column[3]->AddNote(note_list4);
	SoundManager::instance()->load(audio_path, SOUNDTYPE_MUSIC);
//	無需加載，因為在選曲界面的時候就加載好了

	Timer::instance()->RunTimer("game");
}

void fr::GameBeatmap::clear()
{
	Beatmap::clear();
	Timer::instance()->ResetTimer("game");
}

void fr::GameBeatmap::update()
{
	Beatmap::update();
	if (Timer::instance()->GetTime("game") > 2000 && is_waiting)
	{
		SoundManager::instance()->play(audio_path, Timer::instance()->GetTime("game") - 2000);
		is_waiting = false;
	}
	if (System::instance()->IsWindowModified())
	{
		combo_text->SetPos(System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2);
		s_light->SetPos(System::instance()->GetWindowWidth() / 2 - 48, 302 * scale_h + Setting::instance()->GetDrawOffset());
		s_light->SetSize(96, 96 * scale_h);
	}

	if (Timer::instance()->GetTime("game") >= m_information->duration + 5000 && !is_ended)
	{
		LoadingState::instance()->init(STATE_RESULT);
		ResultState::instance()->LoadScore(m_information, m_score);
		is_ended = true;
	}
}

void fr::GameBeatmap::render()
{
	Beatmap::render();
	s_light->render();
	combo_text->render(combo_text->GetX(), combo_text->GetY() - 100.f * (1.f - Animator::instance()->GetProcess("combo")));
}

fr::Judgement fr::GameBeatmap::judge(int note_time, bool is_pressed, bool is_ln_pressing, bool error_only)
{
	int time_diff = note_time - Setting::instance()->GetOffset() - Timer::instance()->GetTime("game");
	if (is_pressed)
	{
		if (time_diff > 250 && !is_ln_pressing)
		{
			return JUDGEMENT_NONE;
		}
		else if (time_diff > 150 && is_ln_pressing)
		{
			m_score->score += (m_score->score + JUDGEMENT_ER) > 0 ? JUDGEMENT_ER : -m_score->score;
			//因為error要扣分，為了友好起見不把分數弄到負分
			m_score->combo = 0;
			m_score->error++;

			combo_text->SetColor(0xEC, 0x6A, 0x5C);
			char *combo_ch = new char;
			sprintf(combo_ch, "%d", m_score->combo);
			combo_text->SetText(combo_ch);
			delete combo_ch;

			Animator::instance()->ResetAnimation("combo");
			Animator::instance()->Animate("combo");
			return JUDGEMENT_ER;
		}
		else if (time_diff > 150 && !is_ln_pressing)
		{
			m_score->score += (m_score->score + JUDGEMENT_ER) > 0 ? JUDGEMENT_ER : -m_score->score;
			m_score->combo = 0;
			m_score->error++;

			combo_text->SetColor(0xEC, 0x6A, 0x5C);
			char *combo_ch = new char;
			sprintf(combo_ch, "%d", m_score->combo);
			combo_text->SetText(combo_ch);
			delete combo_ch;

			Animator::instance()->ResetAnimation("combo");
			Animator::instance()->Animate("combo");
			return JUDGEMENT_ER;
		}
		else if (!error_only)
		{
			if (time_diff > 100 || time_diff < -100)
			{
				m_score->score += JUDGEMENT_GD;
				m_score->combo++;
				m_score->good++;

				combo_text->SetColor(0x84, 0xB1, 0xED);
				char *combo_ch = new char;
				sprintf(combo_ch, "%d", m_score->combo);
				combo_text->SetText(combo_ch);
				delete combo_ch;

				Animator::instance()->ResetAnimation("combo");
				Animator::instance()->Animate("combo");
				return JUDGEMENT_GD;
			}
			else if (time_diff > 50 || time_diff < -50)
			{
				m_score->score += JUDGEMENT_GR;
				m_score->combo++;
				m_score->great++;

				combo_text->SetColor(0x81, 0xC7, 0x84);
				char *combo_ch = new char;
				sprintf(combo_ch, "%d", m_score->combo);
				combo_text->SetText(combo_ch);
				delete combo_ch;

				Animator::instance()->ResetAnimation("combo");
				Animator::instance()->Animate("combo");
				return JUDGEMENT_GR;
			}
			else
			{
				m_score->score += JUDGEMENT_PG;
				m_score->combo++;
				m_score->pure++;

				combo_text->SetColor(0xFF, 0xEA, 0x00);
				char *combo_ch = new char;
				sprintf(combo_ch, "%d", m_score->combo);
				combo_text->SetText(combo_ch);
				delete combo_ch;

				Animator::instance()->ResetAnimation("combo");
				Animator::instance()->Animate("combo");
				return JUDGEMENT_PG;
			}
		}
		else
		{
			return JUDGEMENT_SAFE;
		}
	}
	else if (time_diff < -150)
	{
		m_score->score += (m_score->score + JUDGEMENT_ER) > 0 ? JUDGEMENT_ER : -m_score->score;
		m_score->combo = 0;
		m_score->error++;

		combo_text->SetColor(0xEC, 0x6A, 0x5C);
		char *combo_ch = new char;
		sprintf(combo_ch, "%d", m_score->combo);
		combo_text->SetText(combo_ch);
		delete combo_ch;

		Animator::instance()->ResetAnimation("combo");
		Animator::instance()->Animate("combo");
		return JUDGEMENT_ER;
	}
	return JUDGEMENT_NONE;
}	//Judgement fr::GameBeatmap::judge()

int fr::GameBeatmap::GetCombo()
{
	return m_score->combo;
}

int fr::GameBeatmap::GetScore()
{
	return m_score->score;
}