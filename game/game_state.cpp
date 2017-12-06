#include "game_state.h"
#include "../button.h"
#include "../sprite.h"
#include "../system.h"
#include "../text_area.h"
#include "../texture_manager.h"
#include "../song_data.h"
#include "../select/select_state.h"
#include "../select/song_list.h"
#include "../loading/loading_state.h"
#include "../user/character.h"
#include "beatmap.h"

lm::GameState *lm::GameState::m_instance = 0;

void lm::GameState::init()
{
	title_base = new Button;
	score_base = new Sprite;
	score_text = new TextArea;
	TextureManager::instance()->loadfont("assets/fonts/Audiowide.ttf", 36);
	TextureManager::instance()->loadfont("assets/fonts/Audiowide.ttf", 56);
	title_base->init("assets/game/title_base.png", System::instance()->GetWindowWidth() / 2 - 360, 0);
	title_base->AddPressedFrame("assets/game/title_base_pressed.png");
	title_base->AddText(m_information->title, title_base->GetW() / 2, title_base->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
	score_base->init("assets/game/score_base.png", 0, 64);
	score_text->init("0", 32, 96, "assets/fonts/Audiowide.ttf", 56, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);
	Beatmap::instance()->load(m_information);
}

void lm::GameState::clear()
{
	title_base->clear();
	Beatmap::instance()->clear();
}

void lm::GameState::update()
{
	char *score_ch = new char[6];
	sprintf(score_ch, "%d", Beatmap::instance()->GetScore());
	score_text->SetText(score_ch);
	delete [] score_ch;
	if (System::instance()->IsWindowModified())
	{
		title_base->SetPos(System::instance()->GetWindowWidth() / 2 - title_base->GetW() / 2, 0);
	}
	Character::instance()->render();
	title_base->update();
	Beatmap::instance()->update();

	Beatmap::instance()->render();

	score_base->render();
	score_text->render();
	title_base->render();
	if (title_base->IsReleased())
	{
		LoadingState::instance()->init(SelectState::instance(), this);
	}
}

void lm::GameState::SetFile(lm::SongInformation *load_information)
{
	m_information = load_information;
}