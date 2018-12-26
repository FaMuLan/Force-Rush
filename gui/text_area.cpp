#include "text_area.h"
#include <SDL2/SDL.h>
#include "../texture_manager.h"
#include "../system.h"

void fr::TextArea::init(std::string input_text, fr::Point2Di input_position, std::string input_font_path, int input_font_size, Color input_color, TextFormat input_format, int input_limited_w, bool input_is_wrapped, int input_roll_speed)
{
	text = text;
	dest_rect.x = input_position.x;
	dest_rect.y = input_position.y;
	position = input_position;
	font_path = input_font_path;
	font_size = input_font_size;
	color = input_color
	format = input_format;
	limited_w = input_limited_w;
	is_wrapped = input_is_wrapped;
	roll_speed = input_roll_speed;
	scale = 1;
	TextureManager::instance()->LoadFont(font_path, font_size);
	matrix_id = "default";
	source_rect = Rect(0, 0, 0, 0);
	vectrices = new float[24];

	if (text != "")
	{
		cache = TextureManager::instance()->CacheText(text, font_path, font_size, color, limited_w, is_wrapped);

		switch (m_format)
		{
			case TEXTFORMAT_MIDDLE:
				dest_rect.x = position.x - ((limited_w > cache->w || limited_w == 0) ? cache->w : limited_w) / 2;
				dest_rect.y = position.y - cache->h / 2;
			break;
			case TEXTFORMAT_RIGHT:
				dest_rect.x = position.x - (limited_w > cache->w || limited_w == 0) ? cache->w : limited_w;
			break;
		}

		dest_rect.w = (limited_w > cache->w || limited_w == 0) ? cache->w : limited_w;
		dest_rect.h = cache->h;
		source_rect.w = dest_rect.w / cache->w;
		source_rect.h = cache->h;
		Rect2DtoVectrices(dest_rect, source_rect, vectrices, true, true, 1);
	}
}

void fr::TextArea::render()
{
	if (text != "")
	{
		TextureManager::instance()->render(cache->texture, vectrices, color.a, matrix_id);
	}
}

void fr::TextArea::render(int x, int y, fr::TextFormat format)
{
	if (text != "")
	{
		format = format == TEXTFORMAT_NONE ? m_format : format;
		switch (format)
		{
			case TEXTFORMAT_MIDDLE:
				x -= ((limited_w > cache->w || limited_w == 0) ? cache->w : limited_w) / 2;
				y -= cache->h / 2;
			break;
			case TEXTFORMAT_RIGHT:
				x -= (limited_w > cache->w || limited_w == 0) ? cache->w : limited_w;
			break;
		}

		float *temp_vectrices = new float[24];
		memcpy(temp_vectrices, vectrices, 24 * sizeof(float));
		
		TextureManager::instance()->render(cache->texture, temp_vectrices, color.a, matrix_id);
		delete [] temp_vectrices;
	}
}

void fr::TextArea::clear()
{
	TextureManager::instance()->DestroyCache(cache);
	delete [] vectrices;
}

void fr::TextArea::SetPos(int x, int y)
{
	if (cache)
	{
		dest_rect.x = position.x = x;
		dest_rect.y = position.y = y;
		switch (m_format)
		{
			case TEXTFORMAT_MIDDLE:
				dest_rect.x = position.x - ((limited_w > cache->w || limited_w == 0) ? cache->w : limited_w) / 2;
				dest_rect.y = position.y - cache->h / 2;
			break;
			case TEXTFORMAT_RIGHT:
				dest_rect.x = position.x - (limited_w > cache->w || limited_w == 0) ? cache->w : limited_w;
			break;
		}

		Rect2DtoVectrices(dest_rect, source_rect, vectrices, true, false, 1);
	}
}

void fr::TextArea::SetSize(int w, int h)
{
	dest_rect.w = w;
	dest_rect.h = h;
	Rect2DtoVectrices(dest_rect, source_rect, vectrices, true, false, 1);
}

void fr::TextArea::SetText(std::string input_text)
{
	if (text != input_text)
	{
		if (text != "")
		{
			TextureManager::instance()->DestroyCache(cache);
		}
		text = input_text;
		if (text != "")
		{
			cache = TextureManager::instance()->CacheText(text, font_path, font_size, color, limited_w);
			source_rect.w = cache->w;
			source_rect.h = cache->h;
			switch (m_format)
			{
				case TEXTFORMAT_MIDDLE:
					dest_rect.x = position.x - ((limited_w > cache->w || limited_w == 0) ? cache->w : limited_w) / 2;
					dest_rect.y = position.y - cache->h / 2;
				break;
				case TEXTFORMAT_RIGHT:
					dest_rect.x = position.x - (limited_w > cache->w || limited_w == 0) ? cache->w : limited_w;
				break;
			}

			dest_rect.w = (limited_w > cache->w || limited_w == 0) ? cache->w : limited_w;
			dest_rect.h = cache->h;
			source_rect.w = dest_rect.w / cache->w;
			source_rect.h = cache->h;
			Rect2DtoVectrices(dest_rect, source_rect, vectrices, true, true, 1);
		}
	}

}

void fr::TextArea::SetColor(Color input_color)
{
	color = input_color;
	if (text != "")
	{
		TextureManager::instance()->DestroyCache(cache);
		cache = TextureManager::instance()->CacheText(text, font_path, font_size, color, limited_w);
	}
}

void fr::TextArea::SetAlpha(int input_alpha)
{
	color.a = alpha;
}

void fr::TextArea::SetFont(std::string input_font_path, int input_font_size)
{
	font_path = input_font_path;
	font_size = input_font_size;
	if (text != "")
	{
		TextureManager::instance()->DestroyCache(cache);
		cache = TextureManager::instance()->CacheText(text, font_path, font_size, color, limited_w);
		switch (m_format)
		{
			case TEXTFORMAT_MIDDLE:
				dest_rect.x = position.x - ((limited_w > cache->w || limited_w == 0) ? cache->w : limited_w) / 2;
				dest_rect.y = position.y - cache->h / 2;
			break;
			case TEXTFORMAT_RIGHT:
				dest_rect.x = position.x - (limited_w > cache->w || limited_w == 0) ? cache->w : limited_w;
			break;
		}

		dest_rect.w = (limited_w > cache->w || limited_w == 0) ? cache->w : limited_w;
		dest_rect.h = cache->h;
		source_rect.w = dest_rect.w / cache->w;
		source_rect.h = cache->h;
		Rect2DtoVectrices(dest_rect, source_rect, vectrices, true, true, 1);
	}
}

void fr::TextArea::SetScale(float input_scale)
{
	scale = input_scale;
	if (text != "")
	{
		TextureManager::instance()->DestroyCache(cache);
		cache = TextureManager::instance()->CacheText(text, font_path, font_size, color, limited_w);
		source_rect.w = cache->w;
		source_rect.h = cache->h;
		switch (m_format)
		{
			case TEXTFORMAT_MIDDLE:
				dest_rect.x = position.x - ((limited_w > cache->w || limited_w == 0) ? cache->w : limited_w) / 2;
				dest_rect.y = position.y - cache->h / 2;
			break;
			case TEXTFORMAT_RIGHT:
				dest_rect.x = position.x - (limited_w > cache->w || limited_w == 0) ? cache->w : limited_w;
			break;
		}

		dest_rect.w = (limited_w > cache->w || limited_w == 0) ? cache->w : limited_w;
		dest_rect.h = cache->h;
		source_rect.w = dest_rect.w / cache->w;
		source_rect.h = cache->h;
		Rect2DtoVectrices(dest_rect, source_rect, vectrices, true, true, 1);
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
	return position.x;
}
int fr::TextArea::GetUserY()
{
	return position.y;
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
	return text;
}

float fr::TextArea::GetScale()
{
	return scale;
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