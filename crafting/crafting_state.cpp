#include "crafting_state.h"

lm::CraftingState *lm::CraftingState::m_instance = 0;

void lm::CraftingState::init()
{
	crafting_back = new Button;
	background = new Sprite;
	crafting_back->load("assets/sortbutton.png", "assets/sortbutton_pressed.png", 0, 660, 180, 81);
	background->load("assets/bg_light.png", 0, 0, 1286, 965);
	TextureManager::instance()->loadfont("assets/GeosansLight.ttf", 30);
	crafting_back->SetText("Back", "assets/GeosansLight.ttf", 0x00, 0x00, 0x00);
}

void lm::CraftingState::clear()
{
	
}

void lm::CraftingState::update()
{
	crafting_back->update();

	background->render();
	crafting_back->render();
	TextureManager::instance()->render("Building... Coming Soon!", System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2, "assets/GeosansLight.ttf", 0x00, 0x00, 0x00);
	
	if (crafting_back->IsReleased())
	{
		LoadingState::instance()->init(MainState::instance(), this);
		DebugWidget::instance()->PushLog("Back to main menu from crafting_state");
	}
}