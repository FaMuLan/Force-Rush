#include "beatmap.h"
#include <string>
#include <regex>
#include "column.h"
#include "../message_box.h"
#include "../sound_manager.h"
#include "../file_system.h"
#include "../system.h"
#include "../timer.h"
#include "../sprite.h"

lm::Beatmap *lm::Beatmap::m_instance = 0;

void lm::Beatmap::load(std::string path)
{
	play_base = new Sprite;
	play_base->init("assets/game/play_base.png", 0, 0, System::instance()->GetWindowWidth(), System::instance()->GetWindowHeigh());
	scale_w = System::instance()->GetWindowWidth() / 720.f;
	scale_h = System::instance()->GetWindowHeigh() / 1280.f;

	audio_path = GetParentDir(path);
	note_duration = 700;
	offset = 200;
	bool is_mapped = false;
	is_waiting = true;

	std::string text;
	ReadFile(path, text);
	std::regex audio_path_pattern("AudioFilename: (.*)");
	std::regex note_pattern("(\\d+),\\d+,(\\d+),(\\d+),\\d+,(\\d+):\\d+:\\d+:\\d+:(\\d+:)?");

	std::smatch audio_path_line;
	std::regex_search(text, audio_path_line, audio_path_pattern);
	audio_path += std::regex_replace(audio_path_line.str(), audio_path_pattern, "$1");

	for (std::sregex_iterator i = std::sregex_iterator(text.begin(), text.end(), note_pattern); i != std::sregex_iterator(); i++)
	{
		std::smatch line = *i;
		Note *new_note = new Note;
		new_note->time = atoi(std::regex_replace(line.str(), note_pattern, "$2").c_str()) + offset + 2000;
		int type = atoi(std::regex_replace(line.str(), note_pattern, "$3").c_str());
		new_note->time_end = (type % 16 == 0) ? atoi(std::regex_replace(line.str(), note_pattern, "$4").c_str()) + offset + 2000 : new_note->time;
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
		else if (time_diff > 100 && is_ln_pressing)
		{
			score += JUDGEMENT_ER;
			combo = 0;
			MessageBox::instance()->SetText("Error Early");
			return JUDGEMENT_ER;
		}
		else if (time_diff > 100 || time_diff < -100)
		{
			score += JUDGEMENT_ER;
			combo = 0;
			MessageBox::instance()->SetText("Error Early");
			return JUDGEMENT_ER;
		}
		else if (time_diff > 50 || time_diff < -50)
		{
			score += JUDGEMENT_GD;
			combo++;
			MessageBox::instance()->SetText("Good");
			return JUDGEMENT_GD;
		}
		else if (time_diff > 25 || time_diff < -25)
		{
			score += JUDGEMENT_GR;
			combo++;
			MessageBox::instance()->SetText("Great");
			return JUDGEMENT_GR;
		}
		else
		{
			score += JUDGEMENT_PG;
			combo++;
			MessageBox::instance()->SetText("Pure");
			return JUDGEMENT_PG;
		}
	}
	else if (time_diff < -100)
	{
		score += JUDGEMENT_ER;
		combo = 0;
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