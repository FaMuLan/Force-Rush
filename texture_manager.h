#ifndef FORCE_RUSH_TEXTURE_MANAGER_H
#define FORCE_RUSH_TEXTURE_MANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <GLES2/gl2.h>
#include <string>
#include <vector>
#include <map>

#include "text_area.h"
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
			void loadfont(std::string path, int size);
			void clear();
			void clear(std::string path);
			void clearfont(std::string path, int size);
			void update();
			void render(std::string path, Rect dest_rect, Rect texture_size, Rect source_rect = Rect(0, 0, 0, 0), Point center = Point(0, 0), double angle = 0, float scale = 1);
			void render(GLuint *texture, Rect dest_rect, Rect texture_size, Rect source_rect = Rect(0, 0, 0, 0), Point center = Point(0, 0), double angle = 0, float scale = 1);
			TextureCache *CacheText(std::string text, std::string font_path, int font_size, char r, char g, char b, int limited_w = 0, bool wrapper = false);
			//limit_w = 0 = NO_LIMIT
			void DestroyCache(TextureCache *cache);
		private: 
			TextureManager() {}
			~TextureManager() {}
			static TextureManager *m_instance;
			std::map<std::string, GLuint*> texture;
			//保守起见用指针来确认是否加载好
			std::map<std::string, std::map<int, TTF_Font*> > font;
			//存储字体
			GLint position_location;
			GLint texture_coord_location;
			GLint sampler_location;
			GLint mvp_location;
			Matrix *null_matrix;
	};	//class TextureManager
};	//namespace fr

#endif	//FORCE_RUSH_TEXTURE_MANAGER_H