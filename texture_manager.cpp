#include "texture_manager.h"

lm::TextureManager *lm::TextureManager::m_instance = 0;

void lm::TextureManager::init(SDL_Renderer *load_renderer)
{
	renderer = load_renderer;
}

void lm::TextureManager::load(std::string path)
{
	SDL_Surface *load_surface = IMG_Load(path.c_str());
	SDL_Texture *new_texture = SDL_CreateTextureFromSurface(renderer, load_surface);
	texture[path] = new_texture;
	SDL_FreeSurface(load_surface);
}

void lm::TextureManager::load(SDL_Texture *load_texture, std::string path)
{
	texture[path] = load_texture;
}

void lm::TextureManager::loadfont(std::string path, int size)
{
	TTF_Font *new_font = TTF_OpenFont(path.c_str(), size);
	font[path] = new_font;
}

void lm::TextureManager::clear()
{
	texture.clear();
}

void lm::TextureManager::clear(std::string path)
{
	SDL_DestroyTexture(texture[path]);
	texture[path] = NULL;
}

void lm::TextureManager::clearfont(std::string path)
{
	TTF_CloseFont(font[path]);
	font[path] = NULL;
}

void lm::TextureManager::render(std::string path, int x, int y, int w, int h)
{
	SDL_Rect dest_rect = { x, y, w, h };
	SDL_RenderCopy(renderer, texture[path], NULL, &dest_rect);
}

void lm::TextureManager::render(SDL_Texture *load_texture, int x, int y, int w, int h)
{
	SDL_Rect dest_rect = { x, y, w, h };
	SDL_RenderCopy(renderer, load_texture, NULL, &dest_rect);
}

void lm::TextureManager::render(std::string text, int x, int y, std::string font_path, Uint8 r, Uint8 g, Uint8 b, TextFormat format)
{
	SDL_Color color = { r, g, b };
	SDL_Surface *text_surface = TTF_RenderUTF8_Blended(font[font_path], text.c_str(), color);
	SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	int w = text_surface->w;
	int h = text_surface->h;
	switch (format)
	{
		case TEXTFORMAT_MIDDLE:
			x -= w / 2;
			y -= h / 2;
		break;
		case TEXTFORMAT_RIGHT:
			x -= w;
			y -= h;
		break;
	}
	SDL_Rect dest_rect = { x, y, w, h };
	SDL_RenderCopy(renderer, text_texture, NULL, &dest_rect);

	SDL_DestroyTexture(text_texture);
	SDL_FreeSurface(text_surface);
}