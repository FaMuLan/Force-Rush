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
			Setting::instance()->SetDuration(Setting::instance()->GetDuration() + load_finger.dy);
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
	m_score->safe = 0;
	m_score->warning = 0;
	m_score->error = 0;
	m_score->score = 0;
	m_score->chain = 0;
	current_chain = 0;
	for (int i = 0; i < 4; i++)
	{
		Column *new_column = new Column;
		new_column->init(i, this);
		m_column.push_back(new_column);
	}

	chain_text = new TextArea;
	judge_text = new TextArea;
	early_text = new TextArea;
	late_text = new TextArea;
	show_early = false;
	show_late = false;
	s_light = new Sprite;
	chain_text->init(" ", System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2, "assets/fonts/Audiowide.ttf", 140, 0xBB, 0xBB, 0xBB);
	judge_text->init(" ", System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2 + 120, "assets/fonts/Audiowide.ttf", 40, 0xBB, 0xBB, 0xBB);
	early_text->init("EARLY", System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2 + 80, "assets/fonts/Audiowide.ttf", 40, 0xBB, 0xBB, 0xBB);
	late_text->init("LATE", System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2 + 160, "assets/fonts/Audiowide.ttf", 40, 0xBB, 0xBB, 0xBB);
	s_light->init("assets/game/light.png", Rect(System::instance()->GetWindowWidth() / 2 - 48, 302 * scale_h + Setting::instance()->GetDrawOffset(), 96, 96 * scale_h));
	TextureManager::instance()->loadfont("assets/fonts/Audiowide.ttf", 140);
	TextureManager::instance()->loadfont("assets/fonts/Audiowide.ttf", 40);
	Animator::instance()->AddAnimation("chain", ANIMATIONTYPE_UNIFORMLY_DECELERATED, 300);
	Animator::instance()->ResetAnimation("chain");

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
		chain_text->SetPos(System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2);
		judge_text->SetPos(System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2 + 120);
		early_text->SetPos(System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2 + 80);
		late_text->SetPos(System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2 + 160);
		s_light->SetPos(System::instance()->GetWindowWidth() / 2 - 48, 302 * scale_h + Setting::instance()->GetDrawOffset());
		s_light->SetSize(96, 96 * scale_h);
	}

	if (Timer::instance()->GetTime("game") >= m_information->duration + 5000 && !is_ended)
	{
		LoadingState::instance()->init(STATE_RESULT);
		ResultState::instance()->LoadScore(m_information, m_score);
		is_ended = true;
	}

	if (show_early && Timer::instance()->GetTime("show_early") > 200)
	{
		show_early = false;
		Timer::instance()->ResetTimer("show_early");
	}
	if (show_late && Timer::instance()->GetTime("show_late") > 200)
	{
		show_late = false;
		Timer::instance()->ResetTimer("show_late");
	}
}

void fr::GameBeatmap::render()
{
	Beatmap::render();
	s_light->render();
	chain_text->render(chain_text->GetX(), chain_text->GetY() - 100.f * (1.f - Animator::instance()->GetProcess("chain")));
	if (show_early)
	{
		early_text->render();
	}
	if (show_late)
	{
		late_text->render();
	}
	judge_text->render();
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
			current_chain = 0;
			m_score->error++;

			char *chain_ch = new char;
			sprintf(chain_ch, "%d", current_chain);
			chain_text->SetText(chain_ch);
			delete chain_ch;
			show_early = true;
			Timer::instance()->ResetTimer("show_early");
			Timer::instance()->RunTimer("show_early");
			judge_text->SetText("ERROR");

			Animator::instance()->ResetAnimation("chain");
			Animator::instance()->Animate("chain");
			return JUDGEMENT_ER;
		}
		else if (time_diff > 150 && !is_ln_pressing)
		{
			m_score->score += (m_score->score + JUDGEMENT_ER) > 0 ? JUDGEMENT_ER : -m_score->score;
			current_chain = 0;
			m_score->error++;

			char *chain_ch = new char;
			sprintf(chain_ch, "%d", current_chain);
			chain_text->SetText(chain_ch);
			delete chain_ch;
			show_early = true;
			Timer::instance()->ResetTimer("show_early");
			Timer::instance()->RunTimer("show_early");
			judge_text->SetText("ERROR");

			Animator::instance()->ResetAnimation("chain");
			Animator::instance()->Animate("chain");
			return JUDGEMENT_ER;
		}
		else if (!error_only)
		{
			if (time_diff > 100 || time_diff < -100)
			{
				m_score->score += (m_score->score + JUDGEMENT_GD) > 0 ? JUDGEMENT_GD : -m_score->score;
				current_chain++;
				m_score->warning++;

				char *chain_ch = new char;
				char *judge_ch = new char[20];
				sprintf(chain_ch, "%d", current_chain);
				sprintf(judge_ch, "WARNING %dms", time_diff);
				chain_text->SetText(chain_ch);
				judge_text->SetText(judge_ch);
				delete chain_ch;
				delete judge_ch;
				if (time_diff > 0)
				{	
					show_early = true;
					Timer::instance()->ResetTimer("show_early");
					Timer::instance()->RunTimer("show_early");
				}
				else
				{
					show_late = true;
					Timer::instance()->ResetTimer("show_late");
					Timer::instance()->RunTimer("show_late");
				}

				Animator::instance()->ResetAnimation("chain");
				Animator::instance()->Animate("chain");
				return JUDGEMENT_GD;
			}
			else if (time_diff > 50 || time_diff < -50)
			{
				m_score->score += JUDGEMENT_GR;
				current_chain++;
				m_score->safe++;

				char *chain_ch = new char;
				char *judge_ch = new char[20];
				sprintf(chain_ch, "%d", current_chain);
				sprintf(judge_ch, "SAFE %dms", time_diff);
				chain_text->SetText(chain_ch);
				judge_text->SetText(judge_ch);
				delete chain_ch;
				delete judge_ch;
				if (time_diff > 0)
				{	
					show_early = true;
					Timer::instance()->ResetTimer("show_early");
					Timer::instance()->RunTimer("show_early");
				}
				else
				{
					show_late = true;
					Timer::instance()->ResetTimer("show_late");
					Timer::instance()->RunTimer("show_late");
				}

				Animator::instance()->ResetAnimation("chain");
				Animator::instance()->Animate("chain");
				return JUDGEMENT_GR;
			}
			else
			{
				m_score->score += JUDGEMENT_PG;
				current_chain++;
				m_score->pure++;

				char *chain_ch = new char;
				sprintf(chain_ch, "%d", current_chain);
				chain_text->SetText(chain_ch);
				delete chain_ch;
				judge_text->SetText("PURE");

				Animator::instance()->ResetAnimation("chain");
				Animator::instance()->Animate("chain");
				return JUDGEMENT_PG;
			}
			m_score->chain = current_chain > m_score->chain ? current_chain : m_score->chain;
		}
		else
		{
			return JUDGEMENT_SAFE;
		}
	}
	else if (time_diff < -150)
	{
		m_score->score += (m_score->score + JUDGEMENT_ER) > 0 ? JUDGEMENT_ER : -m_score->score;
		current_chain = 0;
		m_score->error++;

		char *chain_ch = new char;
		sprintf(chain_ch, "%d", current_chain);
		chain_text->SetText(chain_ch);
		delete chain_ch;
		judge_text->SetText("ERROR");
		show_late = true;
		Timer::instance()->ResetTimer("show_late");
		Timer::instance()->RunTimer("show_late");

		Animator::instance()->ResetAnimation("chain");
		Animator::instance()->Animate("chain");
		return JUDGEMENT_ER;
	}
	return JUDGEMENT_NONE;
}	//Judgement fr::GameBeatmap::judge()

int fr::GameBeatmap::GetChain()
{
	return m_score->chain;
}

int fr::GameBeatmap::GetScore()
{
	return m_score->score;
}