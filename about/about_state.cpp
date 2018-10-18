#include "about_state.h"
#include "../gui/button.h"
#include "../sprite.h"
#include "../texture_manager.h"
#include "../system.h"
#include "../loading/loading_state.h"
#include "../gui/text_area.h"
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
		"版本: beta 1.3.5\n"
		"特别致谢：\n"
		"{\n"
		"\t直接参与协助：\n"
		"\t{\n"
		"\t\tSSsea+\n"
		"\t\tCytokinesis\n"
		"\t\tTeradora\n"
		"\t\tluyzi\n"
		"\t\tMTF LHZS\n"
		"\t}\n"
		"\t精神股东：\n"
		"\t{\n"
		"\t\tHDRoop\n"
		"\t\tQAIU\n"
		"\t\tcr01x\n"
		"\t\tG-Orochi\n"
		"\t\t隐月\n"
		"\t\t小小苏\n"
		"\t}\n"
		"\t还有那个一直在关注我成长的你\n"
		"}\n";
	text->init(str, System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2, "assets/fonts/Miui-Bold.ttf", 30, 0x00, 0x00, 0x00, TEXTFORMAT_MIDDLE, 560);
}

void fr::AboutState::clear()
{
	text->clear();
	back->clear();
	delete text;
	delete back;
}

void fr::AboutState::update()
{
	if (System::instance()->IsWindowModified())
	{
		text->SetPos(System::instance()->GetWindowWidth() / 2, System::instance()->GetWindowHeigh() / 2);
		back->SetPos(0, 0);
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