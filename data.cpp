#include "data.h"
#include <glm/glm.hpp>
#include "texture_manager.h"
#include "system.h"

void fr::Rect2DtoGLVectrices(Rect dest_rect, Rect source_rect, Rect texture_size, float *vectrices, bool modify_dest_rect, bool modify_source_rect, float scale)
{
	if (modify_dest_rect)
	{
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
	if (modify_source_rect)
	{
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

	vectrices[2] = 0.f;
	vectrices[3] = 1.f;
	vectrices[8] = 0.f;
	vectrices[9] = 1.f;
	vectrices[14] = 0.f;
	vectrices[15] = 1.f;
	vectrices[20] = 0.f;
	vectrices[21] = 1.f;
}

bool fr::UTF8toUnicode(const std::string &utf8, std::vector<unsigned int> &unicode)
{
	unsigned int utf8_length = utf8.size();
	unsigned int utf8_cursor = 0;
	unsigned int offset;
	unsigned char *utf8_ch;
	unsigned int unicode_ch;
	unicode.clear();
	while (utf8_cursor < utf8_length)
	{
		utf8_ch = (unsigned char*)&utf8[utf8_cursor];
		if (*utf8_ch < 0xC0)
		{
			offset = 0;
			unicode_ch = utf8_ch[0];
		}
		else if (*utf8_ch < 0xE0)
		{
			/*2:<11000000>*/
			offset = 1;
			if (utf8_cursor + offset >= utf8_length)
			{
				return false;
			}
			unicode_ch = (utf8_ch[0] & 0x1f) << 6;
			unicode_ch |= (utf8_ch[1] & 0x3f);
		}
		else if (*utf8_ch < 0xF0)
		{
			/*3:<11100000>*/
			offset = 2;
			if (utf8_cursor + offset >= utf8_length)
			{
				return false;
			}
			unicode_ch = (utf8_ch[0] & 0x0f) << 12;
			unicode_ch |= (utf8_ch[1] & 0x3f) << 6;
			unicode_ch |= (utf8_ch[2] & 0x3f);
		}
		else if (*utf8_ch < 0xF8)
		{
			/*4:<11110000>*/
			offset = 3;
			if (utf8_cursor + offset >= utf8_length)
			{
				return false;
			}
			unicode_ch = (utf8_ch[0] & 0x07) << 18;
			unicode_ch |= (utf8_ch[1] & 0x3f) << 12;
			unicode_ch |= (utf8_ch[2] & 0x3f) << 6;
			unicode_ch |= (utf8_ch[3] & 0x3f);
		}
		else if (*utf8_ch < 0xFC)
		{
			/*5:<11111000>*/
			offset = 4;
			if (utf8_cursor + offset >= utf8_length)
			{
				return false;
			}
			unicode_ch = (utf8_ch[0] & 0x03) << 24;
			unicode_ch |= (utf8_ch[1] & 0x3f) << 18;
			unicode_ch |= (utf8_ch[2] & 0x3f) << 12;
			unicode_ch |= (utf8_ch[3] & 0x3f) << 6;
			unicode_ch |= (utf8_ch[4] & 0x3f);
		}
		else
		{
			/*6:<11111100>*/
			offset = 5;
			if (utf8_cursor + offset >= utf8_length)
			{
				return false;
			}
			unicode_ch = (utf8_ch[0] & 0x01) << 30;
			unicode_ch |= (utf8_ch[1] & 0x3f) << 24;
			unicode_ch |= (utf8_ch[2] & 0x3f) << 18;
			unicode_ch |= (utf8_ch[3] & 0x3f) << 12;
			unicode_ch |= (utf8_ch[4] & 0x3f) << 6;
			unicode_ch |= (utf8_ch[5] & 0x3f);
		}
		unicode.push_back(unicode_ch);
		utf8_cursor += offset + 1;
	}
	return true;
}

fr::Point2Di fr::Point3DftoPoint2Di(fr::Point3Df input)
{
	glm::vec4 gl_pos(input.x, input.y, input.z, 1.f);
	glm::mat4x4 mvp_matrix = TextureManager::instance()->GetMatrix("mvp");
	glm::vec4 converted_pos;
	converted_pos = mvp_matrix * gl_pos;
	converted_pos = converted_pos * (1.0f / converted_pos.w);
	Point2Di output;
	output.x = (converted_pos.x + 1.f) / 2.f * System::instance()->GetWindowWidth();
	output.y = (1.f - (converted_pos.y + 1.f) / 2.f) * System::instance()->GetWindowHeigh();
	return output;
}

fr::Vector2Di::Vector2Di(int input_x, int input_y)
{
	x = input_x;
	y = input_y;
	length = sqrt(x * x + y * y);
	angle = atan(double(y) / double(x));
}

fr::Vector2Di::Vector2Di(double input_length, double input_angle)
{
	length = input_length;
	angle = input_angle;
	x = input_length * cos(input_angle);
	y = input_length * sin(input_angle);
}

fr::Vector2Di fr::Vector2Di::operator+(fr::Vector2Di input)
{
	return Vector2Di(x + input.x, y + input.y);
}

fr::Vector2Di fr::Vector2Di::operator-(fr::Vector2Di input)
{
	return Vector2Di(x - input.x, y - input.y);
}

fr::Vector2Di fr::Vector2Di::operator*(double input)
{
	return Vector2Di(int(x * input), int(y * input));
}

fr::Vector2Di fr::Vector2Di::operator/(double input)
{
	return Vector2Di(int(x / input), int(y / input));
}

int fr::Vector2Di::operator*(fr::Vector2Di input)
{
	return x * input.x + y * input.y;
}

double fr::Vector2Di::GetIntersectingAngle(Vector2Di input)
{
	int output = angle - input.angle;
	output = output > 0 ? output : -output;
	return output;
}

fr::Vector2Di fr::Point2Di::operator-(Point2Di input)
{
	return Vector2Di(x - input.x, y - input.y);
}

fr::Point2Di fr::Point2Di::operator+(fr::Vector2Di input)
{
	return Point2Di(x + input.x, y + input.y);
}

bool fr::CompareString(std::string str1, std::string str2)
{
	for (int i = 0; (i < str1.size()) && (i < str2.size()); i++)
	{
		if (str1[i] > str2[i])
		{
			return false;
		}
		else if (str1[i] < str2[i])
		{
			return true;
		}
	}
	if (str1.length() > str2.length())
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool fr::CompareTitle(const SongInformation *arg1, const SongInformation *arg2)
{
	return CompareString(arg1->title, arg2->title);
}

bool fr::CompareArtist(const SongInformation *arg1, const SongInformation *arg2)
{
	return CompareString(arg1->artist, arg2->artist);
}

bool fr::CompareNoter(const SongInformation *arg1, const SongInformation *arg2)
{
	return CompareString(arg1->noter, arg2->noter);
}

bool fr::CompareDifficulty(const SongInformation *arg1, const SongInformation *arg2)
{
	return arg1->difficulty > arg2->difficulty;
}

bool fr::CompareDuration(const SongInformation *arg1, const SongInformation *arg2)
{
	return arg1->duration > arg2->duration;
}


bool fr::CompareTitleReverse(const SongInformation *arg1, const SongInformation *arg2)
{
	return !CompareString(arg1->title, arg2->title);
}

bool fr::CompareArtistReverse(const SongInformation *arg1, const SongInformation *arg2)
{
	return !CompareString(arg1->artist, arg2->artist);
}

bool fr::CompareNoterReverse(const SongInformation *arg1, const SongInformation *arg2)
{
	return !CompareString(arg1->noter, arg2->noter);
}

bool fr::CompareDifficultyReverse(const SongInformation *arg1, const SongInformation *arg2)
{
	return arg1->difficulty < arg2->difficulty;
}

bool fr::CompareDurationReverse(const SongInformation *arg1, const SongInformation *arg2)
{
	return arg1->duration < arg2->duration;
}

bool fr::CompareNote(const Note *arg1, const Note *arg2)
{
	return arg1->time < arg2->time;
}

//用於排序
