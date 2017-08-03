#include "debug_widget.h"

lm::DebugWidget *lm::DebugWidget::m_instance = 0;

void lm::DebugWidget::init()
{
	background = new Sprite;
	m_w = 528;
	m_h = 316;
	background->load("assets/window_base.png", 0, 0, m_w, m_h);
	font_size = 16;
	row_count = (m_w - 60) / font_size;
	//限制顯示行數，我怎麼知道這是什麼操作啊！
	v_x = 0;
	v_y = 0;
	TextureManager::instance()->loadfont("assets/Kazesawa-Regular.ttf", font_size);
	m_x = System::instance()->GetWindowWidth() - 528;
	m_y = System::instance()->GetWindowHeigh() - 316;
	//將窗口放到右下角。
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
	//開關
		for (int i = 0; i < ControlHandler::instance()->GetFingerCount(); i++)
		{
			Finger load_finger = ControlHandler::instance()->GetFinger(i);
			//輸入檢測(僅支持觸控)
			if (load_finger.x >= m_x && load_finger.y >= m_y && load_finger.x <= (m_x + m_w) && load_finger.y <= (m_y + m_h))
			{
				m_x += load_finger.dx;
				m_y += load_finger.dy;
				//使用位移來改變坐標，這樣比較方便一些
				v_x = load_finger.dx;
				v_y = load_finger.dy;
				//記錄速度以實現滑動。

				if (m_x > System::instance()->GetWindowWidth() - m_w)
				{
					m_x = System::instance()->GetWindowWidth() - m_w;
				}
				else if (m_x < 0)
				{
					m_x = 0;
				}
				if (m_y > System::instance()->GetWindowHeigh() - m_h)
				{
					m_y = System::instance()->GetWindowHeigh() - m_h;
				}
				else if (m_y < 0)
				{
					m_y = 0;
				}
				//防止窗口劃出屏幕
			}
		}	//for (int i = 0; i < ControlHandler::instance()->GetFingerCount(); i++)
		if (ControlHandler::instance()->GetFingerCount() == 0)
		{
			if (v_x < 0)
			{
				v_x += 1;
				//假裝有摩擦力阻止運動，這種實現方法不太好，應該用矢量
				v_x = v_x > 0 ? 0 : v_x;
			}
			else if (v_x > 0)
			{
				v_x -= 1;
				v_x = v_x < 0 ? 0 : v_x;
			}
			if (v_y < 0)
			{
				v_y += 1;
				v_y = v_y > 0 ? 0 : v_y;
			}
			else if (v_y > 0)
			{
				v_y -= 1;
				v_y = v_y < 0 ? 0 : v_y;
			}
			m_x += v_x;
			m_y += v_y;
			//假裝利用慣性運動
			if (m_x > System::instance()->GetWindowWidth() - m_w)
			{
				m_x = System::instance()->GetWindowWidth() - m_w;
				v_x = -v_x;
			}
			else if (m_x < 0)
			{
				m_x = 0;
				v_x = -v_x;
			}
			if (m_y > System::instance()->GetWindowHeigh() - m_h)
			{
				m_y = System::instance()->GetWindowHeigh() - m_h;
				v_y = -v_y;
			}
			else if (m_y < 0)
			{
				m_y = 0;
				v_y = -v_y;
			}
			//窗口滑動時碰到邊緣就反彈，就當是實現了碰撞檢測了(大霧)
		}	//if (ControlHandler::instance()->GetFingerCount() == 0)
		background->SetPos(m_x, m_y);
		//render
		background->render();
		//先渲染背景
		for (int i = 0; i < text.size(); i++)
		{
			int x = m_x + 16;
			int y = m_y + 6 + (font_size + 2) * i;
			TextureManager::instance()->render(text[i], x, y, "assets/Kazesawa-Regular.ttf", 0xFF, 0xFF, 0xFF, TEXTFORMAT_LEFT);
		}
		//文字渲染
	}	//if (is_showing)
}	//void lm::DebugWidget::update()

void lm::DebugWidget::PushLog(std::string load_log)
{
	if (text.size() >= row_count)
	{
		text.erase(text.begin());
	}
	//超過行數的話就把開頭去掉，如果要搞得像黑匣子一樣的輸出文件的話，這段代碼要改改了
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
	//調試信息輸出
}

bool lm::DebugWidget::IsShowing()
{
	return is_showing;
}