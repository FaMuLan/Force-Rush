#include "beatmap.h"
#include <string>
#include <regex>
#include "column.h"
#include "game_state.h"
#include "result_state.h"
#include "../select/song_list.h"
#include "../loading/loading_state.h"
#include "../user/setting.h"
#include "../message_box.h"
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
	play_base->init("assets/game/play_base.png", 0, 0, System::instance()->GetWindowWidth(), System::instance()->GetWindowHeigh());
	note_duration = Setting::instance()->GetDuration();
	offset = Setting::instance()->GetOffset();
	scale_w = System::instance()->GetWindowWidth() / 720.f;
	scale_h = System::instance()->GetWindowHeigh() / 1280.f;
}

void fr::Beatmap::clear()
{
	m_column.clear();
	if (SoundManager::instance()->IsPlayingMusic())
	{
		SoundManager::instance()->stop();
	}
}

void fr::Beatmap::update()
{
	if (System::instance()->IsWindowModified())
	{
		play_base->SetSize(System::instance()->GetWindowWidth(), System::instance()->GetWindowHeigh());
		scale_w = System::instance()->GetWindowWidth() / 720.f;
		scale_h = System::instance()->GetWindowHeigh() / 1280.f;
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

	combo_text = new TextArea;
	combo_text->init("", System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2, "assets/fonts/Audiowide.ttf", 240, 0, 0, 0);
	TextureManager::instance()->loadfont("assets/fonts/Audiowide.ttf", 240);
	Animator::instance()->AddAnimation("combo", ANIMATIONTYPE_UNIFORMLY_DECELERATED, 100);
	Animator::instance()->ResetAnimation("combo");

	audio_path = GetParentDir(m_information->file_path);
	bool is_mapped = false;
	is_waiting = true;
	is_ended = false;

	std::string text;
	ReadFile(m_information->file_path, text);
	std::regex audio_path_pattern("AudioFilename: (.*)");
	std::regex note_pattern("(\\d+),\\d+,(\\d+),(\\d+),\\d+,(\\d+):\\d+:\\d+:\\d+:(\\d+:)?");

	std::smatch audio_path_line;
	std::regex_search(text, audio_path_line, audio_path_pattern);
	audio_path += std::regex_replace(audio_path_line.str(), audio_path_pattern, "$1");

	for (std::sregex_iterator i = std::sregex_iterator(text.begin(), text.end(), note_pattern); i != std::sregex_iterator(); i++)
	{
		std::smatch line = *i;
		Note *new_note = new Note;
		new_note->time = atoi(std::regex_replace(line.str(), note_pattern, "$2").c_str()) - offset + 2000;
		int type = atoi(std::regex_replace(line.str(), note_pattern, "$3").c_str());
		new_note->time_end = (type % 16 == 0) ? atoi(std::regex_replace(line.str(), note_pattern, "$4").c_str()) - offset + 2000 : new_note->time;
		int column_index = atoi(std::regex_replace(line.str(), note_pattern, "$1").c_str());
		if (!is_mapped)
		{
			switch (column_index)
			{
				case 64:
				case 192:
				case 320:
				case 448:
					for (int i = 0; i < 4; i++)
					{
						Column *new_column = new Column;
						new_column->init(i);
						m_column.push_back(new_column);
					}
					column_mapper[64] = 0;
					column_mapper[192] = 1;
					column_mapper[320] = 2;
					column_mapper[448] = 3;
				break;
			}
			is_mapped = true;
		}
		m_column[column_mapper[column_index]]->AddNote(new_note);
	}
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
		SoundManager::instance()->play(audio_path, SOUNDTYPE_MUSIC);
		is_waiting = false;
	}
	if (System::instance()->IsWindowModified())
	{
		combo_text->SetPos(System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2);
	}

	if (Timer::instance()->GetTime("game") >= m_information->duration + 5000 && !is_ended)
	{
		LoadingState::instance()->init(ResultState::instance(), GameState::instance());
		ResultState::instance()->LoadScore(m_information, m_score);
		is_ended = true;
	}
}

void fr::GameBeatmap::render()
{
	Beatmap::render();
	combo_text->render(combo_text->GetX(), combo_text->GetY() - 100.f * (1.f - Animator::instance()->GetProcess("combo")));
}

fr::Judgement fr::GameBeatmap::judge(int note_time, bool is_pressed, bool is_ln_pressing)
{
	int time_diff = note_time - Timer::instance()->GetTime("game");
	if (is_pressed)
	{
		if (time_diff > 500 && !is_ln_pressing)
		{
			return JUDGEMENT_NONE;
		}
		else if (time_diff > 150 && is_ln_pressing)
		{
			m_score->score += JUDGEMENT_ER;
			m_score->combo = 0;
			m_score->error++;
			combo_text->SetColor(0xEC, 0x6A, 0x5C);
			MessageBox::instance()->SetText("Error Early");
			Animator::instance()->ResetAnimation("combo");
			Animator::instance()->Animate("combo");
			return JUDGEMENT_ER;
		}
		else if (time_diff > 150 && !is_ln_pressing)
		{
			m_score->score += JUDGEMENT_ER;
			m_score->combo = 0;
			m_score->error++;
			combo_text->SetColor(0xEC, 0x6A, 0x5C);
			MessageBox::instance()->SetText("Error Early");
			Animator::instance()->ResetAnimation("combo");
			Animator::instance()->Animate("combo");
			return JUDGEMENT_ER;
		}
		else if (time_diff > 100 || time_diff < -100)
		{
			m_score->score += JUDGEMENT_GD;
			m_score->combo++;
			m_score->good++;
			combo_text->SetColor(0x84, 0xB1, 0xED);
			MessageBox::instance()->SetText("Good");
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
			MessageBox::instance()->SetText("Great");
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
			MessageBox::instance()->SetText("Pure");
			Animator::instance()->ResetAnimation("combo");
			Animator::instance()->Animate("combo");
			return JUDGEMENT_PG;
		}
	}
	else if (time_diff < -150)
	{
		m_score->score += JUDGEMENT_ER;
		m_score->combo = 0;
		m_score->error++;
		combo_text->SetColor(0xEC, 0x6A, 0x5C);
		MessageBox::instance()->SetText("Error Late");
		Animator::instance()->ResetAnimation("combo");
		Animator::instance()->Animate("combo");
		return JUDGEMENT_ER;
	}
	char *combo_ch = new char[5];
	sprintf(combo_ch, "%d", m_score->combo);
	combo_text->SetText(combo_ch);
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

int fr::GameBeatmap::GetDuration()
{
	return note_duration;
}

int fr::GameBeatmap::GetOffset()
{
	return offset;
}
