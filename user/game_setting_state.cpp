#include "game_setting_state.h"
#include "../sprite.h"
#include "../button.h"
#include "../text_area.h"
#include "../sound_manager.h"
#include "../timer.h"
#include "../animator.h"
#include "../texture_manager.h"
#include "../select/select_state.h"
#include "../loading/loading_state.h"
#include "../game/column.h"
#include "../system.h"
#include "setting.h"

fr::SettingBeatmap *fr::SettingBeatmap::m_instance = 0;
fr::GameSettingState *fr::GameSettingState::m_instance = 0;

void fr::SettingBeatmap::init()
{
	Beatmap::init();

	hit_offset_num = new TextArea;
	hit_offset_num->init(" ", System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2, "assets/fonts/Audiowide.ttf", 240, 0, 0, 0);
	TextureManager::instance()->loadfont("assets/fonts/Audiowide.ttf", 240);


	audio_path = "assets/base/offset_wizard_bgm.ogg";
	SoundManager::instance()->load(audio_path, SOUNDTYPE_MUSIC);
	for (int i = 0; i < 120; i++)
	{
		Note *new_note = new Note;
		new_note->time = i * 500 + 2000;
		new_note->time_end = new_note->time;
		m_column[3]->AddNote(new_note);
	}
	Animator::instance()->AddAnimation("offset", ANIMATIONTYPE_UNIFORMLY_DECELERATED, 100);
	Animator::instance()->ResetAnimation("offset");
}

void fr::SettingBeatmap::clear()
{
	
}

void fr::SettingBeatmap::update()
{
	Beatmap::update();
	if (is_running)
	{
		if (Timer::instance()->GetTime("game") > 2000 && is_waiting)
		{
			SoundManager::instance()->play(audio_path, SOUNDTYPE_MUSIC);
			is_waiting = false;
		}
	}
	if (System::instance()->IsWindowModified())
	{
		hit_offset_num->SetPos(System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2);
	}
	if (Timer::instance()->GetTime("game") >= 60000 + 1000 && is_running)
	{
		stop();
	}
}

void fr::SettingBeatmap::render()
{
	Beatmap::render();
	if (is_running)
	{
		hit_offset_num->render(hit_offset_num->GetX(), hit_offset_num->GetY() - 100.f * (1.f - Animator::instance()->GetProcess("offset")));
	}
}

fr::Judgement fr::SettingBeatmap::judge(int note_time, bool is_pressed, bool is_ln_pressing)
{
	int time_diff = note_time - Setting::instance()->GetOffset() - Timer::instance()->GetTime("game");
	if (is_pressed)
	{
		if (time_diff > 500 && !is_ln_pressing)
		{
			return JUDGEMENT_NONE;
		}
		else if (time_diff > 150 && is_ln_pressing)
		{
			hit_offset_num->SetColor(0xEC, 0x6A, 0x5C);
			char *offset_ch = new char[5];
			sprintf(offset_ch, "%dms", time_diff);
			hit_offset_num->SetText(offset_ch);
			delete [] offset_ch;
			Animator::instance()->ResetAnimation("offset");
			Animator::instance()->Animate("offset");
			return JUDGEMENT_ER;
		}
		else if (time_diff > 150 && !is_ln_pressing)
		{
			hit_offset_num->SetColor(0xEC, 0x6A, 0x5C);
			char *offset_ch = new char[5];
			sprintf(offset_ch, "%dms", time_diff);
			hit_offset_num->SetText(offset_ch);
			delete [] offset_ch;
			Animator::instance()->ResetAnimation("offset");
			Animator::instance()->Animate("offset");
			return JUDGEMENT_ER;
		}
		else if (time_diff > 100 || time_diff < -100)
		{
			hit_offset_num->SetColor(0x84, 0xB1, 0xED);
			char *offset_ch = new char[5];
			sprintf(offset_ch, "%dms", time_diff);
			hit_offset_num->SetText(offset_ch);
			delete [] offset_ch;
			Animator::instance()->ResetAnimation("offset");
			Animator::instance()->Animate("offset");
			return JUDGEMENT_GD;
		}
		else if (time_diff > 50 || time_diff < -50)
		{
			hit_offset_num->SetColor(0x81, 0xC7, 0x84);
			char *offset_ch = new char[5];
			sprintf(offset_ch, "%dms", time_diff);
			hit_offset_num->SetText(offset_ch);
			delete [] offset_ch;
			Animator::instance()->ResetAnimation("offset");
			Animator::instance()->Animate("offset");
			return JUDGEMENT_GR;
		}
		else
		{
			hit_offset_num->SetColor(0xFF, 0xEA, 0x00);
			char *offset_ch = new char[5];
			sprintf(offset_ch, "%dms", time_diff);
			hit_offset_num->SetText(offset_ch);
			delete [] offset_ch;
			Animator::instance()->ResetAnimation("offset");
			Animator::instance()->Animate("offset");
			return JUDGEMENT_PG;
		}
	}
	else if (time_diff < -150)
	{
		hit_offset_num->SetColor(0xEC, 0x6A, 0x5C);
		Animator::instance()->ResetAnimation("offset");
		Animator::instance()->Animate("offset");
		return JUDGEMENT_ER;
	}
	return JUDGEMENT_NONE;
}

void fr::SettingBeatmap::start()
{
	Timer::instance()->RunTimer("game");
	is_running = true;
}

void fr::SettingBeatmap::stop()
{
	m_column[3]->reset();
	Timer::instance()->ResetTimer("game");
	is_running = false;
}

bool fr::SettingBeatmap::IsRunning()
{
	return is_running;
}

void fr::GameSettingState::init()
{
	back = new Button;
	offset_wizard_switch = new Button;
	offset_num = new TextArea;
	SettingBeatmap::instance()->init();
	back->init("assets/base/sort_button.png", 0, 0);
	back->AddPressedFrame("assets/base/sort_button_pressed.png");
	offset_wizard_switch->init("assets/base/sort_button.png", System::instance()->GetWindowWidth() - 160, 0);
	offset_wizard_switch->AddPressedFrame("assets/base/sort_button_pressed.png");
	offset_wizard_switch->AddText("START", offset_wizard_switch->GetW() / 2, offset_wizard_switch->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
	back->AddText("Back", back->GetW() / 2, back->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
}

void fr::GameSettingState::clear()
{
	
}

void fr::GameSettingState::update()
{
	SettingBeatmap::instance()->update();
	back->update();
	offset_wizard_switch->update();

	if (SettingBeatmap::instance()->IsRunning())
	{
		offset_wizard_switch->ClearText();
		offset_wizard_switch->AddText("STOP", offset_wizard_switch->GetW() / 2, offset_wizard_switch->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
		if (offset_wizard_switch->IsReleased())
		{
			SettingBeatmap::instance()->stop();
		}
	}
	else
	{
		offset_wizard_switch->ClearText();
		offset_wizard_switch->AddText("START", offset_wizard_switch->GetW() / 2, offset_wizard_switch->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
		if (offset_wizard_switch->IsReleased())
		{
			SettingBeatmap::instance()->start();
		}
	}

	if (back->IsReleased())
	{
		LoadingState::instance()->init(SelectState::instance(), this);
	}

	SettingBeatmap::instance()->render();
	back->render();
	offset_wizard_switch->render();
}