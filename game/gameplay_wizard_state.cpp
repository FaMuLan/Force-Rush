#include "gameplay_wizard_state.h"
#include "column.h"
#include "../sprite.h"
#include "../button.h"
#include "../text_area.h"
#include "../sound_manager.h"
#include "../timer.h"
#include "../animator.h"
#include "../texture_manager.h"
#include "../loading/loading_state.h"
#include "../system.h"
#include "../song_data.h"
#include "../user/setting.h"

fr::GameplayWizardBeatmap *fr::GameplayWizardBeatmap::m_instance = 0;
fr::GameplayWizardState *fr::GameplayWizardState::m_instance = 0;

void fr::GameplayWizardBeatmap::init()
{
	Beatmap::init();

	hit_offset_num = new TextArea;
	current_offset_text = new TextArea;
	hit_offset_num->init(" ", System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2, "assets/fonts/Audiowide.ttf", 120, 0, 0, 0);
	char *current_offset_ch = new char[30];
	sprintf(current_offset_ch, "current: %dms", Setting::instance()->GetOffset());
	current_offset_text->init(current_offset_ch, System::instance()->GetWindowWidth() / 2, 36, "assets/fonts/Audiowide.ttf", 36, 0, 0, 0);
	delete current_offset_ch;

	audio_path = "assets/base/offset_wizard_bgm.ogg";
	SoundManager::instance()->load(audio_path, SOUNDTYPE_MUSIC);
	for (int i = 0; i < 120; i++)
	{
		Note *new_note = new Note;
		new_note->time = i * 500 + 2000;
		new_note->time_end = new_note->time;
		new_note->type = NOTETYPE_NORMAL;
		new_note->type_end = NOTETYPE_NORMAL;
		m_column[3]->AddNote(new_note);
	}
	Animator::instance()->AddAnimation("offset", ANIMATIONTYPE_UNIFORMLY_DECELERATED, 100);
	Animator::instance()->ResetAnimation("offset");
	beat_count = 0;
	average_offset = 0;
	is_running = false;
}

void fr::GameplayWizardBeatmap::clear()
{
	Beatmap::clear();
}

void fr::GameplayWizardBeatmap::update()
{
	Beatmap::update();
	if (is_running)
	{
		if (Timer::instance()->GetTime("game") > 2000 && is_waiting)
		{
			SoundManager::instance()->play(audio_path, Timer::instance()->GetTime("game") - 2000);
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
		char *current_offset_ch = new char[30];
		sprintf(current_offset_ch, "current: %dms", Setting::instance()->GetOffset());
		current_offset_text->SetText(current_offset_ch);
		delete [] current_offset_ch;
	}
}

void fr::GameplayWizardBeatmap::render()
{
	Beatmap::render();
	current_offset_text->render();
	if (is_running)
	{
		hit_offset_num->render(hit_offset_num->GetX(), hit_offset_num->GetY() - 100.f * (1.f - Animator::instance()->GetProcess("offset")));
	}
}

fr::Judgement fr::GameplayWizardBeatmap::judge(int note_time, bool is_pressed, bool is_ln_pressing, bool error_only)
{
	int time_diff = note_time - Setting::instance()->GetOffset() - Timer::instance()->GetTime("game");
	if (is_pressed)
	{
		if (time_diff > 500)
		{
			return JUDGEMENT_NONE;
		}
		else
		{
			char *offset_ch = new char[10];
			sprintf(offset_ch, "%dms", time_diff);
			hit_offset_num->SetText(offset_ch);
			delete [] offset_ch;
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

void fr::GameplayWizardBeatmap::start()
{
	hit_offset_num->SetText(" ");
	Timer::instance()->RunTimer("game");
	is_running = true;
	is_waiting = true;
}

void fr::GameplayWizardBeatmap::stop()
{
	m_column[3]->reset();
	Timer::instance()->ResetTimer("game");
	if (SoundManager::instance()->IsPlayingMusic())
	{
		SoundManager::instance()->stop();
	}
	is_running = false;
}

bool fr::GameplayWizardBeatmap::IsRunning()
{
	return is_running;
}


void fr::GameplayWizardState::init()
{
	widget_base = new Sprite;
	back = new Button;
	offset_wizard_switch = new Button;
	camera_pos_y_left = new Button;
	camera_pos_y_right = new Button;
	camera_pos_y_left_dual = new Button;
	camera_pos_y_right_dual = new Button;
	camera_pos_z_left = new Button;
	camera_pos_z_right = new Button;
	camera_pos_z_left_dual = new Button;
	camera_pos_z_right_dual = new Button;
	camera_rotate_x_left = new Button;
	camera_rotate_x_right = new Button;
	camera_rotate_x_left_dual = new Button;
	camera_rotate_x_right_dual = new Button;
	offset_wizard_text = new TextArea;
	camera_pos_y_text = new TextArea;
	camera_pos_z_text = new TextArea;
	camera_rotate_x_text = new TextArea;
	camera_pos_y_num = new TextArea;
	camera_pos_z_num = new TextArea;
	camera_rotate_x_num = new TextArea;

	GameplayWizardBeatmap::instance()->init();
	widget_base->init("assets/base/widget_base.png");
	widget_base->SetPos(System::instance()->GetWindowWidth() / 2 - widget_base->GetW() / 2, System::instance()->GetWindowHeigh() / 2 - widget_base->GetH() / 2);
	back->init("assets/base/sort_button.png");
	back->AddPressedFrame("assets/base/sort_button_pressed.png");
	back->AddText("Back", back->GetW() / 2, back->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
	offset_wizard_switch->init("assets/base/sort_button.png", Rect(widget_base->GetX() + 464, widget_base->GetY() + 32, 0, 0));
	offset_wizard_switch->AddPressedFrame("assets/base/sort_button_pressed.png");
	offset_wizard_switch->AddText("START", offset_wizard_switch->GetW() / 2, offset_wizard_switch->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);

	
	camera_pos_y_left->init("assets/base/arrow_left.png", Rect(widget_base->GetX() + 430,  widget_base->GetY() + 154, 0, 0));
	camera_pos_y_left->AddPressedFrame("assets/base/arrow_left_pressed.png");
	camera_pos_y_left_dual->init("assets/base/arrow_left_dual.png", Rect(widget_base->GetX() + 382,  widget_base->GetY() + 154, 0, 0));
	camera_pos_y_left_dual->AddPressedFrame("assets/base/arrow_left_dual_pressed.png");
	camera_pos_y_right->init("assets/base/arrow_right.png", Rect(widget_base->GetX() + 622, widget_base->GetY() + 154, 0, 0));
	camera_pos_y_right->AddPressedFrame("assets/base/arrow_right_pressed.png");
	camera_pos_y_right_dual->init("assets/base/arrow_right_dual.png", Rect(widget_base->GetX() + 660, widget_base->GetY() + 154, 0, 0));
	camera_pos_y_right_dual->AddPressedFrame("assets/base/arrow_right_dual_pressed.png");
	camera_pos_z_left->init("assets/base/arrow_left.png", Rect(widget_base->GetX() + 430,  widget_base->GetY() + 258, 0, 0));
	camera_pos_z_left->AddPressedFrame("assets/base/arrow_left_pressed.png");
	camera_pos_z_left_dual->init("assets/base/arrow_left_dual.png", Rect(widget_base->GetX() + 382,  widget_base->GetY() + 258, 0, 0));
	camera_pos_z_left_dual->AddPressedFrame("assets/base/arrow_left_dual_pressed.png");
	camera_pos_z_right->init("assets/base/arrow_right.png", Rect(widget_base->GetX() + 622, widget_base->GetY() + 258, 0, 0));
	camera_pos_z_right->AddPressedFrame("assets/base/arrow_right_pressed.png");
	camera_pos_z_right_dual->init("assets/base/arrow_right_dual.png", Rect(widget_base->GetX() + 660, widget_base->GetY() + 258, 0, 0));
	camera_pos_z_right_dual->AddPressedFrame("assets/base/arrow_right_dual_pressed.png");
	camera_rotate_x_left->init("assets/base/arrow_left.png", Rect(widget_base->GetX() + 430, widget_base->GetY() + 362, 0, 0));
	camera_rotate_x_left->AddPressedFrame("assets/base/arrow_left_pressed.png");
	camera_rotate_x_left_dual->init("assets/base/arrow_left_dual.png", Rect(widget_base->GetX() + 382, widget_base->GetY() + 362, 0, 0));
	camera_rotate_x_left_dual->AddPressedFrame("assets/base/arrow_left_dual_pressed.png");
	camera_rotate_x_right->init("assets/base/arrow_right.png", Rect(widget_base->GetX() + 622, widget_base->GetY() + 362, 0, 0));
	camera_rotate_x_right->AddPressedFrame("assets/base/arrow_right_pressed.png");
	camera_rotate_x_right_dual->init("assets/base/arrow_right_dual.png", Rect(widget_base->GetX() + 660, widget_base->GetY() + 362, 0, 0));
	camera_rotate_x_right_dual->AddPressedFrame("assets/base/arrow_right_dual_pressed.png");

	offset_wizard_text->init("OFFSET WIZARD", widget_base->GetX() + 32, widget_base->GetY() + 50, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);
	camera_pos_y_text->init("CAMERA POS Y", widget_base->GetX() + 32, widget_base->GetY() + 154, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);
	camera_pos_z_text->init("CAMERA POS Z", widget_base->GetX() + 32, widget_base->GetY() + 258, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);
	camera_rotate_x_text->init("CAMERA ROTATE X", widget_base->GetX() + 32, widget_base->GetY() + 362, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);
	char *camera_pos_y_ch = new char;
	char *camera_pos_z_ch = new char;
	char *camera_rotate_x_ch = new char;
	sprintf(camera_pos_y_ch, "%d", Setting::instance()->GetCameraPosY());
	sprintf(camera_pos_z_ch, "%d", Setting::instance()->GetCameraPosZ());
	sprintf(camera_rotate_x_ch, "%d", Setting::instance()->GetCameraRotateX());
	camera_pos_y_num->init(camera_pos_y_ch, widget_base->GetX() + 544, widget_base->GetY() + 172, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
	camera_pos_z_num->init(camera_pos_z_ch, widget_base->GetX() + 544, widget_base->GetY() + 276, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
	camera_rotate_x_num->init(camera_rotate_x_ch, widget_base->GetX() + 544, widget_base->GetY() + 380, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
	delete camera_pos_z_ch;
	delete camera_rotate_x_ch;

}

void fr::GameplayWizardState::clear()
{
	GameplayWizardBeatmap::instance()->stop();
	GameplayWizardBeatmap::instance()->clear();
}

void fr::GameplayWizardState::update()
{
	GameplayWizardBeatmap::instance()->update();
	back->update();
	offset_wizard_switch->update();
	camera_pos_y_left->update();
	camera_pos_y_right->update();
	camera_pos_y_left_dual->update();
	camera_pos_y_right_dual->update();
	camera_pos_z_left->update();
	camera_pos_z_right->update();
	camera_pos_z_left_dual->update();
	camera_pos_z_right_dual->update();
	camera_rotate_x_left->update();
	camera_rotate_x_right->update();
	camera_rotate_x_left_dual->update();
	camera_rotate_x_right_dual->update();

	if (GameplayWizardBeatmap::instance()->IsRunning())
	{
		if (offset_wizard_switch->IsReleased())
		{
			offset_wizard_switch->ClearText();
			offset_wizard_switch->AddText("START", offset_wizard_switch->GetW() / 2, offset_wizard_switch->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
			GameplayWizardBeatmap::instance()->stop();
		}
	}
	else
	{
		if (offset_wizard_switch->IsReleased())
		{
			offset_wizard_switch->ClearText();
			offset_wizard_switch->AddText("STOP", offset_wizard_switch->GetW() / 2, offset_wizard_switch->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
			GameplayWizardBeatmap::instance()->start();
		}
	}

	if (System::instance()->IsWindowModified())
	{
		widget_base->SetPos(System::instance()->GetWindowWidth() / 2 - widget_base->GetW() / 2, System::instance()->GetWindowHeigh() / 2 - widget_base->GetH() / 2);
		offset_wizard_switch->SetPos(widget_base->GetX() + 464, widget_base->GetY() + 32);
		camera_pos_y_left->SetPos(widget_base->GetX() + 430,  widget_base->GetY() + 154);
		camera_pos_y_left_dual->SetPos(widget_base->GetX() + 382,  widget_base->GetY() + 154);
		camera_pos_y_right->SetPos(widget_base->GetX() + 622, widget_base->GetY() + 154);
		camera_pos_y_right_dual->SetPos(widget_base->GetX() + 660, widget_base->GetY() + 154);
		camera_pos_z_left->SetPos(widget_base->GetX() + 430,  widget_base->GetY() + 258);
		camera_pos_z_left_dual->SetPos(widget_base->GetX() + 382,  widget_base->GetY() + 258);
		camera_pos_z_right->SetPos(widget_base->GetX() + 622, widget_base->GetY() + 258);
		camera_pos_z_right_dual->SetPos(widget_base->GetX() + 660, widget_base->GetY() + 258);
		camera_rotate_x_left->SetPos(widget_base->GetX() + 430, widget_base->GetY() + 362);
		camera_rotate_x_left_dual->SetPos(widget_base->GetX() + 382, widget_base->GetY() + 362);
		camera_rotate_x_right->SetPos(widget_base->GetX() + 622, widget_base->GetY() + 362);
		camera_rotate_x_right_dual->SetPos(widget_base->GetX() + 660, widget_base->GetY() + 362);
		offset_wizard_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 50);
		camera_pos_y_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 154);
		camera_pos_z_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 258);
		camera_rotate_x_text->SetPos(widget_base->GetX() + 32, widget_base->GetY() + 362);
		camera_pos_y_num->SetPos(widget_base->GetX() + 544, widget_base->GetY() + 172);
		camera_pos_z_num->SetPos(widget_base->GetX() + 544, widget_base->GetY() + 276);
		camera_rotate_x_num->SetPos(widget_base->GetX() + 544, widget_base->GetY() + 380);

		char *camera_pos_z_ch = new char;
		sprintf(camera_pos_z_ch, "%d", Setting::instance()->GetCameraPosZ());
		camera_pos_z_num->SetText(camera_pos_z_ch);
		delete camera_pos_z_ch;
		char *camera_rotate_x_ch = new char;
		sprintf(camera_rotate_x_ch, "%d", Setting::instance()->GetCameraRotateX());
		camera_rotate_x_num->SetText(camera_rotate_x_ch);
		delete camera_rotate_x_ch;
	}

	if (back->IsReleased())
	{
		LoadingState::instance()->init(STATE_PREPARE);
	}

	if (camera_pos_z_left->IsReleased())
	{
		Setting::instance()->SetCameraPosY(Setting::instance()->GetCameraPosY() - 10);
		char *camera_pos_y_ch = new char;
		sprintf(camera_pos_y_ch, "%d", Setting::instance()->GetCameraPosZ());
		camera_pos_y_num->SetText(camera_pos_y_ch);
		delete camera_pos_y_ch;
	}
	if (camera_pos_y_left_dual->IsReleased())
	{
		Setting::instance()->SetCameraPosY(Setting::instance()->GetCameraPosY() - 100);
		char *camera_pos_y_ch = new char;
		sprintf(camera_pos_y_ch, "%d", Setting::instance()->GetCameraPosY());
		camera_pos_y_num->SetText(camera_pos_y_ch);
		delete camera_pos_y_ch;
	}
	if (camera_pos_y_right->IsReleased())
	{
		Setting::instance()->SetCameraPosY(Setting::instance()->GetCameraPosY() + 10);
		char *camera_pos_y_ch = new char;
		sprintf(camera_pos_y_ch, "%d", Setting::instance()->GetCameraPosZ());
		camera_pos_y_num->SetText(camera_pos_y_ch);
		delete camera_pos_y_ch;
	}
	if (camera_pos_y_right_dual->IsReleased())
	{
		Setting::instance()->SetCameraPosY(Setting::instance()->GetCameraPosY() + 100);
		char *camera_pos_y_ch = new char;
		sprintf(camera_pos_y_ch, "%d", Setting::instance()->GetCameraPosY());
		camera_pos_y_num->SetText(camera_pos_y_ch);
		delete camera_pos_y_ch;
	}

	if (camera_pos_z_left->IsReleased())
	{
		Setting::instance()->SetCameraPosZ(Setting::instance()->GetCameraPosZ() - 10);
		char *camera_pos_z_ch = new char;
		sprintf(camera_pos_z_ch, "%d", Setting::instance()->GetCameraPosZ());
		camera_pos_z_num->SetText(camera_pos_z_ch);
		delete camera_pos_z_ch;
	}
	if (camera_pos_z_left_dual->IsReleased())
	{
		Setting::instance()->SetCameraPosZ(Setting::instance()->GetCameraPosZ() - 100);
		char *camera_pos_z_ch = new char;
		sprintf(camera_pos_z_ch, "%d", Setting::instance()->GetCameraPosZ());
		camera_pos_z_num->SetText(camera_pos_z_ch);
		delete camera_pos_z_ch;
	}
	if (camera_pos_z_right->IsReleased())
	{
		Setting::instance()->SetCameraPosZ(Setting::instance()->GetCameraPosZ() + 10);
		char *camera_pos_z_ch = new char;
		sprintf(camera_pos_z_ch, "%d", Setting::instance()->GetCameraPosZ());
		camera_pos_z_num->SetText(camera_pos_z_ch);
		delete camera_pos_z_ch;
	}
	if (camera_pos_z_right_dual->IsReleased())
	{
		Setting::instance()->SetCameraPosZ(Setting::instance()->GetCameraPosZ() + 100);
		char *camera_pos_z_ch = new char;
		sprintf(camera_pos_z_ch, "%d", Setting::instance()->GetCameraPosZ());
		camera_pos_z_num->SetText(camera_pos_z_ch);
		delete camera_pos_z_ch;
	}
	if (camera_rotate_x_left->IsReleased())
	{
		Setting::instance()->SetCameraRotateX(Setting::instance()->GetCameraRotateX() - 1);
		char *camera_rotate_x_ch = new char;
		sprintf(camera_rotate_x_ch, "%d", Setting::instance()->GetCameraRotateX());
		camera_rotate_x_num->SetText(camera_rotate_x_ch);
		delete camera_rotate_x_ch;
	}
	if (camera_rotate_x_left_dual->IsReleased())
	{
		Setting::instance()->SetCameraRotateX(Setting::instance()->GetCameraRotateX() - 10);
		char *camera_rotate_x_ch = new char;
		sprintf(camera_rotate_x_ch, "%d", Setting::instance()->GetCameraRotateX());
		camera_rotate_x_num->SetText(camera_rotate_x_ch);
		delete camera_rotate_x_ch;
	}
	if (camera_rotate_x_right->IsReleased())
	{
		Setting::instance()->SetCameraRotateX(Setting::instance()->GetCameraRotateX() + 1);
		char *camera_rotate_x_ch = new char;
		sprintf(camera_rotate_x_ch, "%d", Setting::instance()->GetCameraRotateX());
		camera_rotate_x_num->SetText(camera_rotate_x_ch);
		delete camera_rotate_x_ch;
	}
	if (camera_rotate_x_right_dual->IsReleased())
	{
		Setting::instance()->SetCameraRotateX(Setting::instance()->GetCameraRotateX() + 10);
		char *camera_rotate_x_ch = new char;
		sprintf(camera_rotate_x_ch, "%d", Setting::instance()->GetCameraRotateX());
		camera_rotate_x_num->SetText(camera_rotate_x_ch);
		delete camera_rotate_x_ch;
	}

	GameplayWizardBeatmap::instance()->render();
	widget_base->render();
	back->render();
	offset_wizard_switch->render();
	camera_pos_y_left->render();
	camera_pos_y_right->render();
	camera_pos_y_left_dual->render();
	camera_pos_y_right_dual->render();
	camera_pos_z_left->render();
	camera_pos_z_right->render();
	camera_pos_z_left_dual->render();
	camera_pos_z_right_dual->render();
	camera_rotate_x_left->render();
	camera_rotate_x_right->render();
	camera_rotate_x_left_dual->render();
	camera_rotate_x_right_dual->render();
	offset_wizard_text->render();
	camera_pos_y_text->render();
	camera_pos_z_text->render();
	camera_rotate_x_text->render();
	camera_pos_y_num->render();
	camera_pos_z_num->render();
	camera_rotate_x_num->render();
}