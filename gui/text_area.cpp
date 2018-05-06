#include "text_area.h"
#include <SDL2/SDL.h>
#include "../texture_manager.h"
#include "../system.h"

void fr::TextArea::init(std::string text, int x, int y, std::string font_path, int font_size, char r, char g, char b, TextFormat format, int limited_w, bool wrapped)
{
	m_text = text;
	dest_rect.x = x;
	dest_rect.y = y;
	m_font_path = font_path;
	m_font_size = font_size;
	m_x = x;
	m_y = y;
	m_r = r;
	m_g = g;
	m_b = b;
	m_format = format;
	m_limited_w = limited_w;
	m_wrapped = wrapped;
	m_scale = 1;
	TextureManager::instance()->LoadFont(font_path, font_size);
	matrix_id = "default";
	source_rect = Rect(0, 0, 0, 0);
	vectrices = new float[24];
	vectrices[2] = 0.f;
	vectrices[3] = 1.f;
	vectrices[4] = 0.f;
	vectrices[5] = 0.f;
	//top left
	vectrices[8] = 0.f;
	vectrices[9] = 1.f;
	vectrices[10] = 1.f;
	vectrices[11] = 0.f;
	//top right
	vectrices[14] = 0.f;
	vectrices[15] = 1.f;
	vectrices[16] = 0.f;
	vectrices[17] = 1.f;
	//bottom left
	vectrices[20] = 0.f;
	vectrices[21] = 1.f;
	vectrices[22] = 1.f;
	vectrices[23] = 1.f;
	//bottom right

	if (m_text != "")
	{
		cache = TextureManager::instance()->CacheText(m_text, m_font_path, m_font_size, m_r, m_g, m_b, m_limited_w, m_wrapped);

		switch (m_format)
		{
			case TEXTFORMAT_MIDDLE:
				dest_rect.x = m_x - ((m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w) / 2;
				dest_rect.y = m_y - cache->h / 2;
			break;
			case TEXTFORMAT_RIGHT:
				dest_rect.x = m_x - (m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w;
			break;
		}

		vectrices[0] = (float(dest_rect.x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
		vectrices[1] = (1.f - float(dest_rect.y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		//top left
		vectrices[6] = (float(dest_rect.x + ((m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w) * m_scale) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
		vectrices[7] = (1.f - float(dest_rect.y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		//top right
		vectrices[12] = (float(dest_rect.x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
		vectrices[13] = (1.f - float(dest_rect.y + cache->h * m_scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		//bottom left
		vectrices[18] = float(dest_rect.x + ((m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w) * m_scale) / float(System::instance()->GetWindowWidth()) * 2.f - 1.f;
		vectrices[19] = (1.f - float(dest_rect.y + cache->h * m_scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		source_rect.w = cache->w;
		source_rect.h = cache->h;
		dest_rect.w = cache->w;
		dest_rect.h = cache->h;
		//bottom right
	}
}

void fr::TextArea::render()
{
	if (m_text != "")
	{
		TextureManager::instance()->render(cache->texture, vectrices, matrix_id);
	}
}

void fr::TextArea::render(int x, int y, fr::TextFormat format)
{
	if (m_text != "")
	{
		format = format == TEXTFORMAT_NONE ? m_format : format;
		switch (format)
		{
			case TEXTFORMAT_MIDDLE:
				x -= ((m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w) / 2;
				y -= cache->h / 2;
			break;
			case TEXTFORMAT_RIGHT:
				x -= (m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w;
			break;
		}

		float *temp_vectrices = new float[24];
		memcpy(temp_vectrices, vectrices, 24 * sizeof(float));
		temp_vectrices[0] = (float(x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
		temp_vectrices[1] = (1.f - float(y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		//top left
		temp_vectrices[6] = (float(x + ((m_limited_w > dest_rect.w || m_limited_w == 0) ? cache->w : m_limited_w) * m_scale) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
		temp_vectrices[7] = (1.f - float(y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		//top right
		temp_vectrices[12] = (float(x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
		temp_vectrices[13] = (1.f - float(y + dest_rect.h * m_scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		//bottom left
		temp_vectrices[18] = float(x + ((m_limited_w > cache->w || m_limited_w == 0) ? dest_rect.w : m_limited_w) * m_scale) / float(System::instance()->GetWindowWidth()) * 2.f - 1.f;
		temp_vectrices[19] = (1.f - float(y + dest_rect.h * m_scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		//bottom right
		TextureManager::instance()->render(cache->texture, temp_vectrices, matrix_id);
		delete [] temp_vectrices;
	}
}

void fr::TextArea::clear()
{
	
}

void fr::TextArea::SetPos(int x, int y)
{
	if (cache)
	{
		dest_rect.x = m_x = x;
		dest_rect.y = m_y = y;
		switch (m_format)
		{
			case TEXTFORMAT_MIDDLE:
				dest_rect.x = m_x - ((m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w) / 2;
				dest_rect.y = m_y - cache->h / 2;
			break;
			case TEXTFORMAT_RIGHT:
				dest_rect.x = m_x - (m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w;
			break;
		}

		vectrices[0] = (float(dest_rect.x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
		vectrices[1] = (1.f - float(dest_rect.y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		//top left
		vectrices[6] = (float(dest_rect.x + ((m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w) * m_scale) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
		vectrices[7] = (1.f - float(dest_rect.y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		//top right
		vectrices[12] = (float(dest_rect.x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
		vectrices[13] = (1.f - float(dest_rect.y + cache->h * m_scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		//bottom left
		vectrices[18] = float(dest_rect.x + ((m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w) * m_scale) / float(System::instance()->GetWindowWidth()) * 2.f - 1.f;
		vectrices[19] = (1.f - float(dest_rect.y + cache->h * m_scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		//bottom right
	}
}

void fr::TextArea::SetSize(int w, int h)
{
	dest_rect.w = w;
	dest_rect.h = h;
	vectrices[0] = (float(dest_rect.x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
	vectrices[1] = (1.f - float(dest_rect.y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
	//top left
	vectrices[6] = (float(dest_rect.x + dest_rect.w * m_scale) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
	vectrices[7] = (1.f - float(dest_rect.y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
	//top right
	vectrices[12] = (float(dest_rect.x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
	vectrices[13] = (1.f - float(dest_rect.y + dest_rect.h * m_scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
	//bottom left
	vectrices[18] = float(dest_rect.x + dest_rect.w * m_scale) / float(System::instance()->GetWindowWidth()) * 2.f - 1.f;
	vectrices[19] = (1.f - float(dest_rect.y + dest_rect.h * m_scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
	//bottom right
}

void fr::TextArea::SetText(std::string text)
{
	if (m_text != text)
	{
		if (m_text != "")
		{
			TextureManager::instance()->DestroyCache(cache);
		}
		m_text = text;
		if (m_text != "")
		{
			cache = TextureManager::instance()->CacheText(m_text, m_font_path, m_font_size, m_r, m_g, m_b, m_limited_w, m_wrapped);
			switch (m_format)
			{
				case TEXTFORMAT_MIDDLE:
					dest_rect.x = m_x - ((m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w) / 2;
					dest_rect.y = m_y - cache->h / 2;
				break;
				case TEXTFORMAT_RIGHT:
					dest_rect.x = m_x - (m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w;
				break;
			}

			vectrices[0] = (float(dest_rect.x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
			vectrices[1] = (1.f - float(dest_rect.y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
			//top left
			vectrices[6] = (float(dest_rect.x + ((m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w) * m_scale) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
			vectrices[7] = (1.f - float(dest_rect.y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
			//top right
			vectrices[12] = (float(dest_rect.x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
			vectrices[13] = (1.f - float(dest_rect.y + cache->h * m_scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
			//bottom left
			vectrices[18] = float(dest_rect.x + ((m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w) * m_scale) / float(System::instance()->GetWindowWidth()) * 2.f - 1.f;
			vectrices[19] = (1.f - float(dest_rect.y + cache->h * m_scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
			source_rect = Rect(0, 0, cache->w, cache->h);
			dest_rect.w = cache->w;
			dest_rect.h = cache->h;
			//bottom right
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
		TextureManager::instance()->DestroyCache(cache);
		cache = TextureManager::instance()->CacheText(m_text, m_font_path, m_font_size, m_r, m_g, m_b, m_limited_w, m_wrapped);
	}
}

void fr::TextArea::SetFont(std::string font_path, int font_size)
{
	m_font_path = font_path;
	m_font_size = font_size;
	if (m_text != "")
	{
		TextureManager::instance()->DestroyCache(cache);
		cache = TextureManager::instance()->CacheText(m_text, m_font_path, m_font_size, m_r, m_g, m_b, m_limited_w, m_wrapped);
		switch (m_format)
		{
			case TEXTFORMAT_MIDDLE:
				dest_rect.x = m_x - ((m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w) / 2;
				dest_rect.y = m_y - cache->h / 2;
			break;
			case TEXTFORMAT_RIGHT:
				dest_rect.x = m_x - (m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w;
			break;
		}

		vectrices[0] = (float(dest_rect.x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
		vectrices[1] = (1.f - float(dest_rect.y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		//top left
		vectrices[6] = (float(dest_rect.x + ((m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w) * m_scale) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
		vectrices[7] = (1.f - float(dest_rect.y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		//top right
		vectrices[12] = (float(dest_rect.x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
		vectrices[13] = (1.f - float(dest_rect.y + cache->h * m_scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		//bottom left
		vectrices[18] = float(dest_rect.x + ((m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w) * m_scale) / float(System::instance()->GetWindowWidth()) * 2.f - 1.f;
		vectrices[19] = (1.f - float(dest_rect.y + cache->h * m_scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		source_rect = Rect(0, 0, cache->w, cache->h);
		//bottom right
	}
}

void fr::TextArea::SetScale(float m_scale)
{
	m_scale = m_scale;
	if (m_text != "")
	{
		TextureManager::instance()->DestroyCache(cache);
		cache = TextureManager::instance()->CacheText(m_text, m_font_path, m_font_size, m_r, m_g, m_b, m_limited_w, m_wrapped);
		switch (m_format)
		{
			case TEXTFORMAT_MIDDLE:
				dest_rect.x = m_x - ((m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w) / 2;
				dest_rect.y = m_y - cache->h / 2;
			break;
			case TEXTFORMAT_RIGHT:
				dest_rect.x = m_x - (m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w;
			break;
		}

		vectrices[0] = (float(dest_rect.x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
		vectrices[1] = (1.f - float(dest_rect.y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		//top left
		vectrices[6] = (float(dest_rect.x + ((m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w) * m_scale) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
		vectrices[7] = (1.f - float(dest_rect.y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		//top right
		vectrices[12] = (float(dest_rect.x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
		vectrices[13] = (1.f - float(dest_rect.y + cache->h * m_scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		//bottom left
		vectrices[18] = float(dest_rect.x + ((m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w) * m_scale) / float(System::instance()->GetWindowWidth()) * 2.f - 1.f;
		vectrices[19] = (1.f - float(dest_rect.y + cache->h * m_scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		source_rect = Rect(0, 0, cache->w, cache->h);
		//bottom right
	}
}

int fr::TextArea::GetX()
{
	return dest_rect.x;
}

int fr::TextArea::GetY()
{
	return dest_rect.y;
}

int fr::TextArea::GetW()
{
	return dest_rect.w;
}

int fr::TextArea::GetH()
{
	return dest_rect.h;
}

int fr::TextArea::GetUserX()
{
	return m_x;
}
int fr::TextArea::GetUserY()
{
	return m_y;
}

int fr::TextArea::GetTextureW()
{
	return cache->w;
}

int fr::TextArea::GetTextureH()
{
	return cache->h;
}

std::string fr::TextArea::GetText()
{
	return m_text;
}

float fr::TextArea::GetScale()
{
	return m_scale;
}

void fr::TextArea::SetSrcRect(fr::Rect load_source_rect)
{
	source_rect = load_source_rect;
	vectrices[4] = float(source_rect.x) / float(cache->w);
	vectrices[5] = float(source_rect.y) / float(cache->h);
	//top left
	vectrices[10] = float(source_rect.x + source_rect.w) / float(cache->w);
	vectrices[11] = float(source_rect.y) / float(cache->h);
	//top right
	vectrices[16] = float(source_rect.x) / float(cache->w);
	vectrices[17] = float(source_rect.y + source_rect.h) / float(cache->h);
	//bottom left
	vectrices[22] = float(source_rect.x + source_rect.w) / float(cache->w);
	vectrices[23] = float(source_rect.y + source_rect.h) / float(cache->h);
	//bottom right
}