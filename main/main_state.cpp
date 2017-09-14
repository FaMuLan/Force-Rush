#include "main_state.h"
#include "../sprite.h"
#include "../button.h"
#include "../sound_manager.h"
#include "../texture_manager.h"
#include "../system.h"
#include "../loading/loading_state.h"
#include "../crafting/crafting_state.h"
#include "../select/select_state.h"

lm::MainState *lm::MainState::m_instance = 0;

void lm::MainState::init()
{
	main_start = new Button;
	main_about = new Button;
	background = new Sprite;

	main_start->load("assets/main_button.png", "assets/main_button_pressed.png", 162, 600, 397, 108);
	main_about->load("assets/main_button.png", "assets/main_button_pressed.png", 162, 800, 397, 108);
	background->load("assets/background.jpg", 0, 0, 720, 1440);
	TextureManager::instance()->loadfont("assets/Audiowide.ttf", 60);

	is_locked = false;

	main_start->SetText("Start", "assets/Audiowide.ttf", 60, 0x00, 0x00, 0x00);
	main_about->SetText("About", "assets/Audiowide.ttf", 60, 0x00, 0x00, 0x00);

//	SoundManager::instance()->load("assets/BGM.wav", SOUNDTYPE_MUSIC);
//	SoundManager::instance()->play("assets/BGM.wav", SOUNDTYPE_MUSIC);
}

void lm::MainState::clear()
{
	main_start->clear();
	main_about->clear();
	background->clear();
	TextureManager::instance()->clearfont("assets/Audiowide.ttf", 60);
}

void lm::MainState::update()
{
	if (!is_locked)
	{
		main_start->update();
		main_about->update();
	}
	//鎖定按鈕在打開側邊欄或彈窗時不進行檢測

	background->render();
	main_start->render();
	main_about->render();

	if (main_start->IsReleased())
	{
		LoadingState::instance()->init(this, this);
		LoadingState::instance()->AddText("Loop", "assets/Audiowide.ttf", 80, System::instance()->GetWindowWidth() / 2, 100, 0x00, 0x00, 0x00, SHUTTER_TOP);
//		SoundManager::instance()->stop();
	}
}

void lm::MainState::lock(bool load)
{
	is_locked = load;
	//這樣鎖定和解鎖可以用在同一個函數。
}