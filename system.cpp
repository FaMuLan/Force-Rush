#include "system.h"
#include "control_handler.h"
#include "state.h"
#include "main/main_state.h"
#include "select/select_state.h"
#include "loading/loading_state.h"
#include "user/character.h"
#include "texture_manager.h"
#include "sound_manager.h"
#include "message_box.h"

lm::System *lm::System::m_instance = 0;

void lm::System::init()
{
	window_width = 720;
	window_heigh = 1280;
	rotation = WINDOWROTATION_PORTRAIT;
	bg_r = 0xFF;
	bg_g = 0xFF;
	bg_b = 0xFF;

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);
	screen_width = displayMode.w;
	screen_heigh = displayMode.h;
	//for fullscreen
	system_window = SDL_CreateWindow("Lunatic Melody", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_heigh, SDL_WINDOW_SHOWN);
	system_renderer = SDL_CreateRenderer(system_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	TTF_Init();
	IMG_Init(0);
	TextureManager::instance()->init(system_renderer);
	SoundManager::instance()->init();
	ControlHandler::instance()->init();
	Character::instance()->init();
	MessageBox::instance()->init();
//	current_state_id = "main";
//	PushState("main", main_state);
	MainState::instance()->init();
	LoadingState::instance()->init();
	current_state = MainState::instance();
	scale = float(screen_width) / float(window_width);
	window_heigh = screen_heigh / scale;
	rotation = window_width > window_heigh ? WINDOWROTATION_LANDSCAPE : WINDOWROTATION_PORTRAIT;
	//将理论分辨率的宽高比例与实际屏幕分辨率的宽高比例吻合
}

void lm::System::run()
{
	while (!ControlHandler::instance()->IsQuit())
	{
		is_window_modified = false;
		ControlHandler::instance()->update();
		SDL_SetRenderDrawColor(system_renderer, bg_r, bg_g, bg_b, 0xFF);
		SDL_RenderClear(system_renderer);
//		m_state[current_state_id]->update();
		current_state->update();
		LoadingState::instance()->update();
		MessageBox::instance()->update();
		MessageBox::instance()->render();
//		ControlHandler::instance()->render();
		SDL_RenderPresent(system_renderer);
	}
}

void lm::System::clear()
{
	ControlHandler::instance()->clear();
	TextureManager::instance()->clear();
	SDL_DestroyRenderer(system_renderer);
	SDL_DestroyWindow(system_window);
	system_renderer = NULL;
	system_window = NULL;
	TTF_Quit();
	IMG_Quit();
}

void lm::System::RefreshWindowSize()
{
	int w, h;
	SDL_GetWindowSize(system_window, &w, &h);
	screen_width = w;
	screen_heigh = h;
	rotation = window_width > window_heigh ? WINDOWROTATION_LANDSCAPE : WINDOWROTATION_PORTRAIT;
	window_width = rotation == WINDOWROTATION_PORTRAIT ? 720 : 1280;
	//屏幕縮放以720p 16:9為基準(也就是我手機屏幕的分辨率了)
	//是720 * 1280的分辨率
	scale = float(screen_width) / float(window_width);
	window_heigh = screen_heigh / scale;
	is_window_modified = true;
}

bool lm::System::IsWindowModified()
{
	return is_window_modified;
}

void lm::System::SetBackgroundColor(char r, char g, char b)
{
	bg_r = r;
	bg_g = g;
	bg_b = b;
}

int lm::System::GetWindowWidth()
{
	return window_width;
}
int lm::System::GetWindowHeigh()
{
	return window_heigh;
}
int lm::System::GetScreenWidth()
{
	return screen_width;
}
int lm::System::GetScreenHeigh()
{
	return screen_heigh;
}
float lm::System::GetScale()
{
	return scale;
}
lm::WindowRotation lm::System::GetWindowRotation()
{
	return rotation;
}