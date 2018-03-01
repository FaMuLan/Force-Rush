#include "main_state.h"
#include "../sprite.h"
#include "../button.h"
#include "../text_area.h"
#include "../sound_manager.h"
#include "../texture_manager.h"
#include "../text_input_box.h"
#include "../system.h"
#include "../loading/loading_state.h"
#include "../user/user_profile.h"
#include "../background.h"

fr::MainState *fr::MainState::m_instance = 0;

void fr::MainState::init()
{
	main_start = new Button;
	main_about = new Button;
	widget_base = new Sprite;
	performance_process_bar = new Sprite;
	user_name_text = new TextInputBox;
	performance_point_text = new TextArea;

	main_start->init("assets/main/button.png");
	main_start->AddPressedFrame("assets/main/button_pressed.png");
	main_about->init("assets/main/button.png");
	main_about->AddPressedFrame("assets/main/button_pressed.png");
	widget_base->init("assets/base/widget_min_base.png");
	performance_process_bar->init("assets/base/process_bar.png", Rect(32, 94, 0, 0));
	main_start->SetPos(System::instance()->GetWindowWidth() / 2 - main_start->GetW() / 2, System::instance()->GetWindowHeigh() - 480);
	main_about->SetPos(System::instance()->GetWindowWidth() / 2 - main_about->GetW() / 2, System::instance()->GetWindowHeigh() - 280);

	char *performance_point_ch = new char[50];
	sprintf(performance_point_ch, "Performance:%d", UserProfile::instance()->GetPerformancePoint());
	user_name_text->init("", Rect(32, 32, 216, 32));
	user_name_text->InitText(0, 0, "assets/fonts/Audiowide.ttf", 32, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 216);
	user_name_text->SetText(UserProfile::instance()->GetUserName());
	performance_point_text->init(performance_point_ch, 32, 120, "assets/fonts/Audiowide.ttf", 18, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT);
	delete [] performance_point_ch;

	is_locked = false;

	main_start->AddText("Start", main_start->GetW() / 2, main_start->GetH() / 2, "assets/fonts/Audiowide.ttf", 60, 0x00, 0x00, 0x00);
	main_about->AddText("About", main_about->GetW() / 2, main_about->GetH() / 2, "assets/fonts/Audiowide.ttf", 60, 0x00, 0x00, 0x00);

	performance_process_bar->SetSrcRect(Rect(0, 0, (float(UserProfile::instance()->GetPerformancePoint()) / 10000.f) * 216.f, 8));
	performance_process_bar->SetSize((UserProfile::instance()->GetPerformancePoint() < 10000.f ? float(UserProfile::instance()->GetPerformancePoint()) / 10000.f : 1) * 216.f, 8);

//	SoundManager::instance()->load("assets/BGM.wav", SOUNDTYPE_MUSIC);
//	SoundManager::instance()->play("assets/BGM.wav", SOUNDTYPE_MUSIC);
}

void fr::MainState::clear()
{
	main_start->clear();
	main_about->clear();
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
		user_name_text->update();
	}
	//鎖定按鈕在打開側邊欄或彈窗時不進行檢測

//	background->render();
	Background::instance()->render();
	main_start->render();
	main_about->render();
	widget_base->render();
	user_name_text->render();
	performance_point_text->render();
	performance_process_bar->render();

	if (main_start->IsReleased())
	{
		LoadingState::instance()->init(STATE_PREPARE);
//		SoundManager::instance()->stop();
	}
	if (main_about->IsReleased())
	{
		LoadingState::instance()->init(STATE_ABOUT);
//		SoundManager::instance()->stop();
	}

	UserProfile::instance()->SetUserName(user_name_text->GetText());
}

void fr::MainState::lock(bool load)
{
	is_locked = load;
	//這樣鎖定和解鎖可以用在同一個函數。
}