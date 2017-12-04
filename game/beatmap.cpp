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
#include "../file_system.h"
#include "../system.h"
#include "../timer.h"
#include "../sprite.h"

lm::Beatmap *lm::Beatmap::m_instance = 0;

void lm::Beatmap::load(lm::SongInformation *load_information)
{
	m_information = load_information;
	m_score = new Score;
	m_score->pure = 0;
	m_score->great = 0;
	m_score->good = 0;
	m_score->error = 0;
	m_score->score = 0;
	m_score->combo = 0;

	play_base = new Sprite;
	play_base->init("assets/game/play_base.png", 0, 0, System::instance()->GetWindowWidth(), System::instance()->GetWindowHeigh());
	scale_w = System::instance()->GetWindowWidth() / 720.f;
	scale_h = System::instance()->GetWindowHeigh() / 1280.f;

	audio_path = GetParentDir(m_information->file_path);
	note_duration = Setting::instance()->GetDuration();
	offset = Setting::instance()->GetOffset();
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
//	SoundManager::instance()->load(audio_path, SOUNDTYPE_MUSIC);
//	無需加載，因為在選曲界面的時候就加載好了

	Timer::instance()->RunTimer("game");
}

void lm::Beatmap::clear()
{
	m_column.clear();
	Timer::instance()->ResetTimer("game");
	if (SoundManager::instance()->IsPlayingMusic())
	{
		SoundManager::instance()->stop();
	}
}

void lm::Beatmap::update()
{
	if (Timer::instance()->GetTime("game") > 2000 && is_waiting)
	{
		SoundManager::instance()->play(audio_path, SOUNDTYPE_MUSIC);
		is_waiting = false;
	}
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

	if (Timer::instance()->GetTime("game") >= m_information->duration + 5000 && !is_ended)
	{
		LoadingState::instance()->init(ResultState::instance(), GameState::instance());
		ResultState::instance()->LoadScore(m_information, m_score);
		is_ended = true;
	}
}

void lm::Beatmap::render()
{
	play_base->render();
	for (int i = 0; i < m_column.size(); i++)
	{
		m_column[i]->render();
	}
}

lm::Judgement lm::Beatmap::judge(int note_time, bool is_pressed, bool is_ln_pressing)
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
			MessageBox::instance()->SetText("Error Early");
			return JUDGEMENT_ER;
		}
		else if (time_diff > 150 && !is_ln_pressing)
		{
			m_score->score += JUDGEMENT_ER;
			m_score->combo = 0;
			m_score->error++;
			MessageBox::instance()->SetText("Error Early");
			return JUDGEMENT_ER;
		}
		else if (time_diff > 100 || time_diff < -100)
		{
			m_score->score += JUDGEMENT_GD;
			m_score->combo++;
			m_score->good++;
			MessageBox::instance()->SetText("Good");
			return JUDGEMENT_GD;
		}
		else if (time_diff > 50 || time_diff < -50)
		{
			m_score->score += JUDGEMENT_GR;
			m_score->combo++;
			m_score->great++;
			MessageBox::instance()->SetText("Great");
			return JUDGEMENT_GR;
		}
		else
		{
			m_score->score += JUDGEMENT_PG;
			m_score->combo++;
			m_score->pure++;
			MessageBox::instance()->SetText("Pure");
			return JUDGEMENT_PG;
		}
	}
	else if (time_diff < -150)
	{
		m_score->score += JUDGEMENT_ER;
		m_score->combo = 0;
		m_score->error++;
		MessageBox::instance()->SetText("Error Late");
		return JUDGEMENT_ER;
	}
	return JUDGEMENT_NONE;
}	//Judgement lm::Beatmap::judge()

int lm::Beatmap::GetDuration()
{
	return note_duration;
}

int lm::Beatmap::GetOffset()
{
	return offset;
}

float lm::Beatmap::GetScaleW()
{
	return scale_w;
}

float lm::Beatmap::GetScaleH()
{
	return scale_h;
}