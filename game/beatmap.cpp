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

	audio_path = GetParentDir(path);
	note_duration = 1000;
	offset = 200;
	bool is_mapped = false;

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
		new_note->time = atoi(std::regex_replace(line.str(), note_pattern, "$2").c_str()) + offset;
		int type = atoi(std::regex_replace(line.str(), note_pattern, "$3").c_str());
		new_note->time_end = (type % 16 == 0) ? atoi(std::regex_replace(line.str(), note_pattern, "$4").c_str()) + offset : new_note->time;
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
	SoundManager::instance()->play(audio_path, SOUNDTYPE_MUSIC);
	Timer::instance()->RunTimer("game");
}

void lm::Beatmap::clear()
{
	m_column.clear();
}

void lm::Beatmap::update()
{
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
		if (time_diff > 1000 && !is_ln_pressing)
		{
			return JUDGEMENT_NONE;
		}
		else if (time_diff > 1000 && is_ln_pressing)
		{
			score += JUDGEMENT_ER;
			combo = 0;
			MessageBox::instance()->SetText("Error");
			return JUDGEMENT_ER;
		}
		else if (time_diff > 500 || time_diff < -500)
		{
			score += JUDGEMENT_ER;
			combo = 0;
			MessageBox::instance()->SetText("Error");
			return JUDGEMENT_ER;
		}
		else if (time_diff > 200 || time_diff < -200)
		{
			score += JUDGEMENT_GD;
			combo++;
			MessageBox::instance()->SetText("Good");
			return JUDGEMENT_GD;
		}
		else if (time_diff > 100 || time_diff < -100)
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
			return JUDGEMENT_PG;
		}
	}
	else if (time_diff < -1000)
	{
		score += JUDGEMENT_ER;
		combo = 0;
		MessageBox::instance()->SetText("Error");
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