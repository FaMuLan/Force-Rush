#ifndef LUNATIC_MELODY_TEXTURE_MANAGER_H
#define LUNATIC_MELODY_TEXTURE_MANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
#include <map>

namespace lm
{
	enum TextFormat
	{
		TEXTFORMAT_MIDDLE = 0,
		TEXTFORMAT_LEFT = 1,
		TEXTFORMAT_RIGHT
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
			void load(std::string path);
			void load(SDL_Texture *load_texture, std::string path);
			void loadfont(std::string path, int size);
			void clear();
			void clear(std::string path);
			void clearfont(std::string path);
			void render(std::string path, int x, int y, int w, int h);
			void render(SDL_Texture *load_texture, int x, int y, int w, int h);
			void render(std::string text, int x, int y, std::string font_path, Uint8 r, Uint8 g, Uint8 b, TextFormat format = TEXTFORMAT_MIDDLE);
		private: 
			TextureManager() {}
			~TextureManager() {}
			static TextureManager *m_instance;
			SDL_Renderer *renderer;
			std::map<std::string, SDL_Texture*> texture;
			//storage texture
			std::map<std::string, TTF_Font*> font;
			//storage font
	};	//class TextureManager
};	//namespace lm

#endif	//LUNATIC_MELODY_TEXTURE_MANAGER_H