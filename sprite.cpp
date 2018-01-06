#include "sprite.h"
#include "texture_manager.h"
#include "timer.h"

void fr::Sprite::init(std::string path, int x, int y, int w, int h)
{
	if (path != "")
	{
		TextureManager::instance()->load(path, m_w, m_h);
	}
	m_x = x;
	m_y = y;
	if (w != 0 || h != 0)
	{
		m_w = w;
		m_h = h;
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
		TextureManager::instance()->render(frame[current_index], m_x, m_y, m_w * scale, m_h * scale);
	}
}

void fr::Sprite::clear()
{
//	TextureManager::instance()->clear(m_path);
}

void fr::Sprite::AddFrame(std::string path)
{
	int load_w, load_h;
	if (path != "")
	{
		TextureManager::instance()->load(path, load_w, load_h);
	}
	if (m_w == 0 && m_h == 0)
	{
		m_w = load_w;
		m_h = load_h;
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
	m_x = x;
	m_y = y;
}

void fr::Sprite::SetScale(float load_scale)
{
	scale = load_scale;
}

int fr::Sprite::GetX()
{
	return m_x;
}

int fr::Sprite::GetY()
{
	return m_y;
}

int fr::Sprite::GetW()
{
	return m_w;
}

int fr::Sprite::GetH()
{
	return m_h;
}

void fr::Sprite::SetSize(int w, int h)
{
	m_w = w;
	m_h = h;
}