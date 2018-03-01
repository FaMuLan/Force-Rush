#include "text_area.h"
#include <SDL2/SDL.h>
#include "texture_manager.h"

void fr::TextArea::init(std::string text, int x, int y, std::string font_path, int font_size, char r, char g, char b, TextFormat format, int limited_w, bool wrapped)
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
	m_wrapped = wrapped;
	m_scale = 1;
	TextureManager::instance()->loadfont(font_path, font_size);
	if (m_text != "")
	{
		cache = TextureManager::instance()->CacheText(m_text, m_font_path, m_font_size, m_r, m_g, m_b, m_limited_w, m_wrapped, m_scale);
	}
}

void fr::TextArea::render()
{
	int x = m_x;
	int y = m_y;
	switch (m_format)
	{
		case TEXTFORMAT_MIDDLE:
			x -= cache->w / 2;
			y -= cache->h / 2;
		break;
		case TEXTFORMAT_RIGHT:
			x -= cache->w;
		break;
	}
	if (m_text != "")
	{
		TextureManager::instance()->render(cache->texture, Rect(x, y, cache->w, cache->h));
	}
}

void fr::TextArea::render(int x, int y)
{
	switch (m_format)
	{
		case TEXTFORMAT_MIDDLE:
			x -= cache->w / 2;
			y -= cache->h / 2;
		break;
		case TEXTFORMAT_RIGHT:
			x -= cache->w;
		break;
	}
	if (m_text != "")
	{
		TextureManager::instance()->render(cache->texture, Rect(x, y, cache->w, cache->h));
	}
}

void fr::TextArea::clear()
{
	
}

void fr::TextArea::SetPos(int x, int y)
{
	m_x = x;
	m_y = y;
}

void fr::TextArea::SetText(std::string text)
{
	if (m_text != text)
	{
		if (m_text != "")
		{
			SDL_DestroyTexture(cache->texture);
			delete cache;
		}
		m_text = text;
		if (m_text != "")
		{
			cache = TextureManager::instance()->CacheText(m_text, m_font_path, m_font_size, m_r, m_g, m_b, m_limited_w, m_wrapped, m_scale);
		}
	}
}

void fr::TextArea::SetColor(char r, char g, char b)
{
	m_r = r;
	m_g = g;
	m_b = b;
	if (m_text != "")
	{
		SDL_DestroyTexture(cache->texture);
		delete cache;
		cache = TextureManager::instance()->CacheText(m_text, m_font_path, m_font_size, m_r, m_g, m_b, m_limited_w, m_wrapped, m_scale);
	}
}

void fr::TextArea::SetFont(std::string font_path, int font_size)
{
	m_font_path = font_path;
	m_font_size = font_size;
	if (m_text != "")
	{
		SDL_DestroyTexture(cache->texture);
		delete cache;
		cache = TextureManager::instance()->CacheText(m_text, m_font_path, m_font_size, m_r, m_g, m_b, m_limited_w, m_wrapped, m_scale);
	}
}

void fr::TextArea::SetScale(float scale)
{
	m_scale = scale;
	if (m_text != "")
	{
		SDL_DestroyTexture(cache->texture);
		delete cache;
		cache = TextureManager::instance()->CacheText(m_text, m_font_path, m_font_size, m_r, m_g, m_b, m_limited_w, m_wrapped, m_scale);
	}
}

int fr::TextArea::GetX()
{
	return m_x;
}

int fr::TextArea::GetY()
{
	return m_y;
}

std::string fr::TextArea::GetText()
{
	return m_text;
}

float fr::TextArea::GetScale()
{
	return m_scale;
}