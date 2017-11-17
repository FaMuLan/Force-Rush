#include "sprite.h"
#include "texture_manager.h"
#include "timer.h"

void lm::Sprite::init(std::string path, int x, int y, int w, int h)
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

void lm::Sprite::update()
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

void lm::Sprite::render()
{
	if (frame[current_index] != "")
	{
		TextureManager::instance()->render(frame[current_index], m_x, m_y, m_w * scale, m_h * scale);
	}
}

void lm::Sprite::clear()
{
//	TextureManager::instance()->clear(m_path);
}

void lm::Sprite::AddFrame(std::string path)
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

void lm::Sprite::SetAnimate(int load_start, int load_end, int load_duration)
{
	frame_start = load_start;
	frame_end = load_end;
	duration = load_duration;
	start_time = Timer::instance()->GetSystemTime();
}

void lm::Sprite::SetBaseFrame(int index)
{
	base_index = index;
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

int lm::Sprite::GetW()
{
	return m_w;
}

int lm::Sprite::GetH()
{
	return m_h;
}

void lm::Sprite::SetSize(int w, int h)
{
	m_w = w;
	m_h = h;
}