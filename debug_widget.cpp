#include "debug_widget.h"

lm::DebugWidget *lm::DebugWidget::m_instance = 0;

void lm::DebugWidget::init()
{
	background = new Sprite;
	background->load("assets/window_base.png", 0, 0, 528, 316);
	font_size = 16;
	row_count = 296 / font_size;
	// 296 = background->m_h - 20
	TextureManager::instance()->loadfont("assets/Kazesawa-Regular.ttf", 16);
	m_x = System::instance()->GetWindowWidth() - 528;
	m_y = System::instance()->GetWindowHeigh() - 316;
	is_showing = false;
	background->SetPos(m_x, m_y);
}

void lm::DebugWidget::clear()
{
	
}

void lm::DebugWidget::update()
{
	if (is_showing)
	{
		background->render();
		for (int i = 0; i < text.size(); i++)
		{
			int x = m_x + 16;
			int y = m_y + 10 + (font_size + 2) * i;
			TextureManager::instance()->render(text[i], x, y, "assets/Kazesawa-Regular.ttf", 0xFF, 0xFF, 0xFF, TEXTFORMAT_LEFT);
		}
	}
}

void lm::DebugWidget::PushLog(std::string load_log)
{
	if (text.size() >= row_count)
	{
		text.erase(text.begin());
	}
	text.push_back(load_log);
}

void lm::DebugWidget::ShowWidget(bool load)
{
	is_showing = load;
	if (load)
	{
		PushLog("Debug widget on");
	}
	else
	{
		PushLog("Debug widget off");
	}
}

bool lm::DebugWidget::IsShowing()
{
	return is_showing;
}