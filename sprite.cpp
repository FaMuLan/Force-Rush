#include "sprite.h"

void lm::Sprite::load(std::string path, int x, int y, int w, int h)
{
	TextureManager::instance()->load(path);
	m_path = path;
	m_x = x;
	m_y = y;
	m_w = w;
	m_h = h;
}

void lm::Sprite::update()
{
	//do nothing	
}

void lm::Sprite::render()
{
	TextureManager::instance()->render(m_path, m_x, m_y, m_w, m_h);
}

void lm::Sprite::clear()
{
	TextureManager::instance()->clear(m_path);
}

void lm::Sprite::SetPos(int x, int y)
{
	m_x = x;
	m_y = y;
}
