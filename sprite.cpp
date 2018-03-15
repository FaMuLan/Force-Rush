#include "sprite.h"
#include "texture_manager.h"
#include "timer.h"
#include "system.h"

void fr::Sprite::init(std::string path, Rect load_dest_rect, Rect load_source_rect, Point load_center, double load_angle)
{
	dest_rect = load_dest_rect;
	source_rect = load_source_rect;
	center = load_center;
	angle = load_angle;
	texture_size.w = 0;
	texture_size.h = 0;
	vectrices = NULL;

	if (path != "")
	{
		TextureManager::instance()->load(path, texture_size);
		if (load_source_rect.w == 0 && load_source_rect.h == 0)
		{
			source_rect.w = texture_size.w;
			source_rect.h = texture_size.h;
		}
	}
	if (load_dest_rect.w != 0 || load_dest_rect.h != 0)
	{
		dest_rect.w = load_dest_rect.w;
		dest_rect.h = load_dest_rect.h;
	}
	else
	{
		dest_rect.w = texture_size.w;
		dest_rect.h = texture_size.h;
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
		if (vectrices == NULL)
		{
			TextureManager::instance()->render(frame[current_index], dest_rect, texture_size, source_rect, center, angle, scale);
		}
		else
		{
			TextureManager::instance()->render(frame[current_index], vectrices);
		}
	}
}

void fr::Sprite::render(int index)
{
	if (frame[index] != "")
	{
		if (vectrices == NULL)
		{
			TextureManager::instance()->render(frame[index], dest_rect, texture_size, source_rect, center, angle, scale);
		}
		else
		{
			TextureManager::instance()->render(frame[index], vectrices);
		}
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
	if (texture_size.w == 0 && texture_size.h == 0)
	{
		texture_size.w = load_size.w;
		texture_size.h = load_size.h;
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

void fr::Sprite::SetVectrices(int *load_vectrices)
{
	if (vectrices == NULL)
	{
		vectrices = new float[24];
	}
	vectrices[0] = (float(load_vectrices[0]) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
	vectrices[1] = (1.f - float(load_vectrices[1]) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
	vectrices[2] = -float(load_vectrices[2]) / float(System::instance()->GetWindowDepth()) * 10;
	vectrices[3] = 1.f;
	vectrices[4] = float(load_vectrices[4]) / float(texture_size.w);
	vectrices[5] = float(load_vectrices[5]) / float(texture_size.h);
	//top left
	vectrices[6] = (float(load_vectrices[6]) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
	vectrices[7] = (1.f - float(load_vectrices[7]) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
	vectrices[8] = -float(load_vectrices[8]) / float(System::instance()->GetWindowDepth()) * 10;
	vectrices[9] = 1.f;
	vectrices[10] = float(load_vectrices[10]) / float(texture_size.w);
	vectrices[11] = float(load_vectrices[11]) / float(texture_size.h);
	//top right
	vectrices[12] = (float(load_vectrices[12]) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
	vectrices[13] = (1.f - float(load_vectrices[13]) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
	vectrices[14] = -float(load_vectrices[14]) / float(System::instance()->GetWindowDepth()) * 10;
	vectrices[15] = 1.f;
	vectrices[16] = float(load_vectrices[16]) / float(texture_size.w);
	vectrices[17] = float(load_vectrices[17]) / float(texture_size.h);
	//bottom left
	vectrices[18] = (float(load_vectrices[18]) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
	vectrices[19] = (1.f - float(load_vectrices[19]) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
	vectrices[20] = -float(load_vectrices[20]) / float(System::instance()->GetWindowDepth()) * 10;
	vectrices[21] = 1.f;
	vectrices[22] = float(load_vectrices[22]) / float(texture_size.w);
	vectrices[23] = float(load_vectrices[23]) / float(texture_size.h);
	//bottom right
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