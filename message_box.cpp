#include "message_box.h"
#include "texture_manager.h"
#include "sprite.h"
#include "timer.h"

fr::MessageBox *fr::MessageBox::m_instance = 0;

void fr::MessageBox::init()
{
	base = new Sprite;
	base->init("assets/base/message_box_base.png");
	TextureManager::instance()->loadfont("assets/fonts/Ubuntu-R.ttf", 48);
	text_base = "none text";
	is_base_text_showing = true;
}

void fr::MessageBox::clear()
{
	
}

void fr::MessageBox::update()
{
/*
	if (!is_base_text_showing)
	{
		if (Timer::instance()->GetTime("message_box_top_showing") > top_show_duration)
		{
			Timer::instance()->ResetTimer("message_box_top_showing");
			top_show_duration = 0;
			is_base_text_showing = true;
		}
	}
*/
	static int last_time = 0;
	int current_time = Timer::instance()->GetSystemTime();
	int fps = double(1000) / double(current_time - last_time);
	char *fps_ch = new char[8];
	sprintf(fps_ch, "%d", fps);
	text_base = fps_ch;
	last_time = current_time;
}

void fr::MessageBox::render()
{
	base->render();
	if (is_base_text_showing)
	{
		TextureManager::instance()->render(text_base, 360, 40, "assets/fonts/Ubuntu-R.ttf", 48, 0x00, 0x00, 0x00);
	}
	else
	{
		TextureManager::instance()->render(text_top, 360, 40, "assets/fonts/Ubuntu-R.ttf", 48, 0x00, 0x00, 0x00);
	}
}

void fr::MessageBox::SetText(std::string text, int time)
{
	/*
	if (time != 0)
	{
		is_base_text_showing = false;
	Timer::instance()->RunTimer("message_box_top_showing");
		text_top = text;
	}
	else
	{
		text_base = text;
	}
	*/
}