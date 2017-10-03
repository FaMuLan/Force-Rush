#include "sprite.h"
#include "texture_manager.h"

void lm::Sprite::load(std::string path, int x, int y, int w, int h)
{
	TextureManager::instance()->load(path, m_w, m_h);
	m_path = path;
	m_x = x;
	m_y = y;
	if (w != 0 || h != 0)
	{
		m_w = w;
		m_h = h;
	}
	scale = 1;
}

void lm::Sprite::update()
{
	//do nothing
}

void lm::Sprite::render()
{
	TextureManager::instance()->render(m_path, m_x, m_y, m_w * scale, m_h * scale);
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

void lm::Sprite::SetScale(float load_scale)
{
	scale = load_scale;
}

int lm::Sprite::GetX()
{
	return m_x;
}

int lm::Sprite::GetY()
{
	return m_y;
}