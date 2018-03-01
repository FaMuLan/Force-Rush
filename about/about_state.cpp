#include "about_state.h"
#include "../button.h"
#include "../sprite.h"
#include "../texture_manager.h"
#include "../system.h"
#include "../loading/loading_state.h"
#include "../text_area.h"
#include "../background.h"

fr::AboutState *fr::AboutState::m_instance = 0;

void fr::AboutState::init()
{
	back = new Button;
	text = new TextArea;
	back->init("assets/base/sort_button.png");
	back->AddPressedFrame( "assets/base/sort_button_pressed.png");
	back->AddText("Back", back->GetW() / 2, back->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
	std::string str =
		"程序: FaMuLan\n"
		"版本: beta 1.2\n";
	text->init(str, System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2, "assets/fonts/Miui-Bold.ttf", 30, 0x00, 0x00, 0x00, TEXTFORMAT_MIDDLE, 560, true);
}

void fr::AboutState::clear()
{
	
}

void fr::AboutState::update()
{
	if (System::instance()->IsWindowModified())
	{
		text->SetPos(System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2);
	}

	back->update();

	Background::instance()->render();
	text->render();
	back->render();
	if (back->IsReleased())
	{
		LoadingState::instance()->init(STATE_MAIN);
	}
}