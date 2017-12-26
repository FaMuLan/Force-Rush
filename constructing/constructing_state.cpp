#include "constructing_state.h"
#include "../button.h"
#include "../sprite.h"
#include "../texture_manager.h"
#include "../system.h"
#include "../main/main_state.h"
#include "../loading/loading_state.h"
#include "../text_area.h"

fr::ConstructingState *fr::ConstructingState::m_instance = 0;

void fr::ConstructingState::init()
{
	b_back = new Button;
	b_communicate = new Button;
	text_index = 0;
	TextureManager::instance()->loadfont("assets/fonts/Audiowide.ttf", 36);
	TextureManager::instance()->loadfont("assets/fonts/Miui-Regular.ttf", 30);
	TextureManager::instance()->loadfont("assets/fonts/Miui-Light.ttf", 20);
//================= Dialog Loading ==============
	for (int i = 0; i < 10; i++)
	{
		TextArea *new_system_text = new TextArea;
		TextArea *new_user_text = new TextArea;
		new_system_text->init("", System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2, "assets/fonts/Miui-Regular.ttf", 30, 0x00, 0x00, 0x00);
		new_user_text->init("", 0, 0, "assets/fonts/Miui-Light.ttf", 20, 0x00, 0x00, 0x00);
		dialog_system.push_back(new_system_text);
		dialog_user.push_back(new_user_text);
	}
	dialog_system[0]->SetText("正在趕做，敬請期待");
	dialog_user[0]->SetText("開發者可以點這裡");
	dialog_system[1]->SetText("哦？測試遊戲的可不算哦！");
	dialog_user[1]->SetText("不是開發者就別點了x");
	dialog_system[2]->SetText("算了你都點進去了……只想說這裡沒東西");
	dialog_user[2]->SetText("你放屁！");
	dialog_system[3]->SetText("所以你要幹蛤？");
	dialog_user[3]->SetText("關於界面沒做好！");
	dialog_system[4]->SetText("好吧有些東西的確需要澄清一下");
	dialog_user[4]->SetText("啥東西？");
	dialog_system[5]->SetText("背景是從Pixiv偷來的");
	dialog_user[5]->SetText("還有呢？");
	dialog_system[6]->SetText("光效是從某個malody皮膚偷來的");
	dialog_user[6]->SetText("還有呢？");
	dialog_system[7]->SetText("……程序是FaMuLan做的，就這樣");
	dialog_user[7]->SetText("所以……");
	dialog_system[8]->SetText("沒啥別的了orz");
	dialog_user[8]->SetText("所以特別致謝不寫麼？");
	dialog_system[9]->SetText("傻孩子，致謝難道不是感謝你麼<。)#)))<");
	dialog_user[9]->SetText(">人<");
//================= End =========================
	b_back->init("assets/base/sort_button.png", 0, 0);
	b_back->AddPressedFrame( "assets/base/sort_button_pressed.png");
	b_back->AddText("Back", b_back->GetW() / 2, b_back->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
	b_communicate->init("", 0, System::instance()->GetWindowHeigh() - 100, System::instance()->GetWindowWidth(), 100);
	b_communicate->AddPressedFrame("");
		b_communicate->AddText(dialog_user[text_index]->GetText(), b_communicate->GetW() / 2, b_communicate->GetH() / 2, "assets/fonts/Miui-Light.ttf", 20, 0x00, 0x00, 0x00);
}

void fr::ConstructingState::clear()
{
	
}

void fr::ConstructingState::update()
{
	b_back->update();
	b_communicate->update();

	b_back->render();
	dialog_system[text_index]->render();
	b_communicate->render();
	
	if (b_back->IsReleased())
	{
		LoadingState::instance()->init(MainState::instance(), this);
	}
	if (b_communicate->IsReleased())
	{
		text_index++;
		if (text_index >= dialog_system.size())
		{
			text_index = 0;
		}
		b_communicate->ClearText();
		b_communicate->AddText(dialog_user[text_index]->GetText(), b_communicate->GetW() / 2, b_communicate->GetH() / 2, "assets/fonts/Miui-Light.ttf", 20, 0x00, 0x00, 0x00);
	}
}