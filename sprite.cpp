#include "sprite.h"
#include "texture_manager.h"
#include "timer.h"
#include "system.h"

void fr::Sprite::init(std::string path, Rect load_dest_rect, Rect load_source_rect)
{
	dest_rect = load_dest_rect;
	source_rect = load_source_rect;
	texture_size.w = 0;
	texture_size.h = 0;
	scale = 1;
	vectrices = new float[24];

	if (path != "")
	{
		TextureManager::instance()->load(path, texture_size);
		if (load_source_rect.w == 0 && load_source_rect.h == 0)
		{
			source_rect.w = texture_size.w;
			source_rect.h = texture_size.h;
		}
		if (load_dest_rect.w == 0 && load_dest_rect.h == 0)
		{
			dest_rect.w = texture_size.w;
			dest_rect.h = texture_size.h;
		}
	}

	vectrices[0] = (float(dest_rect.x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
	vectrices[1] = (1.f - float(dest_rect.y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
	vectrices[2] = 0.f;
	vectrices[3] = 1.f;
	vectrices[4] = float(source_rect.x) / float(texture_size.w);
	vectrices[5] = float(source_rect.y) / float(texture_size.h);
	//top left
	vectrices[6] = (float(dest_rect.x + dest_rect.w * scale) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
	vectrices[7] = (1.f - float(dest_rect.y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
	vectrices[8] = 0.f;
	vectrices[9] = 1.f;
	vectrices[10] = float(source_rect.x + source_rect.w) / float(texture_size.w);
	vectrices[11] = float(source_rect.y) / float(texture_size.h);
	//top right
	vectrices[12] = (float(dest_rect.x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
	vectrices[13] = (1.f - float(dest_rect.y + dest_rect.h * scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
	vectrices[14] = 0.f;
	vectrices[15] = 1.f;
	vectrices[16] = float(source_rect.x) / float(texture_size.w);
	vectrices[17] = float(source_rect.y + source_rect.h) / float(texture_size.h);
	//bottom left
	vectrices[18] = float(dest_rect.x + dest_rect.w * scale) / float(System::instance()->GetWindowWidth()) * 2.f - 1.f;
	vectrices[19] = (1.f - float(dest_rect.y + dest_rect.h * scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
	vectrices[20] = 0.f;
	vectrices[21] = 1.f;
	vectrices[22] = float(source_rect.x + source_rect.w) / float(texture_size.w);
	vectrices[23] = float(source_rect.y + source_rect.h) / float(texture_size.h);
	//bottom right

	frame.push_back(path);
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
}

void fr::Sprite::render()
{
	if (frame[current_index] != "")
	{
		TextureManager::instance()->render(frame[current_index], vectrices, matrix_id);
	}
}

void fr::Sprite::render(int index)
{
	if (frame[index] != "")
	{
		TextureManager::instance()->render(frame[index], vectrices, matrix_id);
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
		vectrices[0] = (float(dest_rect.x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
		vectrices[1] = (1.f - float(dest_rect.y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		vectrices[4] = float(source_rect.x) / float(texture_size.w);
		vectrices[5] = float(source_rect.y) / float(texture_size.h);
		//top left
		vectrices[6] = (float(dest_rect.x + dest_rect.w * scale) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
		vectrices[7] = (1.f - float(dest_rect.y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		vectrices[10] = float(source_rect.x + source_rect.w) / float(texture_size.w);
		vectrices[11] = float(source_rect.y) / float(texture_size.h);
		//top right
		vectrices[12] = (float(dest_rect.x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
		vectrices[13] = (1.f - float(dest_rect.y + dest_rect.h * scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		vectrices[16] = float(source_rect.x) / float(texture_size.w);
		vectrices[17] = float(source_rect.y + source_rect.h) / float(texture_size.h);
		//bottom left
		vectrices[18] = float(dest_rect.x + dest_rect.w * scale) / float(System::instance()->GetWindowWidth()) * 2.f - 1.f;
		vectrices[19] = (1.f - float(dest_rect.y + dest_rect.h * scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
		vectrices[22] = float(source_rect.x + source_rect.w) / float(texture_size.w);
		vectrices[23] = float(source_rect.y + source_rect.h) / float(texture_size.h);
		//bottom right
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
	vectrices[0] = (float(dest_rect.x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
	vectrices[1] = (1.f - float(dest_rect.y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
	//top left
	vectrices[6] = (float(dest_rect.x + dest_rect.w * scale) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
	vectrices[7] = (1.f - float(dest_rect.y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
	//top right
	vectrices[12] = (float(dest_rect.x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
	vectrices[13] = (1.f - float(dest_rect.y + dest_rect.h * scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
	//bottom left
	vectrices[18] = float(dest_rect.x + dest_rect.w * scale) / float(System::instance()->GetWindowWidth()) * 2.f - 1.f;
	vectrices[19] = (1.f - float(dest_rect.y + dest_rect.h * scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
	//bottom right
}

void fr::Sprite::SetPos(int x, int y, int z)
{
	glm::vec4 gl_pos(float(x) / System::instance()->GetWindowWidth() * 2.f - 1.f, (1.f - float(y) / System::instance()->GetWindowHeigh()) * 2.f - 1.f, -float(z) / 360.f, 1.f);
//	glm::vec4 gl_pos(float(x), float(y), float(z), 1.f);

	glm::mat4x4 mvp_matrix = TextureManager::instance()->GetMatrix("mvp");
	glm::vec4 converted_pos;
	converted_pos = mvp_matrix * gl_pos;
	converted_pos = converted_pos * (1.0f / converted_pos.w);
	dest_rect.x = ((converted_pos.x + 1.f) / 2.f * System::instance()->GetWindowWidth()) - dest_rect.w / 2;
	dest_rect.y = (1.f - (converted_pos.y + 1.f) / 2.f) * System::instance()->GetWindowHeigh() - dest_rect.h / 2;
	vectrices[0] = (float(dest_rect.x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
	vectrices[1] = (1.f - float(dest_rect.y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
	//top left
	vectrices[6] = (float(dest_rect.x + dest_rect.w * scale) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
	vectrices[7] = (1.f - float(dest_rect.y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
	//top right
	vectrices[12] = (float(dest_rect.x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
	vectrices[13] = (1.f - float(dest_rect.y + dest_rect.h * scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
	//bottom left
	vectrices[18] = float(dest_rect.x + dest_rect.w * scale) / float(System::instance()->GetWindowWidth()) * 2.f - 1.f;
	vectrices[19] = (1.f - float(dest_rect.y + dest_rect.h * scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
	//bottom right
}

void fr::Sprite::SetScale(float load_scale)
{
	scale = load_scale;
}

void fr::Sprite::SetSize(int w, int h)
{
	dest_rect.w = w;
	dest_rect.h = h;
	vectrices[0] = (float(dest_rect.x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
	vectrices[1] = (1.f - float(dest_rect.y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
	//top left
	vectrices[6] = (float(dest_rect.x + dest_rect.w * scale) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
	vectrices[7] = (1.f - float(dest_rect.y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
	//top right
	vectrices[12] = (float(dest_rect.x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
	vectrices[13] = (1.f - float(dest_rect.y + dest_rect.h * scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
	//bottom left
	vectrices[18] = float(dest_rect.x + dest_rect.w * scale) / float(System::instance()->GetWindowWidth()) * 2.f - 1.f;
	vectrices[19] = (1.f - float(dest_rect.y + dest_rect.h * scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
	//bottom right
}

void fr::Sprite::SetSrcRect(Rect load_source_rect)
{
	source_rect = load_source_rect;
	vectrices[4] = float(source_rect.x) / float(texture_size.w);
	vectrices[5] = float(source_rect.y) / float(texture_size.h);
	//top left
	vectrices[10] = float(source_rect.x + source_rect.w) / float(texture_size.w);
	vectrices[11] = float(source_rect.y) / float(texture_size.h);
	//top right
	vectrices[16] = float(source_rect.x) / float(texture_size.w);
	vectrices[17] = float(source_rect.y + source_rect.h) / float(texture_size.h);
	//bottom left
	vectrices[22] = float(source_rect.x + source_rect.w) / float(texture_size.w);
	vectrices[23] = float(source_rect.y + source_rect.h) / float(texture_size.h);
	//bottom right
}

void fr::Sprite::SetRotation(Point load_center, double load_angle)
{
	center = load_center;
	angle = load_angle;
}

void fr::Sprite::SetVectrices(int *load_vectrices)
{
	vectrices[0] = (float(load_vectrices[0]) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
	vectrices[1] = (1.f - float(load_vectrices[1]) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
	vectrices[2] = -float(load_vectrices[2]) / float(360);
	vectrices[3] = 1.f;
	vectrices[4] = float(load_vectrices[4]) / float(texture_size.w);
	vectrices[5] = float(load_vectrices[5]) / float(texture_size.h);
	//top left
	vectrices[6] = (float(load_vectrices[6]) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
	vectrices[7] = (1.f - float(load_vectrices[7]) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
	vectrices[8] = -float(load_vectrices[8]) / float(360);
	vectrices[9] = 1.f;
	vectrices[10] = float(load_vectrices[10]) / float(texture_size.w);
	vectrices[11] = float(load_vectrices[11]) / float(texture_size.h);
	//top right
	vectrices[12] = (float(load_vectrices[12]) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
	vectrices[13] = (1.f - float(load_vectrices[13]) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
	vectrices[14] = -float(load_vectrices[14]) / float(360);
	vectrices[15] = 1.f;
	vectrices[16] = float(load_vectrices[16]) / float(texture_size.w);
	vectrices[17] = float(load_vectrices[17]) / float(texture_size.h);
	//bottom left
	vectrices[18] = (float(load_vectrices[18]) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f;
	vectrices[19] = (1.f - float(load_vectrices[19]) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f;
	vectrices[20] = -float(load_vectrices[20]) / float(360);
	vectrices[21] = 1.f;
	vectrices[22] = float(load_vectrices[22]) / float(texture_size.w);
	vectrices[23] = float(load_vectrices[23]) / float(texture_size.h);
	//bottom right
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