#ifndef LUNATIC_MELODY_TEXTURE_MANAGER_H
#define LUNATIC_MELODY_TEXTURE_MANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
#include <map>

#include "text_area.h"

namespace lm
{
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
			void load(std::string path, int &output_w, int &output_h);
			void load(SDL_Texture *load_texture, std::string path);
			void loadfont(std::string path, int size);
			void clear();
			void clear(std::string path);
			void clearfont(std::string path, int size);
			void render(std::string path, int x, int y, int w, int h, int src_x = 0, int src_y = 0, int src_w = 0, int src_h = 0);
			void render(SDL_Texture *load_texture, int x, int y, int w, int h);
			void render(std::string text, int x, int y, std::string font_path, int font_size, char r, char g, char b, TextFormat format = TEXTFORMAT_MIDDLE, int limited_w = 0);
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
};	//namespace lm

#endif	//LUNATIC_MELODY_TEXTURE_MANAGER_H