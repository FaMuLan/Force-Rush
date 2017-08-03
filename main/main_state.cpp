#include "main_state.h"

lm::MainState *lm::MainState::m_instance = 0;

void lm::MainState::init()
{
	main_start = new Button;
	main_debug = new Button;
	main_about = new Button;
	main_character = new Sprite;
	background = new Sprite;

	main_start->load("assets/main_musicplay.png", "assets/main_musicplay_selected.png", 180, 160, 492, 129);
	main_debug->load("assets/main_network_connected.png", "assets/main_network_pressed.png", 90, 330, 492, 129);
	main_about->load("assets/main_button_narrow.png", "assets/main_button_narrow_pressed.png", 180, 500, 313, 129);
	main_character->load("assets/character.png", 450, 0, 1200, 1200);
	background->load("assets/bg_light.png", 0, 0, 1286, 965);
	TextureManager::instance()->loadfont("assets/GeosansLight-Oblique.ttf", 60);
	main_start->SetText("Music Play", "assets/GeosansLight-Oblique.ttf", 0xFF, 0xFF, 0xFF);
	main_debug->SetText("Debuger", "assets/GeosansLight-Oblique.ttf", 0xFF, 0xFF, 0xFF);
	main_about->SetText("About", "assets/GeosansLight-Oblique.ttf", 0xFF, 0xFF, 0xFF);

	AboutSidedialog::instance()->init();
	is_locked = false;

//	SoundManager::instance()->load("assets/BGM.ogg", SOUNDTYPE_MUSIC);
//	SoundManager::instance()->play("assets/BGM.ogg", SOUNDTYPE_MUSIC);
}

void lm::MainState::clear()
{
	main_start->clear();
	main_debug->clear();
	main_about->clear();
	main_character->clear();
	background->clear();
	TextureManager::instance()->clearfont("assets/GeosansLight-Oblique.ttf");
}

void lm::MainState::update()
{
	if (!is_locked)
	{
		main_start->update();
		main_debug->update();
		main_about->update();
	}
	//鎖定按鈕在打開側邊欄或彈窗時不進行檢測
	AboutSidedialog::instance()->update();

	background->render();
	main_character->render();
	main_start->render();
	main_debug->render();
	main_about->render();
	AboutSidedialog::instance()->render();

	if (main_start->IsReleased())
	{
		LoadingState::instance()->init(SelectState::instance(), this);
		DebugWidget::instance()->PushLog("Leaving main menu");
//		SoundManager::instance()->stop();
	}
	if (main_debug->IsReleased())
	{
		DebugWidget::instance()->ShowWidget(!DebugWidget::instance()->IsShowing());
	}
	if (main_about->IsReleased())
	{
		AboutSidedialog::instance()->open();
	}
}

void lm::MainState::lock(bool load)
{
	is_locked = load;
	//這樣鎖定和解鎖可以用在同一個函數。
}