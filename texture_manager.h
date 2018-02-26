#ifndef FORCE_RUSH_TEXTURE_MANAGER_H
#define FORCE_RUSH_TEXTURE_MANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
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
		SDL_Texture *texture;
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
			void init(SDL_Renderer *load_renderer);
			void load(std::string path, Rect &output_size);
			void load(SDL_Texture *load_texture, std::string path);
			void loadfont(std::string path, int size);
			void clear();
			void clear(std::string path);
			void clearfont(std::string path, int size);
			void render(std::string path, Rect dest_rect, Rect source_rect = Rect(0, 0, 0, 0), Point center = Point(0, 0), double angle = 0, float scale = 1);
			void render(SDL_Texture *load_texture, Rect dest_rect, Rect source_rect = Rect(0, 0, 0, 0), Point center = Point(0, 0), double angle = 0, float scale = 1);
			TextureCache *CacheText(std::string text, std::string font_path, int font_size, char r, char g, char b, int limited_w = 0, bool wrapper = false, float scale = 1);
			//limit_w = 0 = NO_LIMIT
		private: 
			TextureManager() {}
			~TextureManager() {}
			static TextureManager *m_instance;
			SDL_Renderer *renderer;
			std::map<std::string, SDL_Texture*> texture;
			//storage texture
			std::map<std::string, std::map<int, TTF_Font*> > font;
			//storage font
	};	//class TextureManager
};	//namespace fr

#endif	//FORCE_RUSH_TEXTURE_MANAGER_H