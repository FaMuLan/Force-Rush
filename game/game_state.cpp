#include "game_state.h"
#include "../button.h"
#include "../sprite.h"
#include "../system.h"
#include "../text_area.h"
#include "../texture_manager.h"
#include "../timer.h"
#include "../song_data.h"
#include "../sound_manager.h"
#include "../prepare/song_list.h"
#include "../loading/loading_state.h"
#include "../user/setting.h"
#include "../user/user_profile.h"
#include "beatmap.h"
#include "game_header.h"

fr::GameState *fr::GameState::m_instance = 0;

void fr::GameState::init()
{
	GameBeatmap::instance()->load(m_information);
	GameHeader::instance()->init();

	wall_l = new Sprite;
	wall_r = new Sprite;
	background_l = new Sprite;
	background_r = new Sprite;

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
	GameBeatmap::instance()->clear();
}

void fr::GameState::update()
{
	GameBeatmap::instance()->update();
	GameHeader::instance()->update();

	if (System::instance()->IsWindowModified())
	{
		background_l->SetSize(327.f * GameBeatmap::instance()->GetScaleW(), 625.f * GameBeatmap::instance()->GetScaleH());
		background_r->SetSize(327.f * GameBeatmap::instance()->GetScaleW(), 625.f * GameBeatmap::instance()->GetScaleH());
		background_l->SetPos(0, 328 * GameBeatmap::instance()->GetScaleH() + Setting::instance()->GetDrawOffset());
		background_r->SetPos(System::instance()->GetWindowWidth() - background_r->GetW(), 328 * GameBeatmap::instance()->GetScaleH() + Setting::instance()->GetDrawOffset());
		wall_l->SetSize(228.f * GameBeatmap::instance()->GetScaleW(), 584.f * GameBeatmap::instance()->GetScaleH());
		wall_r->SetSize(228.f * GameBeatmap::instance()->GetScaleW(), 584.f * GameBeatmap::instance()->GetScaleH());
	}

	DrawWall();
	GameBeatmap::instance()->render();
//	UserProfile::instance()->RenderController();
	GameHeader::instance()->render();
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
	double process = double(Timer::instance()->GetTime("game") + 2000) / double(Setting::instance()->GetDuration());
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