#include "beatmap.h"
#include <string>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "column.h"
#include "game_state.h"
#include "result_state.h"
#include "../prepare/song_list.h"
#include "../loading/loading_state.h"
#include "../user/setting.h"
#include "../control_handler.h"
#include "../sound_manager.h"
#include "../data.h"
#include "../texture_manager.h"
#include "../file_system.h"
#include "../system.h"
#include "../timer.h"
#include "../gui/text_area.h"
#include "../sprite.h"
#include "../animator.h"

fr::GameBeatmap *fr::GameBeatmap::m_instance = 0;

void fr::Beatmap::init()
{
	play_base = new Sprite;
	wall = new Sprite;
	ground = new Sprite;
	play_base->init("assets/game/play_base_header.png");
	play_base->AddFrame("assets/game/play_base_body.png");
	wall->init("assets/game/wall.png");
	ground->init("assets/game/ground.png");
	Animator::instance()->AddAnimation("rotate", ANIMATIONTYPE_UNIFORMLY_DECELERATED, 300);
	wall_vectrices = new float[24];
	ground_vectrices = new float[24];
	play_base_vectrices = new float[24];
	force = 0;
	last_force = 0;
	current_angle = 0;
	last_angle = 0;
	angle_diff = 0;
	last_frame_time = 0;

	wall_vectrices[0] = -360.f / 360.f;
	wall_vectrices[1] = 0;
	wall_vectrices[2] = 0;
	wall_vectrices[3] = 1;
	wall_vectrices[4] = 0;
	wall_vectrices[5] = 0;
	//top left
	wall_vectrices[6] = -360.f / 360.f;
	wall_vectrices[7] = 0;
	wall_vectrices[8] = wall->GetW() / 360.f;
	wall_vectrices[9] = 1;
	wall_vectrices[10] = 1;
	wall_vectrices[11] = 0;
	//top right
	wall_vectrices[12] = -360.f / 360.f;
	wall_vectrices[13] = -360.f / 360.f;
	wall_vectrices[14] = 0;
	wall_vectrices[15] = 1;
	wall_vectrices[16] = 0;
	wall_vectrices[17] = 1;
	//bottom left
	wall_vectrices[18] = -360.f / 360.f;
	wall_vectrices[19] = -360.f / 360.f;
	wall_vectrices[20] = wall->GetW() / 360.f;
	wall_vectrices[21] = 1;
	wall_vectrices[22] = 1;
	wall_vectrices[23] = 1;
	//bottom right

	ground_vectrices[0] = -360.f / 360.f;
	ground_vectrices[1] = 0;
	ground_vectrices[2] = 0;
	ground_vectrices[3] = 1;
	ground_vectrices[4] = 0;
	ground_vectrices[5] = 0;
	//top left
	ground_vectrices[6] = 360.f / 360.f;
	ground_vectrices[7] = 0;
	ground_vectrices[8] = 0;
	ground_vectrices[9] = 1;
	ground_vectrices[10] = 1;
	ground_vectrices[11] = 0;
	//top right
	ground_vectrices[12] = -360.f / 360.f;
	ground_vectrices[13] = 0;
	ground_vectrices[14] = ground->GetH() / 320.f;
	ground_vectrices[15] = 1;
	ground_vectrices[16] = 0;
	ground_vectrices[17] = 1;
	//bottom left
	ground_vectrices[18] = 360.f / 360.f;
	ground_vectrices[19] = 0;
	ground_vectrices[20] = ground->GetH() / 360.f;
	ground_vectrices[21] = 1;
	ground_vectrices[22] = 0;
	ground_vectrices[23] = 0;
	//bottom right

	play_base_vectrices[0] = -360.f / 360.f;
	play_base_vectrices[1] = -360.f / 360.f;
	play_base_vectrices[2] = -play_base->GetH() / 360.f;
	play_base_vectrices[3] = 1;
	play_base_vectrices[4] = 0;
	play_base_vectrices[5] = 0;
	//top left
	play_base_vectrices[6] = 360.f / 360.f;
	play_base_vectrices[7] = -360.f / 360.f;
	play_base_vectrices[8] = -play_base->GetH() / 360.f;
	play_base_vectrices[9] = 1;
	play_base_vectrices[10] = 1;
	play_base_vectrices[11] = 0;
	//top right
	play_base_vectrices[12] = -360.f / 360.f;
	play_base_vectrices[13] = -360.f / 360.f;
	play_base_vectrices[14] = 0;
	play_base_vectrices[15] = 1;
	play_base_vectrices[16] = 0;
	play_base_vectrices[17] = 1;
	//bottom left
	play_base_vectrices[18] = 360.f / 360.f;
	play_base_vectrices[19] = -360.f / 360.f;
	play_base_vectrices[20] = 0;
	play_base_vectrices[21] = 1;
	play_base_vectrices[22] = 1;
	play_base_vectrices[23] = 1;
	//bottom right
	wall->SetVectrices(wall_vectrices);
	wall->SetMatrix("mvp");
	ground->SetVectrices(ground_vectrices);
	ground->SetMatrix("mvp");
	play_base->SetVectrices(play_base_vectrices);
	play_base->SetMatrix("mvp");

	glm::mat4x4 model_view_matrix;
	model_view_matrix = glm::mat4(1.f);
	model_view_matrix = glm::translate(model_view_matrix, glm::vec3(0.f, -Setting::instance()->GetCameraPosY() / float(System::instance()->GetWindowHeigh()) * 2.f - 1.f, -Setting::instance()->GetCameraPosZ() / 360.f));
	model_view_matrix = glm::rotate(model_view_matrix, glm::radians(float(Setting::instance()->GetCameraRotateX())), glm::vec3(1.0, 0.0, 0.0));
	model_view_matrix = glm::rotate(model_view_matrix, 0.0f, glm::vec3(0.0, 0.0, 1.0));
	TextureManager::instance()->LoadMatrix("model_view", model_view_matrix);
	TextureManager::instance()->update();

	for (int i = 0; i < 4; i++)
	{
		Column *new_column = new Column;
		new_column->init(i, this);
		m_column.push_back(new_column);
	}
}

void fr::Beatmap::clear()
{
	for (int i = 0; i < m_column.size(); i++)
	{
		m_column[i]->clear();
	}
	m_column.clear();
	SoundManager::instance()->GetProcess("default_music")->is_playing = false;
	SoundManager::instance()->clear(audio_path);
}

void fr::Beatmap::update()
{
	force = 0;
	if (System::instance()->IsWindowModified())
	{
		wall_vectrices[13] = System::instance()->GetWindowHeigh();
		wall_vectrices[19] = System::instance()->GetWindowHeigh();
		ground_vectrices[6] = System::instance()->GetWindowWidth();
		ground_vectrices[18] = System::instance()->GetWindowWidth();
		play_base_vectrices[1] = System::instance()->GetWindowHeigh();
		play_base_vectrices[6] = System::instance()->GetWindowWidth();
		play_base_vectrices[7] = System::instance()->GetWindowHeigh();
		play_base_vectrices[13] = System::instance()->GetWindowHeigh();
		play_base_vectrices[18] = System::instance()->GetWindowWidth();
		play_base_vectrices[19] = System::instance()->GetWindowHeigh();
		play_base->SetVectrices(play_base_vectrices);
	}

	for (int i = 0; i < ControlHandler::instance()->GetFingerCount(); i++)
	{
		Finger load_finger = ControlHandler::instance()->GetFinger(i);
		if (load_finger.y < System::instance()->GetWindowHeigh() / 2)
		{
			Setting::instance()->SetSpeed(Setting::instance()->GetSpeed() - load_finger.dy / 8.f);
		}
		else
		{
//			int drag_angle = float(load_finger.x - load_finger.startx) / System::instance()->GetWindowWidth() * Setting::instance()->GetForceAngle();
//			current_angle += drag_angle;
//			force = current_angle;
		}
	}

	for (int i = 0; i < m_column.size(); i++)
	{
		m_column[i]->update();
	}

	if (last_force != force)
	{
		Animator::instance()->ResetAnimation("rotate");
		Animator::instance()->Animate("rotate");
		last_angle = current_angle;
		angle_diff = force - last_angle;
	}
	if (!Animator::instance()->IsTimeUp("rotate"))
	{
		float process = Animator::instance()->GetProcess("rotate");
		current_angle = last_angle + angle_diff * process;
	}
	else
	{
		current_angle = last_angle + angle_diff;
	}

	glm::mat4x4 model_view_matrix;
	model_view_matrix = glm::mat4(1.f);
	model_view_matrix = glm::translate(model_view_matrix, glm::vec3(0.f, -Setting::instance()->GetCameraPosY() / float(System::instance()->GetWindowHeigh()) * 2.f - 1.f, -Setting::instance()->GetCameraPosZ() / 360.f));
	model_view_matrix = glm::rotate(model_view_matrix, glm::radians(float(Setting::instance()->GetCameraRotateX())), glm::vec3(1.0, 0.0, 0.0));
	model_view_matrix = glm::rotate(model_view_matrix, glm::radians(current_angle), glm::vec3(0.0, 0.0, 1.0));
	TextureManager::instance()->LoadMatrix("model_view", model_view_matrix);

	last_force = force;
}

void fr::Beatmap::render()
{
	int current_time = Timer::instance()->GetTime("game");
	int wall_z;
	int play_base_z = 0;
	float avg_speed = 0;
	for (int i = 0; i < m_column.size(); i++)
	{
		avg_speed += m_column[i]->GetCurrentSpeed() * 0.25f;
	}
	background_process += float((current_time - last_frame_time) / float(50000.f / Setting::instance()->GetSpeed())) * avg_speed;
	background_process -= int(background_process);
	wall_z = -background_process * (System::instance()->GetWindowDepth() - (System::instance()->GetWindowDepth() % wall->GetW()));
	while (wall_z < System::instance()->GetWindowDepth())
	{
		wall_vectrices[2] = wall_z;
		wall_vectrices[8] = wall_z + wall->GetW();
		wall_vectrices[14] = wall_z;
		wall_vectrices[20] = wall_z + wall->GetW();
		ground_vectrices[2] = wall_z + ground->GetH();
		ground_vectrices[8] = wall_z + ground->GetH();
		ground_vectrices[14] = wall_z;
		ground_vectrices[20] = wall_z;
		wall_vectrices[0] = 0;
		wall_vectrices[6] = 0;
		wall_vectrices[12] = 0;
		wall_vectrices[18] = 0;
		wall->SetVectrices(wall_vectrices);
		wall->render();
		wall_vectrices[0] = System::instance()->GetWindowWidth();
		wall_vectrices[6] = System::instance()->GetWindowWidth();
		wall_vectrices[12] = System::instance()->GetWindowWidth();
		wall_vectrices[18] = System::instance()->GetWindowWidth();
		wall->SetVectrices(wall_vectrices);
		wall->render();
		ground->SetVectrices(ground_vectrices);
		ground->render();
		wall_z += wall->GetW();
	}

	play_base_vectrices[2] = play_base_z + play_base->GetH();
	play_base_vectrices[8] = play_base_z + play_base->GetH();
	play_base_vectrices[14] = play_base_z;
	play_base_vectrices[20] = play_base_z;
	play_base->SetVectrices(play_base_vectrices);
	play_base->render(0);
	play_base_z += play_base->GetH();
	while (play_base_z < System::instance()->GetWindowDepth())
	{
		play_base_vectrices[2] = play_base_z + play_base->GetH();
		play_base_vectrices[8] = play_base_z + play_base->GetH();
		play_base_vectrices[14] = play_base_z;
		play_base_vectrices[20] = play_base_z;
		play_base->SetVectrices(play_base_vectrices);
		play_base->render(1);
		play_base_z += play_base->GetH();
	}

	for (int i = 0; i < m_column.size(); i++)
	{
		m_column[i]->render();
	}
	last_frame_time = current_time;
}

void fr::Beatmap::AddForce(float angle)
{
	force += angle;
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

	chain_text = new TextArea;
	judge_text = new TextArea;
	early_text = new TextArea;
	late_text = new TextArea;
	show_early = false;
	show_late = false;
	chain_text->init(" ", Point2Di(System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2), "assets/fonts/Audiowide.ttf", 140, Color(0xBB, 0xBB, 0xBB));
	judge_text->init(" ", Point2Di(System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2 + 120), "assets/fonts/Audiowide.ttf", 40, Color(0xBB, 0xBB, 0xBB));
	early_text->init("EARLY", Point2Di(System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2 + 80), "assets/fonts/Audiowide.ttf", 40, Color(0xBB, 0xBB, 0xBB));
	late_text->init("LATE", Point2Di(System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2 + 160), "assets/fonts/Audiowide.ttf", 40, Color(0xBB, 0xBB, 0xBB));
	Animator::instance()->AddAnimation("chain", ANIMATIONTYPE_UNIFORMLY_DECELERATED, 300);
	Animator::instance()->ResetAnimation("chain");

	audio_path = m_information->audio_path;
	bool is_mapped = false;
	is_waiting = true;
	is_ended = false;
	SoundManager::instance()->GetProcess("default_music")->path = audio_path;

	std::vector<NoteSet*> new_note_set;
	LoadBeatmapFile(m_information->file_path, NULL, &new_note_set);

	m_column[0]->AddNote(new_note_set[0]);
	m_column[1]->AddNote(new_note_set[1]);
	m_column[2]->AddNote(new_note_set[2]);
	m_column[3]->AddNote(new_note_set[3]);
//	SoundManager::instance()->load(audio_path);
	SoundManager::instance()->seek("default_music", 0);

	Timer::instance()->RunTimer("game");
}

void fr::GameBeatmap::clear()
{
	Beatmap::clear();
	Timer::instance()->ResetTimer("game");
	chain_text->clear();
	judge_text->clear();
	early_text->clear();
	late_text->clear();
	delete chain_text;
	delete judge_text;
	delete early_text;
	delete late_text;
}

void fr::GameBeatmap::update()
{
	Beatmap::update();
	if (Timer::instance()->GetTime("game") > 2000 && is_waiting)
	{
		SoundManager::instance()->play("default_music");
		SoundManager::instance()->seek("default_music", Timer::instance()->GetTime("game") - 2000);
		is_waiting = false;
	}
	if (System::instance()->IsWindowModified())
	{
		chain_text->SetPos(System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2);
		judge_text->SetPos(System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2 + 120);
		early_text->SetPos(System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2 + 80);
		late_text->SetPos(System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2 + 160);
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
	chain_text->render(chain_text->GetUserX(), chain_text->GetUserY() - 100.f * (1.f - Animator::instance()->GetProcess("chain")));
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
		if (time_diff > 150 && !is_ln_pressing)
		{
			return JUDGEMENT_NONE;
		}
		else if (time_diff > 100 && is_ln_pressing)
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
		else if (time_diff > 100 && !is_ln_pressing)
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
			if (time_diff > 50 || time_diff < -50)
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
			else if (time_diff > 25 || time_diff < -25)
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
	else if (time_diff < -100)
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