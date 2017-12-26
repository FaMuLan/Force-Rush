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
	current_offset_text = new TextArea;
	hit_offset_num->init(" ", System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2, "assets/fonts/Audiowide.ttf", 120, 0, 0, 0);
	char *current_offset_ch = new char;
	sprintf(current_offset_ch, "current: %dms", Setting::instance()->GetOffset());
	current_offset_text->init(current_offset_ch, System::instance()->GetWindowWidth() / 2, 36, "assets/fonts/Audiowide.ttf", 36, 0, 0, 0);
	delete current_offset_ch;
	TextureManager::instance()->loadfont("assets/fonts/Audiowide.ttf", 120);

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
	beat_count = 0;
	average_offset = 0;
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
		current_offset_text->SetPos(System::instance()->GetWindowWidth() / 2, 36);
	}
	if (Timer::instance()->GetTime("game") >= 60000 + 1000 && is_running)
	{
		stop();
	}
	if (beat_count > 4)
	{
		beat_count = 0;
		Setting::instance()->SetOffset(Setting::instance()->GetOffset() + average_offset);
		char *current_offset_ch = new char;
		sprintf(current_offset_ch, "current: %dms", Setting::instance()->GetOffset());
		current_offset_text->SetText(current_offset_ch);
		delete current_offset_ch;
	}
}

void fr::SettingBeatmap::render()
{
	Beatmap::render();
	current_offset_text->render();
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
		else if (time_diff > 150 || time_diff < -150)
		{
			hit_offset_num->SetColor(0xEC, 0x6A, 0x5C);
			char *offset_ch = new char;
			sprintf(offset_ch, "%dms", time_diff);
			hit_offset_num->SetText(offset_ch);
			delete offset_ch;
			Animator::instance()->ResetAnimation("offset");
			Animator::instance()->Animate("offset");
			average_offset = (average_offset + time_diff) / 2;
			beat_count++;
			return JUDGEMENT_ER;
		}
		else if (time_diff > 100 || time_diff < -100)
		{
			hit_offset_num->SetColor(0x84, 0xB1, 0xED);
			char *offset_ch = new char;
			sprintf(offset_ch, "%dms", time_diff);
			hit_offset_num->SetText(offset_ch);
			delete offset_ch;
			Animator::instance()->ResetAnimation("offset");
			Animator::instance()->Animate("offset");
			average_offset = (average_offset + time_diff) / 2;
			beat_count++;
			return JUDGEMENT_GD;
		}
		else if (time_diff > 50 || time_diff < -50)
		{
			hit_offset_num->SetColor(0x81, 0xC7, 0x84);
			char *offset_ch = new char;
			sprintf(offset_ch, "%dms", time_diff);
			hit_offset_num->SetText(offset_ch);
			delete offset_ch;
			Animator::instance()->ResetAnimation("offset");
			Animator::instance()->Animate("offset");
			average_offset = (average_offset + time_diff) / 2;
			beat_count++;
			return JUDGEMENT_GR;
		}
		else
		{
			hit_offset_num->SetColor(0xFF, 0xEA, 0x00);
			char *offset_ch = new char;
			sprintf(offset_ch, "%dms", time_diff);
			hit_offset_num->SetText(offset_ch);
			delete offset_ch;
			Animator::instance()->ResetAnimation("offset");
			Animator::instance()->Animate("offset");
			average_offset = (average_offset + time_diff) / 2;
			beat_count++;
			return JUDGEMENT_PG;
		}
	}
	else if (time_diff < -500)
	{
		return JUDGEMENT_ER;
	}
	return JUDGEMENT_NONE;
}

void fr::SettingBeatmap::start()
{
	hit_offset_num->SetText(" ");
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

void fr::SettingBeatmap::ResetScale()
{
	play_base->SetSize(System::instance()->GetWindowWidth(), System::instance()->GetWindowHeigh() * Setting::instance()->GetDrawScale());
	play_base->SetPos(0, Setting::instance()->GetDrawOffset());
	scale_w = System::instance()->GetWindowWidth() / 720.f;
	scale_h = System::instance()->GetWindowHeigh() / 1280.f * Setting::instance()->GetDrawScale();
}

void fr::GameSettingState::init()
{
	widget_base = new Sprite;
	back = new Button;
	offset_wizard_switch = new Button;
	draw_scale_left = new Button;
	draw_scale_right = new Button;
	draw_scale_left_dual = new Button;
	draw_scale_right_dual = new Button;
	draw_offset_left = new Button;
	draw_offset_right = new Button;
	draw_offset_left_dual = new Button;
	draw_offset_right_dual = new Button;
	offset_wizard_text = new TextArea;
	draw_scale_text = new TextArea;
	draw_offset_text = new TextArea;
	draw_scale_num = new TextArea;
	draw_offset_num = new TextArea;

	TextureManager::instance()->loadfont("assets/fonts/Audiowide.ttf", 36);
	SettingBeatmap::instance()->init();
	widget_base->init("assets/base/widget_base.png");
	widget_base->SetPos(System::instance()->GetWindowWidth() / 2 - widget_base->GetW() / 2, System::instance()->GetWindowHeigh() / 2 - widget_base->GetH() / 2);
	back->init("assets/base/sort_button.png", 0, 0);
	back->AddPressedFrame("assets/base/sort_button_pressed.png");
	back->AddText("Back", back->GetW() / 2, back->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
	offset_wizard_switch->init("assets/base/sort_button.png", widget_base->GetX() + 464, widget_base->GetY() + 32);
	offset_wizard_switch->AddPressedFrame("assets/base/sort_button_pressed.png");
	offset_wizard_switch->AddText("START", offset_wizard_switch->GetW() / 2, offset_wizard_switch->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
	draw_scale_left->init("assets/base/arrow_left.png", widget_base->GetX() + 430,  widget_base->GetY() + 154);
	draw_scale_left->AddPressedFrame("assets/base/arrow_left_pressed.png");
	draw_scale_left_dual->init("assets/base/arrow_left_dual.png", widget_base->GetX() + 382,  widget_base->GetY() + 154);
	draw_scale_left_dual->AddPressedFrame("assets/base/arrow_left_dual_pressed.png");
	draw_scale_right->init("assets/base/arrow_right.png", widget_base->GetX() + 622, widget_base->GetY() + 154);
	draw_scale_right->AddPressedFrame("assets/base/arrow_right_pressed.png");
	draw_scale_right_dual->init("assets/base/arrow_right_dual.png", widget_base->GetX() + 660, widget_base->GetY() + 154);
	draw_scale_right_dual->AddPressedFrame("assets/base/arrow_right_dual_pressed.png");
	draw_offset_left->init("assets/base/arrow_left.png", widget_base->GetX() + 430, widget_base->GetY() + 258);
	draw_offset_left->AddPressedFrame("assets/base/arrow_left_pressed.png");
	draw_offset_left_dual->init("assets/base/arrow_left_dual.png", widget_base->GetX() + 382, widget_base->GetY() + 258);
	draw_offset_left_dual->AddPressedFrame("assets/base/arrow_left_dual_pressed.png");
	draw_offset_right->init("assets/base/arrow_right.png", widget_base->GetX() + 622, widget_base->GetY() + 258);
	draw_offset_right->AddPressedFrame("assets/base/arrow_right_pressed.png");
	draw_offset_right_dual->init("assets/base/arrow_right_dual.png", widget_base->GetX() + 660, widget_base->GetY() + 258);
	draw_offset_right_dual->AddPressedFrame("assets/base/arrow_right_dual_pressed.png");

	offset_wizard_text->init("OFFSET WIZARD", widget_base->GetX() + 32, widget_base->GetY() + 50, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);
	draw_scale_text->init("DRAW SCALE", widget_base->GetX() + 32, widget_base->GetY() + 154, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);
	draw_offset_text->init("DRAW OFFSET", widget_base->GetX() + 32, widget_base->GetY() + 258, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);
	char *draw_scale_ch = new char;
	char *draw_offset_ch = new char;
	sprintf(draw_scale_ch, "%.1f%%", Setting::instance()->GetDrawScale() * 100);
	sprintf(draw_offset_ch, "%d", Setting::instance()->GetDrawOffset());
	draw_scale_num->init(draw_scale_ch, widget_base->GetX() + 544, widget_base->GetY() + 172, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
	draw_offset_num->init(draw_offset_ch, widget_base->GetX() + 544, widget_base->GetY() + 276, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
	delete draw_scale_ch;
	delete draw_offset_ch;

}

void fr::GameSettingState::clear()
{
	
}

void fr::GameSettingState::update()
{
	SettingBeatmap::instance()->update();
	back->update();
	offset_wizard_switch->update();
	draw_scale_left->update();
	draw_scale_right->update();
	draw_scale_left_dual->update();
	draw_scale_right_dual->update();
	draw_offset_left->update();
	draw_offset_right->update();
	draw_offset_left_dual->update();
	draw_offset_right_dual->update();

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

	if (System::instance()->IsWindowModified())
	{
		widget_base->SetPos(System::instance()->GetWindowWidth() / 2 - widget_base->GetW() / 2, System::instance()->GetWindowHeigh() / 2 - widget_base->GetH() / 2);
		offset_wizard_switch->SetPos(widget_base->GetX() + 464, widget_base->GetY() + 32);
		draw_scale_left->SetPos(widget_base->GetX() + 430,  widget_base->GetY() + 154);
		draw_scale_left_dual->SetPos(widget_base->GetX() + 382,  widget_base->GetY() + 154);
		draw_scale_right->SetPos(widget_base->GetX() + 622, widget_base->GetY() + 154);
		draw_scale_right_dual->SetPos(widget_base->GetX() + 660, widget_base->GetY() + 154);
		draw_offset_left->SetPos(widget_base->GetX() + 430, widget_base->GetY() + 258);
		draw_offset_left_dual->SetPos(widget_base->GetX() + 382, widget_base->GetY() + 258);
		draw_offset_right->SetPos(widget_base->GetX() + 622, widget_base->GetY() + 258);
		draw_offset_right_dual->SetPos(widget_base->GetX() + 660, widget_base->GetY() + 258);
		offset_wizard_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 50);
		draw_scale_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 154);
		draw_offset_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 258);
		draw_scale_num->SetPos(widget_base->GetX() + 544, widget_base->GetY() + 172);
		draw_offset_num->SetPos(widget_base->GetX() + 544, widget_base->GetY() + 276);
	}

	if (back->IsReleased())
	{
		LoadingState::instance()->init(SelectState::instance(), this);
	}

	if (draw_scale_left->IsReleased())
	{
		Setting::instance()->SetDrawScale(Setting::instance()->GetDrawScale() - 0.001f);
		char *draw_scale_ch = new char;
		sprintf(draw_scale_ch, "%.1f", Setting::instance()->GetDrawScale() * 100);
		draw_scale_num->SetText(draw_scale_ch);
		delete draw_scale_ch;
		SettingBeatmap::instance()->ResetScale();
	}
	if (draw_scale_left_dual->IsReleased())
	{
		Setting::instance()->SetDrawScale(Setting::instance()->GetDrawScale() - 0.01f);
		char *draw_scale_ch = new char;
		sprintf(draw_scale_ch, "%.1f", Setting::instance()->GetDrawScale() * 100);
		draw_scale_num->SetText(draw_scale_ch);
		delete draw_scale_ch;
		SettingBeatmap::instance()->ResetScale();
	}
	if (draw_scale_right->IsReleased())
	{
		Setting::instance()->SetDrawScale(Setting::instance()->GetDrawScale() + 0.001f);
		char *draw_scale_ch = new char;
		sprintf(draw_scale_ch, "%.1f", Setting::instance()->GetDrawScale() * 100);
		draw_scale_num->SetText(draw_scale_ch);
		delete draw_scale_ch;
		SettingBeatmap::instance()->ResetScale();
	}
	if (draw_scale_right_dual->IsReleased())
	{
		Setting::instance()->SetDrawScale(Setting::instance()->GetDrawScale() + 0.01f);
		char *draw_scale_ch = new char;
		sprintf(draw_scale_ch, "%.1f", Setting::instance()->GetDrawScale() * 100);
		draw_scale_num->SetText(draw_scale_ch);
		delete draw_scale_ch;
		SettingBeatmap::instance()->ResetScale();
	}
	if (draw_offset_left->IsReleased())
	{
		Setting::instance()->SetDrawOffset(Setting::instance()->GetDrawOffset() - 1);
		char *draw_offset_ch = new char;
		sprintf(draw_offset_ch, "%d", Setting::instance()->GetDrawOffset());
		draw_offset_num->SetText(draw_offset_ch);
		delete draw_offset_ch;
		SettingBeatmap::instance()->ResetScale();
	}
	if (draw_offset_left_dual->IsReleased())
	{
		Setting::instance()->SetDrawOffset(Setting::instance()->GetDrawOffset() - 10);
		char *draw_offset_ch = new char;
		sprintf(draw_offset_ch, "%d", Setting::instance()->GetDrawOffset());
		draw_offset_num->SetText(draw_offset_ch);
		delete draw_offset_ch;
		SettingBeatmap::instance()->ResetScale();
	}
	if (draw_offset_right->IsReleased())
	{
		Setting::instance()->SetDrawOffset(Setting::instance()->GetDrawOffset() + 1);
		char *draw_offset_ch = new char;
		sprintf(draw_offset_ch, "%d", Setting::instance()->GetDrawOffset());
		draw_offset_num->SetText(draw_offset_ch);
		delete draw_offset_ch;
		SettingBeatmap::instance()->ResetScale();
	}
	if (draw_offset_right_dual->IsReleased())
	{
		Setting::instance()->SetDrawOffset(Setting::instance()->GetDrawOffset() + 10);
		char *draw_offset_ch = new char;
		sprintf(draw_offset_ch, "%d", Setting::instance()->GetDrawOffset());
		draw_offset_num->SetText(draw_offset_ch);
		delete draw_offset_ch;
		SettingBeatmap::instance()->ResetScale();
	}

	SettingBeatmap::instance()->render();
	widget_base->render();
	back->render();
	offset_wizard_switch->render();
	draw_scale_left->render();
	draw_scale_right->render();
	draw_scale_left_dual->render();
	draw_scale_right_dual->render();
	draw_offset_left->render();
	draw_offset_right->render();
	draw_offset_left_dual->render();
	draw_offset_right_dual->render();
	offset_wizard_text->render();
	draw_scale_text->render();
	draw_offset_text->render();
	draw_scale_num->render();
	draw_offset_num->render();
}