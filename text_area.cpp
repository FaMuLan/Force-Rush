#include "text_area.h"
#include <SDL2/SDL.h>
#include "texture_manager.h"

void lm::TextArea::init(std::string text, int x, int y, std::string font_path, int font_size, char r, char g, char b, TextFormat format, int limited_w)
{
	m_text = text;
	m_x = x;
	m_y = y;
	m_font_path = font_path;
	m_font_size = font_size;
	m_r = r;
	m_g = g;
	m_b = b;
	m_format = format;
	m_limited_w = limited_w;
}

void lm::TextArea::render()
{
	TextureManager::instance()->render(m_text, m_x, m_y, m_font_path, m_font_size, m_r, m_g, m_b, m_format, m_limited_w);
}

void lm::TextArea::render(int x, int y)
{
	TextureManager::instance()->render(m_text, x, y, m_font_path, m_font_size, m_r, m_g, m_b, m_format, m_limited_w);
}

void lm::TextArea::clear()
{
	
}

void lm::TextArea::SetPos(int x, int y)
{
	m_x = x;
	m_y = y;
}

void lm::TextArea::SetText(std::string text)
{
	m_text = text;
}

int lm::TextArea::GetX()
{
	return m_x;
}

int lm::TextArea::GetY()
{
	return m_y;
}

std::string lm::TextArea::GetText()
{
	return m_text;
}