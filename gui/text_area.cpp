#include "text_area.h"
#include <SDL2/SDL.h>
#include "../texture_manager.h"
#include "../system.h"

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
	TextureManager::instance()->LoadFont(font_path, font_size);
	matrix_id = "default";
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

		int formated_x = m_x;
		int formated_y = m_y;
		switch (m_format)
		{
			case TEXTFORMAT_MIDDLE:
				formated_x -= ((m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w) / 2;
				formated_y -= cache->h / 2;
			break;
			case TEXTFORMAT_RIGHT:
				formated_x -= (m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w;
			break;
		}

		vectrices[0] = (float(formated_x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
		vectrices[1] = (1.f - float(formated_y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		//top left
		vectrices[6] = (float(formated_x + ((m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w) * m_scale) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
		vectrices[7] = (1.f - float(formated_y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		//top right
		vectrices[12] = (float(formated_x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
		vectrices[13] = (1.f - float(formated_y + cache->h * m_scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		//bottom left
		vectrices[18] = float(formated_x + ((m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w) * m_scale) / float(System::instance()->GetWindowWidth()) * 2.f - 1.f;
		vectrices[19] = (1.f - float(formated_y + cache->h * m_scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
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

void fr::TextArea::render(int x, int y)
{
	if (m_text != "")
	{
		switch (m_format)
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
		temp_vectrices[6] = (float(x + ((m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w) * m_scale) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
		temp_vectrices[7] = (1.f - float(y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		//top right
		temp_vectrices[12] = (float(x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
		temp_vectrices[13] = (1.f - float(y + cache->h * m_scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		//bottom left
		temp_vectrices[18] = float(x + ((m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w) * m_scale) / float(System::instance()->GetWindowWidth()) * 2.f - 1.f;
		temp_vectrices[19] = (1.f - float(y + cache->h * m_scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		//bottom right
		TextureManager::instance()->render(cache->texture, temp_vectrices, matrix_id);
	}
}

void fr::TextArea::clear()
{
	
}

void fr::TextArea::SetPos(int x, int y)
{
	m_x = x;
	m_y = y;

	if (cache)
	{
		int formated_x = m_x;
		int formated_y = m_y;
		switch (m_format)
		{
			case TEXTFORMAT_MIDDLE:
				formated_x -= ((m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w) / 2;
				formated_y -= cache->h / 2;
			break;
			case TEXTFORMAT_RIGHT:
				formated_x -= (m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w;
			break;
		}

		vectrices[0] = (float(formated_x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
		vectrices[1] = (1.f - float(formated_y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		//top left
		vectrices[6] = (float(formated_x + ((m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w) * m_scale) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
		vectrices[7] = (1.f - float(formated_y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		//top right
		vectrices[12] = (float(formated_x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
		vectrices[13] = (1.f - float(formated_y + cache->h * m_scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		//bottom left
		vectrices[18] = float(formated_x + ((m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w) * m_scale) / float(System::instance()->GetWindowWidth()) * 2.f - 1.f;
		vectrices[19] = (1.f - float(formated_y + cache->h * m_scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		//bottom right
	}
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
			int formated_x = m_x;
			int formated_y = m_y;
			switch (m_format)
			{
				case TEXTFORMAT_MIDDLE:
					formated_x -= ((m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w) / 2;
					formated_y -= cache->h / 2;
				break;
				case TEXTFORMAT_RIGHT:
					formated_x -= (m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w;
				break;
			}

			vectrices[0] = (float(formated_x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
			vectrices[1] = (1.f - float(formated_y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
			//top left
			vectrices[6] = (float(formated_x + ((m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w) * m_scale) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
			vectrices[7] = (1.f - float(formated_y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
			//top right
			vectrices[12] = (float(formated_x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
			vectrices[13] = (1.f - float(formated_y + cache->h * m_scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
			//bottom left
			vectrices[18] = float(formated_x + ((m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w) * m_scale) / float(System::instance()->GetWindowWidth()) * 2.f - 1.f;
			vectrices[19] = (1.f - float(formated_y + cache->h * m_scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
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
		int formated_x = m_x;
		int formated_y = m_y;
		switch (m_format)
		{
			case TEXTFORMAT_MIDDLE:
				formated_x -= ((m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w) / 2;
				formated_y -= cache->h / 2;
			break;
			case TEXTFORMAT_RIGHT:
				formated_x -= (m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w;
			break;
		}

		vectrices[0] = (float(formated_x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
		vectrices[1] = (1.f - float(formated_y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		//top left
		vectrices[6] = (float(formated_x + ((m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w) * m_scale) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
		vectrices[7] = (1.f - float(formated_y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		//top right
		vectrices[12] = (float(formated_x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
		vectrices[13] = (1.f - float(formated_y + cache->h * m_scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		//bottom left
		vectrices[18] = float(formated_x + ((m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w) * m_scale) / float(System::instance()->GetWindowWidth()) * 2.f - 1.f;
		vectrices[19] = (1.f - float(formated_y + cache->h * m_scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
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
		int formated_x = m_x;
		int formated_y = m_y;
		switch (m_format)
		{
			case TEXTFORMAT_MIDDLE:
				formated_x -= ((m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w) / 2;
				formated_y -= cache->h / 2;
			break;
			case TEXTFORMAT_RIGHT:
				formated_x -= (m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w;
			break;
		}

		vectrices[0] = (float(formated_x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
		vectrices[1] = (1.f - float(formated_y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		//top left
		vectrices[6] = (float(formated_x + ((m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w) * m_scale) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
		vectrices[7] = (1.f - float(formated_y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		//top right
		vectrices[12] = (float(formated_x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
		vectrices[13] = (1.f - float(formated_y + cache->h * m_scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		//bottom left
		vectrices[18] = float(formated_x + ((m_limited_w > cache->w || m_limited_w == 0) ? cache->w : m_limited_w) * m_scale) / float(System::instance()->GetWindowWidth()) * 2.f - 1.f;
		vectrices[19] = (1.f - float(formated_y + cache->h * m_scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		//bottom right
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