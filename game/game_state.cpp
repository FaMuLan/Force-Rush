#include "game_state.h"
#include "../button.h"
#include "../sprite.h"
#include "../system.h"
#include "../text_area.h"
#include "../texture_manager.h"
#include "../timer.h"
#include "../song_data.h"
#include "../select/select_state.h"
#include "../select/song_list.h"
#include "../loading/loading_state.h"
#include "../user/setting.h"
#include "beatmap.h"

fr::GameState *fr::GameState::m_instance = 0;

void fr::GameState::init()
{
	GameBeatmap::instance()->load(m_information);

	title_base = new Button;
	score_base = new Sprite;
	score_text = new TextArea;
	wall_l = new Sprite;
	wall_r = new Sprite;
	background_l = new Sprite;
	background_r = new Sprite;
	TextureManager::instance()->loadfont("assets/fonts/Audiowide.ttf", 36);
	TextureManager::instance()->loadfont("assets/fonts/Audiowide.ttf", 56);
	title_base->init("assets/game/title_base.png", System::instance()->GetWindowWidth() / 2 - 360, 0);
	title_base->AddPressedFrame("assets/game/title_base_pressed.png");
	title_base->AddText(m_information->title, title_base->GetW() / 2, title_base->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
	score_base->init("assets/game/score_base.png", 0, 64);
	score_text->init("0", 32, 96, "assets/fonts/Audiowide.ttf", 56, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);

	wall_l->init("assets/game/wall_l.png");
	wall_r->init("assets/game/wall_r.png");
	background_l->init("assets/game/background_l.png");
	background_r->init("assets/game/background_r.png");
	background_l->SetSize(327.f * GameBeatmap::instance()->GetScaleW(), 625.f * GameBeatmap::instance()->GetScaleH());
	background_r->SetSize(327.f * GameBeatmap::instance()->GetScaleW(), 625.f * GameBeatmap::instance()->GetScaleH());
	background_l->SetPos(0, 328 * GameBeatmap::instance()->GetScaleH() + Setting::instance()->GetDrawOffset());
	background_r->SetPos(System::instance()->GetWindowWidth() - background_r->GetW(), 328 * GameBeatmap::instance()->GetScaleH() + Setting::instance()->GetDrawOffset());

	wall_l->SetSize(228.f * GameBeatmap::instance()->GetScaleW(), 584.f * GameBeatmap::instance()->GetScaleH());
	wall_r->SetSize(228.f * GameBeatmap::instance()->GetScaleW(), 584.f * GameBeatmap::instance()->GetScaleH());
}

void fr::GameState::clear()
{
	title_base->clear();
	GameBeatmap::instance()->clear();
}

void fr::GameState::update()
{
	char *score_ch = new char[6];
	sprintf(score_ch, "%d", GameBeatmap::instance()->GetScore());
	score_text->SetText(score_ch);
	delete [] score_ch;

	GameBeatmap::instance()->update();
	if (System::instance()->IsWindowModified())
	{
		title_base->SetPos(System::instance()->GetWindowWidth() / 2 - title_base->GetW() / 2, 0);
		background_l->SetSize(327.f * GameBeatmap::instance()->GetScaleW(), 625.f * GameBeatmap::instance()->GetScaleH());
		background_r->SetSize(327.f * GameBeatmap::instance()->GetScaleW(), 625.f * GameBeatmap::instance()->GetScaleH());
		background_l->SetPos(0, 328 * GameBeatmap::instance()->GetScaleH() + Setting::instance()->GetDrawOffset());
		background_r->SetPos(System::instance()->GetWindowWidth() - background_r->GetW(), 328 * GameBeatmap::instance()->GetScaleH() + Setting::instance()->GetDrawOffset());
		wall_l->SetSize(228.f * GameBeatmap::instance()->GetScaleW(), 584.f * GameBeatmap::instance()->GetScaleH());
		wall_r->SetSize(228.f * GameBeatmap::instance()->GetScaleW(), 584.f * GameBeatmap::instance()->GetScaleH());
	}

	title_base->update();

	DrawWall();
	GameBeatmap::instance()->render();
	score_base->render();
	score_text->render();
	title_base->render();
	if (title_base->IsReleased())
	{
		LoadingState::instance()->init(SelectState::instance(), this);
	}
}

void fr::GameState::SetFile(fr::SongInformation *load_information)
{
	m_information = load_information;
}

void fr::GameState::DrawWall()
{
	static const int start_x = 334;
	static const int start_y = 321;
	static const int end_x = 0;
	static const int end_y = 364;
	static const float start_scale = 0.08386f;
	double process = double(Timer::instance()->GetSystemTime()) / double(Setting::instance()->GetDuration());
//	process = process - int(process) + 1;
	double process_sq = process * process;
	background_l->render();
	background_r->render();
	while (process > 0)
	{
		if (process < 1.6f)
		{
			int current_x = start_x + (end_x - start_x) * process_sq;
			int current_y = start_y + (end_y - start_y) * process_sq;
			float current_scale = start_scale + (1.0f - start_scale) * process_sq;
			wall_l->SetPos(current_x * GameBeatmap::instance()->GetScaleW(), current_y * GameBeatmap::instance()->GetScaleH() + Setting::instance()->GetDrawOffset());
			wall_l->SetScale(current_scale);
			wall_l->render();
			wall_r->SetPos((System::instance()->GetWindowWidth() - current_x * GameBeatmap::instance()->GetScaleW() - wall_r->GetW() * current_scale), current_y * GameBeatmap::instance()->GetScaleH() + Setting::instance()->GetDrawOffset());
			wall_r->SetScale(current_scale);
			wall_r->render();
		}
		process -= 0.28f;
		process_sq = process * process * process;
	}
}