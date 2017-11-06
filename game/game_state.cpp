#include "game_state.h"
#include "../button.h"
#include "beatmap.h"
#include "../main/main_state.h"
#include "../loading/loading_state.h"
#include "../user/character.h"

lm::GameState *lm::GameState::m_instance = 0;

void lm::GameState::init()
{
	game_pause = new Button;
	game_pause->init("assets/sort_button.png", 0, 0, 176, 80);
	game_pause->AddPressedFrame("assets/sort_button_pressed.png");
	game_pause->SetText("Pause", "assets/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
	Beatmap::instance()->load(file_path);
}

void lm::GameState::clear()
{
	game_pause->clear();
	Beatmap::instance()->clear();
}

void lm::GameState::update()
{
	Character::instance()->render();
	game_pause->update();
	Beatmap::instance()->update();

	Beatmap::instance()->render();

	if (game_pause->IsReleased())
	{
		LoadingState::instance()->init(MainState::instance(), this);
	}

	game_pause->render();
}

void lm::GameState::SetFile(std::string load_path)
{
	file_path = load_path;
}