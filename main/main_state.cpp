#include "main_state.h"
#include "../sprite.h"
#include "../gui/button.h"
#include "../gui/text_area.h"
#include "../sound_manager.h"
#include "../texture_manager.h"
#include "../gui/text_input_box.h"
#include "../system.h"
#include "../user/setting.h"
#include "../loading/loading_state.h"
#include "../background.h"

fr::MainState *fr::MainState::m_instance = 0;

void fr::MainState::init()
{
	main_about = new Button;
	widget_base = new Sprite;
	performance_process_bar = new Sprite;
	logo = new Sprite;
	user_name_text = new TextInputBox;
	performance_point_text = new TextArea;
	start_text = new TextArea;

	main_about->init("assets/base/sort_button.png");
	main_about->AddPressedFrame("assets/base/sort_button_pressed.png");
	widget_base->init("assets/base/widget_min_base.png");
	performance_process_bar->init("assets/base/process_bar.png", Rect(32, 94, 0, 0));
	main_about->SetPos(0, System::instance()->GetWindowHeigh() - main_about->GetH());
	logo->init("assets/logo.png", Rect(0, 0, 560, 560));
	logo->SetPos(System::instance()->GetWindowWidth() / 2 - logo->GetW() / 2, System::instance()->GetWindowHeigh() / 2 - logo->GetH() / 2);

	char *performance_point_ch = new char[50];
	sprintf(performance_point_ch, "Performance:%d", Setting::instance()->GetPerformancePoint());
	user_name_text->init("", Rect(32, 32, 216, 32));
	user_name_text->InitText(0, 0, "assets/fonts/Audiowide.ttf", 32, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 216);
	user_name_text->SetText(Setting::instance()->GetUserName());
	performance_point_text->init(performance_point_ch, Point2Di(32, 120), "assets/fonts/Audiowide.ttf", 18, Color(0x00, 0x00, 0x00), TEXTFORMAT_LEFT);
	start_text->init("Touch anywhere to activiate", Point2Di(System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() - 180), "assets/fonts/Audiowide.ttf", 24, Color(0x00, 0x00, 0x00));
	delete [] performance_point_ch;

	is_locked = false;

	main_about->AddText("About", Point2Di(main_about->GetW() / 2, main_about->GetH() / 2), "assets/fonts/Audiowide.ttf", 32, Color(0x00, 0x00, 0x00));

	performance_process_bar->SetSrcRect(Rect(0, 0, (float(Setting::instance()->GetPerformancePoint()) / 10000.f) * 216.f, 8));
	performance_process_bar->SetSize((Setting::instance()->GetPerformancePoint() < 10000.f ? float(Setting::instance()->GetPerformancePoint()) / 10000.f : 1) * 216.f, 8);

//	SoundManager::instance()->load("assets/BGM.wav", SOUNDTYPE_MUSIC);
//	SoundManager::instance()->play("assets/BGM.wav", SOUNDTYPE_MUSIC);
}

void fr::MainState::clear()
{
	main_about->clear();
	widget_base->clear();
	performance_process_bar->clear();
	logo->clear();
	user_name_text->clear();
	performance_point_text->clear();
	start_text->clear();
	delete main_about;
	delete widget_base;
	delete performance_process_bar;
	delete logo;
	delete user_name_text;
	delete performance_point_text;
	delete start_text;
}

void fr::MainState::update()
{
	if (System::instance()->IsWindowModified())
	{
		widget_base->SetPos(0, 0);
		main_about->SetPos(0, System::instance()->GetWindowHeigh() - main_about->GetH());
		start_text->SetPos(System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() - 180);
		user_name_text->SetPos(32, 32);
		performance_point_text->SetPos(32, 120);
		performance_process_bar->SetPos(32, 94);
		logo->SetPos(System::instance()->GetWindowWidth() / 2 - logo->GetW() / 2, System::instance()->GetWindowHeigh() / 2 - logo->GetH() / 2);
	}

	if (!is_locked)
	{
		main_about->update();
		user_name_text->update();
	}
	//鎖定按鈕在打開側邊欄或彈窗時不進行檢測

//	background->render();
	Background::instance()->render();
	main_about->render();
	start_text->render();
	logo->render();
	widget_base->render();
	user_name_text->render();
	performance_point_text->render();
	performance_process_bar->render();

	if (main_about->IsOutsidePressed() && user_name_text->IsOutsidePressed())
	{
		LoadingState::instance()->init(STATE_PREPARE);
//		SoundManager::instance()->stop();
	}
	if (main_about->IsReleased())
	{
		LoadingState::instance()->init(STATE_ABOUT);
//		SoundManager::instance()->stop();
	}

//	Setting::instance()->SetUserName(user_name_text->GetText());
}

void fr::MainState::lock(bool load)
{
	is_locked = load;
	//這樣鎖定和解鎖可以用在同一個函數。
}