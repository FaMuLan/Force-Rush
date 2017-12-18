#include "character.h"
#include "../sprite.h"
#include "../system.h"

fr::Character *fr::Character::m_instance = 0;

void fr::Character::init()
{
	m_char = new Sprite;
	m_char->init("assets/base/char.png", System::instance()->GetWindowWidth() / 2 - 480, 0, 960, 1280);
}

void fr::Character::clear()
{
	m_char->clear();
}

void fr::Character::render()
{
	if (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT)
	{
		m_char->SetPos(System::instance()->GetWindowWidth() / 2 - 480, 0);
		m_char->SetScale(1);
	}
	else
	{
		m_char->SetPos(System::instance()->GetWindowWidth() / 2 - 280, 0);
		m_char->SetScale(0.583333);
	}
	m_char->render();
}