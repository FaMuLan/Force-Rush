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
	wall = new Sprite;
	ground = new Sprite;
	play_base->init("assets/game/play_base_header.png");
	play_base->AddFrame("assets/game/play_base_body.png");
	wall->init("assets/game/wall.png");
	ground->init("assets/game/ground.png");
	wall_vectrices = new int[24];
	ground_vectrices = new int[24];
	play_base_vectrices = new int [24];

	wall_vectrices[0] = 0;
	wall_vectrices[1] = 0;
	wall_vectrices[2] = 0;
	wall_vectrices[3] = 1;
	wall_vectrices[4] = 0;
	wall_vectrices[5] = 0;
	//top left
	wall_vectrices[6] = 0;
	wall_vectrices[7] = 0;
	wall_vectrices[8] = wall->GetW();
	wall_vectrices[9] = 1;
	wall_vectrices[10] = wall->GetW();
	wall_vectrices[11] = 0;
	//top right
	wall_vectrices[12] = 0;
	wall_vectrices[13] = System::instance()->GetWindowHeigh();
	wall_vectrices[14] = 0;
	wall_vectrices[15] = 1;
	wall_vectrices[16] = 0;
	wall_vectrices[17] = wall->GetH();
	//bottom left
	wall_vectrices[18] = 0;
	wall_vectrices[19] = System::instance()->GetWindowHeigh();
	wall_vectrices[20] = wall->GetW();
	wall_vectrices[21] = 1;
	wall_vectrices[22] = wall->GetW();
	wall_vectrices[23] = wall->GetH();
	//bottom right
	wall->SetVectrices(wall_vectrices);

	ground_vectrices[0] = 0;
	ground_vectrices[1] = 0;
	ground_vectrices[2] = 0;
	ground_vectrices[3] = 1;
	ground_vectrices[4] = 0;
	ground_vectrices[5] = 0;
	//top left
	ground_vectrices[6] = System::instance()->GetWindowWidth();
	ground_vectrices[7] = 0;
	ground_vectrices[8] = 0;
	ground_vectrices[9] = 1;
	ground_vectrices[10] = ground->GetW();
	ground_vectrices[11] = 0;
	//top right
	ground_vectrices[12] = 0;
	ground_vectrices[13] = 0;
	ground_vectrices[14] = ground->GetH();
	ground_vectrices[15] = 1;
	ground_vectrices[16] = 0;
	ground_vectrices[17] = ground->GetH();
	//bottom left
	ground_vectrices[18] = System::instance()->GetWindowWidth();
	ground_vectrices[19] = 0;
	ground_vectrices[20] = ground->GetH();
	ground_vectrices[21] = 1;
	ground_vectrices[22] = ground->GetW();
	ground_vectrices[23] = ground->GetH();
	//bottom right

	play_base_vectrices[0] = 0;
	play_base_vectrices[1] = System::instance()->GetWindowHeigh();
	play_base_vectrices[2] = play_base->GetH();
	play_base_vectrices[3] = 1;
	play_base_vectrices[4] = 0;
	play_base_vectrices[5] = 0;
	//top left
	play_base_vectrices[6] = System::instance()->GetWindowWidth();
	play_base_vectrices[7] = System::instance()->GetWindowHeigh();
	play_base_vectrices[8] = play_base->GetH();
	play_base_vectrices[9] = 1;
	play_base_vectrices[10] = play_base->GetW();
	play_base_vectrices[11] = 0;
	//top right
	play_base_vectrices[12] = 0;
	play_base_vectrices[13] = System::instance()->GetWindowHeigh();
	play_base_vectrices[14] = 0;
	play_base_vectrices[15] = 1;
	play_base_vectrices[16] = 0;
	play_base_vectrices[17] = play_base->GetH();
	//bottom left
	play_base_vectrices[18] = System::instance()->GetWindowWidth();
	play_base_vectrices[19] = System::instance()->GetWindowHeigh();
	play_base_vectrices[20] = 0;
	play_base_vectrices[21] = 1;
	play_base_vectrices[22] = play_base->GetW();
	play_base_vectrices[23] = play_base->GetH();
	//bottom right
	ground->SetVectrices(ground_vectrices);
	play_base->SetVectrices(play_base_vectrices);
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
			Setting::instance()->SetDuration(Setting::instance()->GetDuration() + load_finger.dy);
		}
	}

	for (int i = 0; i < m_column.size(); i++)
	{
		m_column[i]->update();
	}

	Matrix perspective_matrix;
	Matrix model_view_matrix;
	perspective_matrix.LoadIdentity();
	model_view_matrix.LoadIdentity();

	perspective_matrix.Perspective(70, float(System::instance()->GetWindowWidth()) / float(System::instance()->GetWindowHeigh()), 1.f, 20.f);
	model_view_matrix.Translate(0.f, -Setting::instance()->GetCameraPosY() / float(System::instance()->GetWindowHeigh()) * 2.f - 1.f, -Setting::instance()->GetCameraPosZ() / 720.f);
	model_view_matrix.Rotate(Setting::instance()->GetCameraRotateX(), 1.0, 0.0, 0.0);
	Matrix *mvp_matrix = new Matrix;
	*mvp_matrix = model_view_matrix * perspective_matrix;
	TextureManager::instance()->SetMvpMatrix(mvp_matrix);
}

void fr::Beatmap::render()
{
	int wall_z;
	int play_base_z = 0;
	float background_process = float(Timer::instance()->GetTime("game") % Setting::instance()->GetDuration()) / float(Setting::instance()->GetDuration());
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
	chain_text->init(" ", System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2, "assets/fonts/Audiowide.ttf", 140, 0xBB, 0xBB, 0xBB);
	judge_text->init(" ", System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2 + 120, "assets/fonts/Audiowide.ttf", 40, 0xBB, 0xBB, 0xBB);
	early_text->init("EARLY", System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2 + 80, "assets/fonts/Audiowide.ttf", 40, 0xBB, 0xBB, 0xBB);
	late_text->init("LATE", System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2 + 160, "assets/fonts/Audiowide.ttf", 40, 0xBB, 0xBB, 0xBB);
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