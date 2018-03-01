#include "background.h"
#include "sprite.h"
#include "system.h"
#include "timer.h"

fr::Background *fr::Background::m_instance = 0;

void fr::Background::init()
{
	background_arrow_portrait = new Sprite;
	background_arrow_landscape = new Sprite;
	background_arrow_portrait->init("assets/base/background_arrow_portrait.png");
	background_arrow_landscape->init("assets/base/background_arrow_landscape.png");
}

void fr::Background::clear()
{
	background_arrow_portrait->clear();
	background_arrow_landscape->clear();
}

void fr::Background::render()
{
	float process = float(Timer::instance()->GetSystemTime() % 3000) / 3000.f;
	if (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT)
	{
		int x = System::instance()->GetWindowWidth() / 2 - background_arrow_portrait->GetW() / 2;
		int y = -background_arrow_portrait->GetH() + 420 * process;
		while (y < System::instance()->GetWindowHeigh())
		{
			background_arrow_portrait->SetPos(x, y);
			background_arrow_portrait->render();
			y += 420;
		}
	}
	else
	{
		int x = -background_arrow_landscape->GetW() + 420 * process;
		int y = System::instance()->GetWindowHeigh() / 2 - background_arrow_landscape->GetH() / 2;
		while (x < System::instance()->GetWindowWidth())
		{
			background_arrow_landscape->SetPos(x, y);
			background_arrow_landscape->render();
			x += 420;
		}
	}
}