#include "character.h"
#include "../sprite.h"
#include "../system.h"

lm::Character *lm::Character::m_instance = 0;

void lm::Character::init()
{
	m_char = new Sprite;
	m_char->load("assets/char.png", System::instance()->GetWindowWidth() / 2 - 480, 0, 960, 1280);
}

void lm::Character::clear()
{
	m_char->clear();
}

void lm::Character::render()
{
	m_char->render();
}