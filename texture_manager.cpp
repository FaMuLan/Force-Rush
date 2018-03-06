#include "texture_manager.h"
#include "system.h"
#include "shape.h"

fr::TextureManager *fr::TextureManager::m_instance = 0;

void fr::TextureManager::init(SDL_Renderer *load_renderer)
{
	renderer = load_renderer;
}

void fr::TextureManager::load(std::string path, Rect &output_size)
{
	SDL_Surface *load_surface = IMG_Load(path.c_str());
	if (!texture[path])
	{
		SDL_Texture *new_texture = SDL_CreateTextureFromSurface(renderer, load_surface);
		texture[path] = new_texture;
	}
	output_size.w = load_surface->w;
	output_size.h = load_surface->h;
	SDL_FreeSurface(load_surface);
}

void fr::TextureManager::load(SDL_Texture *load_texture, std::string path)
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
	SDL_DestroyTexture(texture[path]);
	texture[path] = NULL;
}

void fr::TextureManager::clearfont(std::string path, int size)
{
	TTF_CloseFont(font[path][size]);
	font[path][size] = NULL;
}

void fr::TextureManager::render(std::string path, Rect dest_rect, Rect source_rect, Point center, double angle, float scale)
{
	SDL_Rect sdl_dest_rect = { int(dest_rect.x * System::instance()->GetScale()), int(dest_rect.y * System::instance()->GetScale()), int(dest_rect.w * System::instance()->GetScale() * scale), int(dest_rect.h * System::instance()->GetScale() * scale) };
	SDL_Point sdl_center = { center.x, center.y };
	if (source_rect.w == 0 && source_rect.h == 0)
	{
		SDL_RenderCopyEx(renderer, texture[path], NULL, &sdl_dest_rect, angle, &sdl_center, SDL_FLIP_NONE);
	}
	else
	{
		SDL_Rect sdl_source_rect = { source_rect.x, source_rect.y, source_rect.w, source_rect.h };
		SDL_RenderCopyEx(renderer, texture[path], &sdl_source_rect, &sdl_dest_rect, angle, &sdl_center, SDL_FLIP_NONE);
	}
}

void fr::TextureManager::render(SDL_Texture *load_texture, Rect dest_rect, Rect source_rect, Point center, double angle, float scale)
{
	SDL_Rect sdl_dest_rect = { int(dest_rect.x * System::instance()->GetScale()), int(dest_rect.y * System::instance()->GetScale()), int(dest_rect.w * System::instance()->GetScale() * scale), int(dest_rect.h * System::instance()->GetScale() * scale) };
	SDL_Point sdl_center = { center.x, center.y };
	if (source_rect.w == 0 && source_rect.h == 0)
	{
		SDL_RenderCopyEx(renderer, load_texture, NULL, &sdl_dest_rect, angle, &sdl_center, SDL_FLIP_NONE);
	}
	else
	{
		SDL_Rect sdl_source_rect = { source_rect.x, source_rect.y, source_rect.w, source_rect.h };
		SDL_RenderCopyEx(renderer, load_texture, &sdl_source_rect, &sdl_dest_rect, angle, &sdl_center, SDL_FLIP_NONE);
	}
}

fr::TextureCache *fr::TextureManager::CacheText(std::string text, std::string font_path, int font_size, char r, char g, char b, int limited_w, bool wrapper, float scale)
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
	SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	int w = text_surface->w;
	int h = text_surface->h * scale;
	w = (limited_w == 0 || limited_w > w) ? w * scale : limited_w * scale;
	TextureCache *output_cache = new TextureCache;
	output_cache->texture = text_texture;
	output_cache->w = w;
	output_cache->h = h;

	SDL_FreeSurface(text_surface);
	return output_cache;
}