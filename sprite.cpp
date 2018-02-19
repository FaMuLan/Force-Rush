#include "sprite.h"
#include "texture_manager.h"
#include "timer.h"

void fr::Sprite::init(std::string path, Rect load_dest_rect, Rect load_source_rect, Point load_center, double load_angle)
{
	dest_rect = load_dest_rect;
	source_rect = load_source_rect;
	center = load_center;
	angle = load_angle;

	if (path != "")
	{
		TextureManager::instance()->load(path, dest_rect);
		if (load_source_rect.w == 0 || load_source_rect.h == 0)
		{
			source_rect.w = dest_rect.w;
			source_rect.h = dest_rect.h;
		}
	}
	if (load_dest_rect.w != 0 || load_dest_rect.h != 0)
	{
		dest_rect.w = load_dest_rect.w;
		dest_rect.h = load_dest_rect.h;
	}

	frame.push_back(path);
	scale = 1;
	base_index = 0;
	start_time = 0;
	duration = 0;
	current_index = 0;
}

void fr::Sprite::update()
{
	if (duration != 0)
	{
		current_index = (frame_end != frame_start) ? (float(Timer::instance()->GetSystemTime() - start_time) / duration * float(frame_end - frame_start) + frame_start) : frame_start;
		if (Timer::instance()->GetSystemTime() - start_time > duration)
		{
			duration = 0;
			start_time = 0;
			current_index = base_index;
		}
	}
	else
	{
		current_index = base_index;
	}
}

void fr::Sprite::render()
{
	if (frame[current_index] != "")
	{
		TextureManager::instance()->render(frame[current_index], dest_rect, source_rect, center, angle, scale);
	}
}

void fr::Sprite::render(int index)
{
	if (frame[index] != "")
	{
		TextureManager::instance()->render(frame[index], dest_rect, source_rect, center, angle, scale);
	}
}

void fr::Sprite::clear()
{
//	TextureManager::instance()->clear(m_path);
}

void fr::Sprite::AddFrame(std::string path)
{
	Rect load_size;
	if (path != "")
	{
		TextureManager::instance()->load(path, load_size);
	}
	if (dest_rect.w == 0 && dest_rect.h == 0)
	{
		dest_rect.w = load_size.w;
		dest_rect.h = load_size.h;
		source_rect.w = load_size.w;
		source_rect.h = load_size.h;
	}
	frame.push_back(path);
}

void fr::Sprite::SetAnimate(int load_start, int load_end, int load_duration)
{
	frame_start = load_start;
	frame_end = load_end;
	duration = load_duration;
	start_time = Timer::instance()->GetSystemTime();
}

int fr::Sprite::GetCurrentIndex()
{
	return current_index;
}

void fr::Sprite::SetBaseFrame(int index)
{
	base_index = index;
}

void fr::Sprite::SetPos(int x, int y)
{
	dest_rect.x = x;
	dest_rect.y = y;
}

void fr::Sprite::SetScale(float load_scale)
{
	scale = load_scale;
}

void fr::Sprite::SetSize(int w, int h)
{
	dest_rect.w = w;
	dest_rect.h = h;
}

void fr::Sprite::SetSrcRect(Rect load_source_rect)
{
	source_rect = load_source_rect;
}

void fr::Sprite::SetRotation(Point load_center, double load_angle)
{
	center = load_center;
	angle = load_angle;
}

int fr::Sprite::GetX()
{
	return dest_rect.x;
}

int fr::Sprite::GetY()
{
	return dest_rect.y;
}

int fr::Sprite::GetW()
{
	return dest_rect.w;
}

int fr::Sprite::GetH()
{
	return dest_rect.h;
}