#include "sprite.h"
#include "texture_manager.h"
#include "timer.h"
#include "system.h"

void fr::Sprite::init(std::string input_path, Rect input_dest_rect, Rect input_source_rect)
{
	dest_rect = input_dest_rect;
	source_rect = input_source_rect;
	texture_size.w = 0;
	texture_size.h = 0;
	scale = 1;
	alpha = 255;
	pos_3d = { 0.f, 0.f, 0.f };
	vectrices = new float[24];

	if (input_path != "")
	{
		TextureManager::instance()->load(input_path, texture_size);
		if (input_source_rect.w == 0 && input_source_rect.h == 0)
		{
			source_rect.w = texture_size.w;
			source_rect.h = texture_size.h;
		}
		if (input_dest_rect.w == 0 && input_dest_rect.h == 0)
		{
			dest_rect.w = texture_size.w;
			dest_rect.h = texture_size.h;
		}
	}

	Rect2DtoGLVectrices(dest_rect, source_rect, texture_size, vectrices, true, true, scale);

	frame.push_back(input_path);
	base_index = 0;
	start_time = 0;
	duration = 0;
	current_index = 0;
	matrix_id = "default";
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
	if (pos_3d.z != 0)
	{
		Point3Df pos_3d_temp = Point3Df(pos_3d.x, pos_3d.y, pos_3d.z);
		Point2Di pos_2d = Point3DftoPoint2Di(pos_3d_temp);
		pos_2d.x -= texture_size.w / 2;
		pos_2d.y -= texture_size.h / 2;
		SetPos(pos_2d.x, pos_2d.y);
	}
}

void fr::Sprite::render()
{
	if (frame[current_index] != "")
	{
		TextureManager::instance()->render(frame[current_index], vectrices, alpha, matrix_id);
	}
}

void fr::Sprite::render(int index)
{
	if (frame[index] != "")
	{
		TextureManager::instance()->render(frame[index], vectrices, alpha, matrix_id);
	}
}

void fr::Sprite::clear()
{
//	TextureManager::instance()->clear(m_path);
	delete [] vectrices;
}

void fr::Sprite::AddFrame(std::string input_path)
{
	Rect input_size;
	if (input_path != "")
	{
		TextureManager::instance()->load(input_path, input_size);
	}
	if (texture_size.w == 0 && texture_size.h == 0)
	{
		texture_size.w = input_size.w;
		texture_size.h = input_size.h;
		dest_rect.w = input_size.w;
		dest_rect.h = input_size.h;
		source_rect.w = input_size.w;
		source_rect.h = input_size.h;
		Rect2DtoGLVectrices(dest_rect, source_rect, texture_size, vectrices, true, true, scale);
	}
	frame.push_back(input_path);
}

void fr::Sprite::SetAnimate(int input_start, int input_end, int input_duration)
{
	frame_start = input_start;
	frame_end = input_end;
	duration = input_duration;
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
	Rect2DtoGLVectrices(dest_rect, source_rect, texture_size, vectrices, true, false, scale);
}

void fr::Sprite::SetScale(float input_scale)
{
	scale = input_scale;
}

void fr::Sprite::SetSize(int w, int h)
{
	dest_rect.w = w;
	dest_rect.h = h;
	Rect2DtoGLVectrices(dest_rect, source_rect, texture_size, vectrices, true, false, scale);
}

void fr::Sprite::SetSrcRect(Rect input_source_rect)
{
	source_rect = input_source_rect;
	Rect2DtoGLVectrices(dest_rect, source_rect, texture_size, vectrices, false, true, scale);
}

void fr::Sprite::SetAlpha(int input_alpha)
{
	alpha = input_alpha;
}

void fr::Sprite::SetVectrices(float *input_vectrices)
{
	vectrices = input_vectrices;
}

void fr::Sprite::SetMatrix(std::string id)
{
	matrix_id = id;
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