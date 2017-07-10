#include "main_state.h"

lm::MainState *lm::MainState::m_instance = 0;

void lm::MainState::init()
{
	main_start = new Button;
	main_network = new Button;
	main_about = new Button;
	main_character = new Sprite;
	background = new Sprite;
	main_start->load("assets/main_musicplay.png", "assets/main_musicplay_selected.png", 180, 160, 492, 129);
	main_network->load("assets/main_network_connected.png", "assets/main_network_pressed.png", 90, 330, 492, 129);
	main_about->load("assets/main_button_narrow.png", "assets/main_button_narrow_pressed.png", 180, 500, 313, 129);
	main_character->load("assets/character.png", 450, 0, 1200, 1200);
	background->load("assets/bg_light.png", 0, 0, 1286, 965);
	TextureManager::instance()->loadfont("assets/GeosansLight-Oblique.ttf", 60);
	main_start->SetText("Music Play", "assets/GeosansLight-Oblique.ttf", 0xFF, 0xFF, 0xFF);
	main_network->SetText("Network", "assets/GeosansLight-Oblique.ttf", 0xFF, 0xFF, 0xFF);
	main_about->SetText("About", "assets/GeosansLight-Oblique.ttf", 0xFF, 0xFF, 0xFF);
}

void lm::MainState::clear()
{
	main_start->clear();
	main_network->clear();
	main_about->clear();
	main_character->clear();
	background->clear();
	TextureManager::instance()->clearfont("assets/GeosansLight-Oblique.ttf");
}

void lm::MainState::update()
{
	main_start->update();
	main_network->update();
	main_about->update();

	background->render();
	main_character->render();
	main_start->render();
	main_network->render();
	main_about->render();

	if (main_start->IsReleased())
	{
		LoadingState::instance()->init(CraftingState::instance(), this);
	}
}
