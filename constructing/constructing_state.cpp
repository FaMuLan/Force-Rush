#include "constructing_state.h"
#include "../button.h"
#include "../sprite.h"
#include "../texture_manager.h"
#include "../system.h"
#include "../main/main_state.h"
#include "../loading/loading_state.h"
#include "../text_area.h"

lm::ConstructingState *lm::ConstructingState::m_instance = 0;

void lm::ConstructingState::init()
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
	dialog_user[1]->SetText("不是開發者就不要點啦！");
	dialog_system[2]->SetText("日你媽也你怎麼還點！");
	dialog_user[2]->SetText("我喜歡點就點吶要妳管！");
	dialog_system[3]->SetText("emmmmmmmmmm所以你真的是那位開發者？");
	dialog_user[3]->SetText("不然你以為是誰啊……");
	dialog_system[4]->SetText("你放屁！這個遊戲的開發者只有一個人！");
	dialog_user[4]->SetText("管你那麼多我就是那個人啊！");
	dialog_system[5]->SetText("那……你在這遊戲裡面鳥一首歌吼不吼啊？");
	dialog_user[5]->SetText("這遊戲都沒寫完鳥個屁啊！");
	dialog_system[6]->SetText("嗚……有這樣罵開發者的麼，你不是開發者！");
	dialog_user[6]->SetText("誒誒誒誒誒上一句話當我沒講……");
	dialog_system[7]->SetText("額……那請女裝，開發者看起來挺喜歡女裝的");
	dialog_user[7]->SetText("女就女唄反正你也看不到");
	dialog_system[8]->SetText("哎呀呀？那這裡不能鳥歌malody總能鳥吧！");
	dialog_user[8]->SetText("那我開自動鳥歌");
	dialog_system[9]->SetText("……好吧就算你是開發者也得知道這裡沒做完x");
	dialog_user[9]->SetText("翹你媽！");
//================= End =========================
	b_back->init("assets/base/sort_button.png", 0, 0);
	b_back->AddPressedFrame( "assets/base/sort_button_pressed.png");
	b_back->AddText("Back", b_back->GetW() / 2, b_back->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
	b_communicate->init("", 0, System::instance()->GetWindowHeigh() - 100, System::instance()->GetWindowWidth(), 100);
	b_communicate->AddPressedFrame("");
		b_communicate->AddText(dialog_user[text_index]->GetText(), b_communicate->GetW() / 2, b_communicate->GetH() / 2, "assets/fonts/Miui-Light.ttf", 20, 0x00, 0x00, 0x00);
}

void lm::ConstructingState::clear()
{
	
}

void lm::ConstructingState::update()
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