#include "texture_manager.h"
#include "system.h"
#include "shape.h"

fr::TextureManager *fr::TextureManager::m_instance = 0;

void fr::TextureManager::init(GLuint program_object)
{
	null_matrix = new Matrix;
	null_matrix->LoadIdentity();
	position_location = glGetAttribLocation(program_object, "a_position");
	texture_coord_location = glGetAttribLocation(program_object, "a_texCoord");
	sampler_location = glGetUniformLocation(program_object, "s_texture");
	mvp_location = glGetUniformLocation(program_object, "mvp_matrix");
}

void fr::TextureManager::load(std::string path, Rect &output_size)
{
	SDL_Surface *load_surface = IMG_Load(path.c_str());
	if (!texture[path])
	{
		SDL_Surface *converted_surface = SDL_ConvertSurfaceFormat(load_surface, SDL_PIXELFORMAT_ABGR8888, 0);
		GLuint *new_texture = new GLuint;
		glGenTextures(1, new_texture);
		glBindTexture(GL_TEXTURE_2D, *new_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, converted_surface->w, converted_surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, converted_surface->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		texture[path] = new_texture;
		SDL_FreeSurface(converted_surface);
	}
	output_size.w = load_surface->w;
	output_size.h = load_surface->h;
	SDL_FreeSurface(load_surface);
}

void fr::TextureManager::load(GLuint *load_texture, std::string path)
{
	texture[path] = load_texture;
}

void fr::TextureManager::loadfont(std::string path, int size)
{
	if (!font[path][size])
	{
		TTF_Font *new_font = TTF_OpenFont(path.c_str(), size);
		font[path][size] = new_font;
	}
}

void fr::TextureManager::clear()
{
	texture.clear();
}

void fr::TextureManager::clear(std::string path)
{
	glDeleteTextures(1, texture[path]);
	delete texture[path];
	texture[path] = NULL;
}

void fr::TextureManager::clearfont(std::string path, int size)
{
	TTF_CloseFont(font[path][size]);
	font[path][size] = NULL;
}

void fr::TextureManager::update()
{
	glEnableVertexAttribArray(position_location);
	glEnableVertexAttribArray(texture_coord_location);
	glActiveTexture(GL_TEXTURE0);
}

void fr::TextureManager::render(std::string path, Rect dest_rect, Rect texture_size, Rect source_rect, Point center, double angle, float scale)
{
	GLfloat gles_vectrices[] =
	{
		(float(dest_rect.x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f,
		(1.f - float(dest_rect.y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f,
		0.f, 1.f,
		float(source_rect.x) / float(texture_size.w),
		float(source_rect.y) / float(texture_size.h),
		//top left
		(float(dest_rect.x + dest_rect.w * scale) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f,
		(1.f - float(dest_rect.y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f,
		0.f, 1.f,
		float(source_rect.x + source_rect.w) / float(texture_size.w),
		float(source_rect.y) / float(texture_size.h),
		//top right
		(float(dest_rect.x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f,
		(1.f - float(dest_rect.y + dest_rect.h * scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f,
		0.f, 1.f,
		float(source_rect.x) / float(texture_size.w),
		float(source_rect.y + source_rect.h) / float(texture_size.h),
		//bottom left
		float(dest_rect.x + dest_rect.w * scale) / float(System::instance()->GetWindowWidth()) * 2.f - 1.f,
		(1.f - float(dest_rect.y + dest_rect.h * scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f,
		0.f, 1.f,
		float(source_rect.x + source_rect.w) / float(texture_size.w),
		float(source_rect.y + source_rect.h) / float(texture_size.h)
		//bottom right
	};
	//二维图像顶点数据，毕竟不需要z轴
	GLushort indices[] = { 0, 1, 2, 1, 2, 3 };
	//顶点顺序
	glVertexAttribPointer(position_location, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), gles_vectrices);
	glVertexAttribPointer(texture_coord_location, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), &gles_vectrices[4]);
	glBindTexture(GL_TEXTURE_2D, *texture[path]);
	glUniform1i(sampler_location, 0);
	glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (GLfloat*)(null_matrix));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
}

void fr::TextureManager::render(GLuint *load_texture, Rect dest_rect, Rect texture_size, Rect source_rect, Point center, double angle, float scale)
{
	GLfloat gles_vectrices[] =
	{
		(float(dest_rect.x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f,
		(1.f - float(dest_rect.y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f,
		0.f, 1.f,
		float(source_rect.x) / float(texture_size.w),
		float(source_rect.y) / float(texture_size.h),
		//top left
		(float(dest_rect.x + dest_rect.w * scale) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f,
		(1.f - float(dest_rect.y) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f,
		0.f, 1.f,
		float(source_rect.x + source_rect.w) / float(texture_size.w),
		float(source_rect.y) / float(texture_size.h),
		//top right
		(float(dest_rect.x) / float(System::instance()->GetWindowWidth())) * 2.f - 1.f,
		(1.f - float(dest_rect.y + dest_rect.h * scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f,
		0.f, 1.f,
		float(source_rect.x) / float(texture_size.w),
		float(source_rect.y + source_rect.h) / float(texture_size.h),
		//bottom left
		float(dest_rect.x + dest_rect.w * scale) / float(System::instance()->GetWindowWidth()) * 2.f - 1.f,
		(1.f - float(dest_rect.y + dest_rect.h * scale) / float(System::instance()->GetWindowHeigh())) * 2.f - 1.f,
		0.f, 1.f,
		float(source_rect.x + source_rect.w) / float(texture_size.w),
		float(source_rect.y + source_rect.h) / float(texture_size.h)
		//bottom right
	};
	//二维图像顶点数据，毕竟不需要z轴
	GLushort indices[] = { 0, 1, 2, 1, 2, 3 };
	//顶点顺序
	glVertexAttribPointer(position_location, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), gles_vectrices);
	glVertexAttribPointer(texture_coord_location, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), &gles_vectrices[4]);
	glBindTexture(GL_TEXTURE_2D, *load_texture);
	glUniform1i(sampler_location, 0);
	glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (GLfloat*)(null_matrix));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
}

fr::TextureCache *fr::TextureManager::CacheText(std::string text, std::string font_path, int font_size, char r, char g, char b, int limited_w, bool wrapper)
{
	SDL_Color color = { char(r), char(g), char(b) };
	SDL_Surface *text_surface;
	if (wrapper)
	{
		std::string text_line;
		for (int i = 0; i < text.length(); i++)
		{
			text_line += text[i];
			int load_w, load_h;
			TTF_SizeUTF8(font[font_path][font_size], text_line.c_str(), &load_w, &load_h);
			if (text[i] == '\n')
			{
				text_line.clear();
			}
			else if (load_w > limited_w)
			{
				text.insert(i, "\n");
				text_line.clear();
			}
		}
		text_surface = TTF_RenderUTF8_Blended_Wrapped(font[font_path][font_size], text.c_str(), color, limited_w);
	}
	else
	{
		text_surface = TTF_RenderUTF8_Blended(font[font_path][font_size], text.c_str(), color);
	}
	SDL_Surface *converted_surface = SDL_ConvertSurfaceFormat(text_surface, SDL_PIXELFORMAT_ABGR8888, 0);
	GLuint *new_texture = new GLuint;
	glGenTextures(1, new_texture);
	glBindTexture(GL_TEXTURE_2D, *new_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, converted_surface->w, converted_surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, converted_surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int w = text_surface->w;
	int h = text_surface->h;
	TextureCache *output_cache = new TextureCache;
	output_cache->texture = new_texture;
	output_cache->w = w;
	output_cache->h = h;

	SDL_FreeSurface(converted_surface);
	SDL_FreeSurface(text_surface);
	return output_cache;
}

void fr::TextureManager::DestroyCache(fr::TextureCache *cache)
{
	glDeleteTextures(1, cache->texture);
	delete cache;
}