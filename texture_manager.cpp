#include "texture_manager.h"
#include "system.h"

fr::TextureManager *fr::TextureManager::m_instance = 0;

void fr::TextureManager::init(SDL_Renderer *load_renderer)
{
	renderer = load_renderer;
}

void fr::TextureManager::load(std::string path, int &output_w, int &output_h)
{
	SDL_Surface *load_surface = IMG_Load(path.c_str());
	SDL_Texture *new_texture = SDL_CreateTextureFromSurface(renderer, load_surface);
	texture[path] = new_texture;
	output_w = load_surface->w;
	output_h = load_surface->h;
	SDL_FreeSurface(load_surface);
}

void fr::TextureManager::load(SDL_Texture *load_texture, std::string path)
{
	texture[path] = load_texture;
}

void fr::TextureManager::loadfont(std::string path, int size)
{
	TTF_Font *new_font = TTF_OpenFont(path.c_str(), size);
	font[path][size] = new_font;
}

void fr::TextureManager::clear()
{
	texture.clear();
}

void fr::TextureManager::clear(std::string path)
{
	SDL_DestroyTexture(texture[path]);
	texture[path] = NULL;
}

void fr::TextureManager::clearfont(std::string path, int size)
{
	TTF_CloseFont(font[path][size]);
	font[path][size] = NULL;
}

void fr::TextureManager::render(std::string path, int x, int y, int w, int h, int src_x, int src_y, int src_w, int src_h)
{
	SDL_Rect dest_rect = { int(x * System::instance()->GetScale()), int(y * System::instance()->GetScale()), int(w * System::instance()->GetScale()), int(h * System::instance()->GetScale()) };
	if (src_w == 0 && src_h == 0)
	{
		SDL_RenderCopy(renderer, texture[path], NULL, &dest_rect);
	}
	else
	{
		SDL_Rect src_rect = { src_x, src_y, src_w, src_h };
		SDL_RenderCopy(renderer, texture[path], &src_rect, &dest_rect);
	}
}

void fr::TextureManager::render(SDL_Texture *load_texture, int x, int y, int w, int h)
{
	SDL_Rect dest_rect = { x, y, w, h };
	SDL_RenderCopy(renderer, load_texture, NULL, &dest_rect);
}

void fr::TextureManager::render(std::string text, int x, int y, std::string font_path, int font_size, char r, char g, char b, TextFormat format, int limited_w, float scale)
{
	SDL_Color color = { char(r), char(g), char(b) };
	SDL_Surface *text_surface = TTF_RenderUTF8_Blended(font[font_path][font_size], text.c_str(), color);
	SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	int w = text_surface->w;
	int h = text_surface->h * scale;
	w = (limited_w == 0 || limited_w > w) ? w * scale : limited_w * scale;
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
	SDL_Rect dest_rect = { int(x * System::instance()->GetScale()), int(y * System::instance()->GetScale()), int(w * System::instance()->GetScale()), int(h * System::instance()->GetScale()) };
	SDL_RenderCopy(renderer, text_texture, NULL, &dest_rect);

	SDL_DestroyTexture(text_texture);
	SDL_FreeSurface(text_surface);
}