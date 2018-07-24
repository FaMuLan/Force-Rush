#include "game_state.h"
#include "../gui/button.h"
#include "../sprite.h"
#include "../system.h"
#include "../gui/text_area.h"
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
}

void fr::GameState::clear()
{
	GameBeatmap::instance()->clear();
	GameHeader::instance()->clear();
}

void fr::GameState::update()
{
	GameBeatmap::instance()->update();
	GameHeader::instance()->update();

	GameBeatmap::instance()->render();
	GameHeader::instance()->render();
}

void fr::GameState::SetFile(fr::SongInformation *load_information)
{
	m_information = load_information;
}