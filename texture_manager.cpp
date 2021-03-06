#include "texture_manager.h"
#include <glm/ext.hpp>
#include <png.h>
#include <SDL2/SDL_ttf.h>
#include "system.h"
#include "user/setting.h"
#include "data.h"

fr::TextureManager *fr::TextureManager::m_instance = 0;

void fr::TextureManager::init(GLuint program_object)
{
	TTF_Init();
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
//	SDL_Surface *load_surface = IMG_Load(path.c_str());
	if (!texture[path])
	{
//		SDL_Surface *converted_surface = SDL_ConvertSurfaceFormat(load_surface, SDL_PIXELFORMAT_ABGR8888, 0);
		png_structp png_ptr = NULL;
		png_infop info_ptr = NULL;
		unsigned int width, heigh;
		unsigned char signal[8];
		unsigned char *pixel;
		int bit_depth;
		int color_type;
		unsigned int row_byte;
		png_bytepp row_pointer;
		FILE *file_ptr = fopen(path.c_str(), "rb");
		fread(signal, sizeof(*signal), sizeof(signal), file_ptr);
		if (!png_check_sig(signal, 8))
		{
			fclose(file_ptr);
			exit(0);
		}
		png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		info_ptr = png_create_info_struct(png_ptr);
		if (setjmp(png_jmpbuf(png_ptr)))
		{
			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
			fclose(file_ptr);
			exit(0);
		}
		png_init_io(png_ptr, file_ptr);
		png_set_sig_bytes(png_ptr, 8);
		png_read_info(png_ptr, info_ptr);
		png_get_IHDR(png_ptr, info_ptr, &width, &heigh, &bit_depth, &color_type, NULL, NULL, NULL);
		if (color_type == PNG_COLOR_TYPE_PALETTE)
		{
			png_set_palette_to_rgb(png_ptr);
		}
		if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		{
			png_set_expand_gray_1_2_4_to_8(png_ptr);
		}
		if (bit_depth == 16)
		{
			png_set_strip_16(png_ptr);
		}
		if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		{
			png_set_tRNS_to_alpha(png_ptr);
		}
		if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		{
			png_set_gray_to_rgb(png_ptr);
		}
		png_read_update_info(png_ptr, info_ptr);

		row_byte = png_get_rowbytes(png_ptr, info_ptr);
		pixel = new unsigned char[row_byte * heigh];
		row_pointer = new png_bytep[heigh];
		for (unsigned int i = 0; i < heigh; i++)
		{
			row_pointer[i] = pixel + i * row_byte;
		}
		png_read_image(png_ptr, row_pointer);

		GLuint *new_texture = new GLuint;
		glGenTextures(1, new_texture);
		glBindTexture(GL_TEXTURE_2D, *new_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, heigh, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		TextureCache *new_texture_cache = new TextureCache;
		new_texture_cache->texture = new_texture;
		new_texture_cache->w = width;
		new_texture_cache->h = heigh;
		texture[path] = new_texture_cache;
		delete [] row_pointer;
		delete [] pixel;
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(file_ptr);
	}
	output_size.w = texture[path]->w;
	output_size.h = texture[path]->h;
//	SDL_FreeSurface(load_surface);
}

void fr::TextureManager::load(GLuint *load_texture, std::string path)
{
	texture[path]->texture = load_texture;
}

void fr::TextureManager::LoadFont(std::string path, int size)
{
	if (!font[path][size])
	{
		TTF_Font *new_font = TTF_OpenFont(path.c_str(), size);
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
		glDeleteTextures(1, texture[path]->texture);
		delete texture[path];
		texture[path] = NULL;
	}
}

void fr::TextureManager::ClearFont(std::string path, int size)
{
	TTF_CloseFont(font[path][size]);
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
	glBindTexture(GL_TEXTURE_2D, *texture[path]->texture);
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
	glBindTexture(GL_TEXTURE_2D, *texture[path]->texture);
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

fr::TextureCache *fr::TextureManager::CacheText(std::string text, std::string font_path, int font_size, Color input_color, int limited_w, bool input_is_wrapped)
{
	SDL_Color color = { char(input_color.r), char(input_color.g), char(input_color.b) };
	SDL_Surface *text_surface;
	if (input_is_wrapped)
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
			else if (load_w > limited_w && limited_w != 0)
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
	//*4是四个颜色，既然是空白，那么RBGA四个数可以都是0
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, converted_surface->w, converted_surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, converted_surface->pixels);
	TextureCache *output_cache = new TextureCache;
	output_cache->texture = new_texture;
	output_cache->w = text_surface->w;
	output_cache->h = text_surface->h;

	SDL_FreeSurface(converted_surface);
	SDL_FreeSurface(text_surface);
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
		delete cache->texture;
	}
	delete cache;
}