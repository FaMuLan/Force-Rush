#include "text_area.h"
#include <SDL2/SDL.h>
#include "texture_manager.h"

void lm::TextArea::load(std::string text, std::string font_path, int font_size, int r, int g, int b)
{
	m_text = text;
	m_font_path = font_path;
	m_font_size = font_size;
	m_r = r;
	m_g = g;
	m_b = b;
}

void lm::TextArea::render()
{
	TextureManager::instance()->render(m_text, m_x, m_y, m_font_path, m_font_size, m_r, m_g, m_b);
}

void lm::TextArea::render(int x, int y)
{
	TextureManager::instance()->render(m_text, x, y, m_font_path, m_font_size, m_r, m_g, m_b);
}

void lm::TextArea::clear()
{
	
}

void lm::TextArea::SetPos(int x, int y)
{
	m_x = x;
	m_y = y;
}

int lm::TextArea::GetX()
{
	return m_x;
}

int lm::TextArea::GetY()
{
	return m_y;
}