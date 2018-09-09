#ifndef FORCE_RUSH_TEXTURE_MANAGER_H
#define FORCE_RUSH_TEXTURE_MANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <GLES2/gl2.h>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>
#include <vector>
#include <map>

#include "gui/text_area.h"
#include "shape.h"

namespace fr
{
	class Rect;
	struct TextureCache
	{
		GLuint *texture;
		int w;
		int h;
	};

	class TextureManager
	{
		public:
			static TextureManager *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new TextureManager;
					return m_instance;
				}
				return m_instance;
			}
			void init(GLuint program_object);
			void load(std::string path, Rect &output_size);
			void load(GLuint *load_texture, std::string path);
			void LoadFont(std::string path, int size);
			void LoadMatrix(std::string id, glm::mat4x4 load_matrix);
			void clear();
			void clear(std::string path);
			void ClearFont(std::string path, int size);
			void ClearMatrix(std::string id);
			void update();
			void render(std::string path, float *load_vectrices, int alpha = 255, std::string matrix_id = "default");
			void render(GLuint *load_texture, float *load_vectrices, int alpha = 255, std::string matrix_id = "default");
			void render(std::string path, float *load_vectrices, int alpha, glm::mat4x4 load_matrix);
			void render(GLuint *load_texture, float *load_vectrices, int alpha, glm::mat4x4 load_matrix);
			TextureCache *CacheText(std::string text, std::string font_path, int font_size, char r, char g, char b, int limited_w = 0, bool wrapper = false);
			//limit_w = 0 = NO_LIMIT
			glm::mat4x4 GetMatrix(std::string id);
			void DestroyCache(TextureCache *cache);
		private: 
			TextureManager() {}
			~TextureManager() {}
			static TextureManager *m_instance;
			FT_Library *ft_library;
			std::map<std::string, GLuint*> texture;
			//保守起见用指针来确认是否加载好
			std::map<std::string, glm::mat4x4> matrix;
			std::map<std::string, std::map<int, FT_Face*> > font;
			//存储字体
			GLint position_location;
			GLint texture_coord_location;
			GLint sampler_location;
			GLint mvp_location;
			GLint alpha_location;
	};	//class TextureManager
};	//namespace fr

#endif	//FORCE_RUSH_TEXTURE_MANAGER_H