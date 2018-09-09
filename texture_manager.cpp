#include "texture_manager.h"
#include <glm/ext.hpp>
#include <ft2build.h>
#include FT_GLYPH_H
#include FT_FREETYPE_H
#include "system.h"
#include "user/setting.h"
#include "shape.h"

fr::TextureManager *fr::TextureManager::m_instance = 0;

void fr::TextureManager::init(GLuint program_object)
{
	ft_library = new FT_Library;
	FT_Init_FreeType(ft_library);
	matrix["default"] = glm::mat4(1.f);
	glm::mat4x4 perspective_matrix;
	glm::mat4x4 model_view_matrix;
	perspective_matrix = glm::perspective(glm::radians(60.f), float(System::instance()->GetWindowWidth()) / float(System::instance()->GetWindowHeigh()), 0.1f, 20.f);
	model_view_matrix = glm::mat4(1.f);
	model_view_matrix = glm::translate(model_view_matrix, glm::vec3(0.f, -Setting::instance()->GetCameraPosY() / float(System::instance()->GetWindowHeigh()) * 2.f - 1.f, -Setting::instance()->GetCameraPosZ() / 360.f));
	model_view_matrix = glm::rotate(model_view_matrix, glm::radians(float(Setting::instance()->GetCameraRotateX())), glm::vec3(1.0, 0.0, 0.0));
	matrix["model_view"] = model_view_matrix;
	glm::mat4x4 mvp_matrix = perspective_matrix * model_view_matrix;
	matrix["mvp"] = mvp_matrix;
	position_location = glGetAttribLocation(program_object, "a_position");
	texture_coord_location = glGetAttribLocation(program_object, "a_texCoord");
	sampler_location = glGetUniformLocation(program_object, "s_texture");
	mvp_location = glGetUniformLocation(program_object, "mvp_matrix");
	alpha_location = glGetUniformLocation(program_object, "u_alpha");
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

void fr::TextureManager::LoadFont(std::string path, int size)
{
	if (!font[path][size])
	{
		FT_Face *new_font = new FT_Face;
		FT_New_Face(*ft_library, path.c_str(), 0, new_font);
		FT_Set_Pixel_Sizes(*new_font, 0, size);
		font[path][size] = new_font;
	}
}

void fr::TextureManager::LoadMatrix(std::string id, glm::mat4x4 load_matrix)
{
	matrix[id] = load_matrix;
}

void fr::TextureManager::clear()
{
	texture.clear();
}

void fr::TextureManager::clear(std::string path)
{
	if (texture[path])
	{
		glDeleteTextures(1, texture[path]);
		delete texture[path];
		texture[path] = NULL;
	}
}

void fr::TextureManager::ClearFont(std::string path, int size)
{
	FT_Done_Face(*font[path][size]);
	font[path][size] = NULL;
}

void fr::TextureManager::ClearMatrix(std::string id)
{
	
}

void fr::TextureManager::update()
{
	glm::mat4x4 perspective_matrix;
	perspective_matrix = glm::perspective(glm::radians(60.f), float(System::instance()->GetWindowWidth()) / float(System::instance()->GetWindowHeigh()), 0.1f, 20.f);
	glm::mat4x4 mvp_matrix = perspective_matrix * matrix["model_view"];
	matrix["mvp"] = mvp_matrix;
	glEnableVertexAttribArray(position_location);
	glEnableVertexAttribArray(texture_coord_location);
	glActiveTexture(GL_TEXTURE0);
}

void fr::TextureManager::render(std::string path, float *load_vectrices, int alpha, std::string matrix_id)
{
	GLushort indices[] = { 0, 1, 2, 1, 2, 3 };
	glVertexAttribPointer(position_location, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), &load_vectrices[0]);
	glVertexAttribPointer(texture_coord_location, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), &load_vectrices[4]);
	glBindTexture(GL_TEXTURE_2D, *texture[path]);
	glUniform1i(sampler_location, 0);
	glUniformMatrix4fv(mvp_location, 1, GL_FALSE, &(matrix[matrix_id])[0][0]);
	glUniform1f(alpha_location, float(alpha / 255.f));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
}

void fr::TextureManager::render(GLuint *load_texture, float *load_vectrices, int alpha, std::string matrix_id)
{
	GLushort indices[] = { 0, 1, 2, 1, 2, 3 };
	glVertexAttribPointer(position_location, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), &load_vectrices[0]);
	glVertexAttribPointer(texture_coord_location, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), &load_vectrices[4]);
	glBindTexture(GL_TEXTURE_2D, *load_texture);
	glUniform1i(sampler_location, 0);
	glUniformMatrix4fv(mvp_location, 1, GL_FALSE, &(matrix[matrix_id])[0][0]);
	glUniform1f(alpha_location, float(alpha / 255.f));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
}

void fr::TextureManager::render(std::string path, float *load_vectrices, int alpha, glm::mat4x4 load_matrix)
{
	GLushort indices[] = { 0, 1, 2, 1, 2, 3 };
	glVertexAttribPointer(position_location, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), &load_vectrices[0]);
	glVertexAttribPointer(texture_coord_location, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), &load_vectrices[4]);
	glBindTexture(GL_TEXTURE_2D, *texture[path]);
	glUniform1i(sampler_location, 0);
	glUniformMatrix4fv(mvp_location, 1, GL_FALSE, &load_matrix[0][0]);
	glUniform1f(alpha_location, float(alpha / 255.f));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
}

void fr::TextureManager::render(GLuint *load_texture, float *load_vectrices, int alpha, glm::mat4x4 load_matrix)
{
	GLushort indices[] = { 0, 1, 2, 1, 2, 3 };
	glVertexAttribPointer(position_location, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), &load_vectrices[0]);
	glVertexAttribPointer(texture_coord_location, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), &load_vectrices[4]);
	glBindTexture(GL_TEXTURE_2D, *load_texture);
	glUniform1i(sampler_location, 0);
	glUniformMatrix4fv(mvp_location, 1, GL_FALSE, &load_matrix[0][0]);
	glUniform1f(alpha_location, float(alpha / 255.f));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
}

fr::TextureCache *fr::TextureManager::CacheText(std::string text, std::string font_path, int font_size, char r, char g, char b, int limited_w, bool wrapper)
{
	int w = 0;
	int current_row_w = 0;
	int h = font_size * 1.4f;
	FT_GlyphSlot slot;
	for (int i = 0; i < text.size(); i++)
	{
		if (text[i] != '\n' || !wrapper)
		{
			FT_Load_Char(*font[font_path][font_size], text[i], FT_LOAD_NO_BITMAP);
			slot = (*font[font_path][font_size])->glyph;
			if (text[i] == '\t')
			{
				current_row_w += font_size * 4;
			}
			else
			{
				current_row_w += slot->advance.x / 64;
			}
			w = current_row_w > w ? current_row_w : w;
		}
		else
		{
			current_row_w = 0;
			h += font_size;
		}
	}

	GLuint *new_texture = new GLuint;
	glGenTextures(1, new_texture);
	glBindTexture(GL_TEXTURE_2D, *new_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, w, h, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);
	//Blank texture;
	FT_Vector pen;
	pen.x = 0;
	pen.y = 0;
	current_row_w = 0;
	w = 0;
	for (int i = 0; i < text.size(); i++)
	{
		if (text[i] == '\n')// && wrapper)
		{
			pen.y -= font_size * 64;
			//千万不要忘记OpenGL的坐标系什么尿性(
			pen.x = 0;
			current_row_w = 0;
		}
		else if (text[i] != '\t')
		{
			FT_Set_Transform(*font[font_path][font_size], NULL, &pen);
			FT_Load_Char(*font[font_path][font_size], text[i], FT_LOAD_RENDER);
			slot = (*font[font_path][font_size])->glyph;
			FT_Bitmap &bitmap = slot->bitmap;
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexSubImage2D(GL_TEXTURE_2D, 0, slot->bitmap_left, font_size - slot->bitmap_top, bitmap.width, bitmap.rows, GL_ALPHA, GL_UNSIGNED_BYTE, bitmap.buffer);
			pen.x += slot->advance.x;
			current_row_w += slot->advance.x / 64;
		}
		else
		{
			pen.x += font_size * 64 * 4;
			current_row_w += font_size * 4;
		}
		w = current_row_w > w ? current_row_w : w;
	}
	TextureCache *output_cache = new TextureCache;
	output_cache->texture = new_texture;
	output_cache->w = w;
	output_cache->h = h;

	return output_cache;
}

glm::mat4x4 fr::TextureManager::GetMatrix(std::string id)
{
	return matrix[id];
}

void fr::TextureManager::DestroyCache(fr::TextureCache *cache)
{
	if (cache)
	{
		glDeleteTextures(1, cache->texture);
	}
	delete cache;
}