#include "main_state.h"
#include "../sprite.h"
#include "../button.h"
#include "../sound_manager.h"
#include "../texture_manager.h"
#include "../system.h"
#include "../loading/loading_state.h"
#include "../user/character.h"

fr::MainState *fr::MainState::m_instance = 0;

void fr::MainState::init()
{
	main_start = new Button;
	main_about = new Button;
	background = new Sprite;

	main_start->init("assets/main/button.png", 0, 0);
	main_start->AddPressedFrame("assets/main/button_pressed.png");
	main_about->init("assets/main/button.png", 0, 0);
	main_about->AddPressedFrame("assets/main/button_pressed.png");
	main_start->SetPos(System::instance()->GetWindowWidth() / 2 - main_start->GetW() / 2, System::instance()->GetWindowHeigh() - 480);
	main_about->SetPos(System::instance()->GetWindowWidth() / 2 - main_about->GetW() / 2, System::instance()->GetWindowHeigh() - 280);
	TextureManager::instance()->loadfont("assets/fonts/Audiowide.ttf", 60);

	is_locked = false;

	main_start->AddText("Start", main_start->GetW() / 2, main_start->GetH() / 2, "assets/fonts/Audiowide.ttf", 60, 0x00, 0x00, 0x00);
	main_about->AddText("About", main_about->GetW() / 2, main_about->GetH() / 2, "assets/fonts/Audiowide.ttf", 60, 0x00, 0x00, 0x00);

//	SoundManager::instance()->load("assets/BGM.wav", SOUNDTYPE_MUSIC);
//	SoundManager::instance()->play("assets/BGM.wav", SOUNDTYPE_MUSIC);
}

void fr::MainState::clear()
{
	main_start->clear();
	main_about->clear();
	background->clear();
	TextureManager::instance()->clearfont("assets/fonts/Audiowide.ttf", 60);
}

void fr::MainState::update()
{
	if (System::instance()->IsWindowModified())
	{
		main_start->SetPos(System::instance()->GetWindowWidth() / 2 - main_start->GetW() / 2, System::instance()->GetWindowHeigh() - 480);
		main_about->SetPos(System::instance()->GetWindowWidth() / 2 - main_about->GetW() / 2, System::instance()->GetWindowHeigh() - 280);
	}

	if (!is_locked)
	{
		main_start->update();
		main_about->update();
	}
	//鎖定按鈕在打開側邊欄或彈窗時不進行檢測

//	background->render();
	Character::instance()->render();
	main_start->render();
	main_about->render();

	if (main_start->IsReleased())
	{
		LoadingState::instance()->init(STATE_PREPARE);
//		SoundManager::instance()->stop();
	}
	if (main_about->IsReleased())
	{
		LoadingState::instance()->init(STATE_CONSTRUCTING);
//		SoundManager::instance()->stop();
	}
}

void fr::MainState::lock(bool load)
{
	is_locked = load;
	//這樣鎖定和解鎖可以用在同一個函數。
}