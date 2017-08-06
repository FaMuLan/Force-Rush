#include "system.h"
#include "control_handler.h"
#include "state.h"
#include "main/main_state.h"
#include "crafting/crafting_state.h"
#include "debug_widget.h"
#include "select/select_state.h"
#include "loading/loading_state.h"
#include "texture_manager.h"
#include "sound_manager.h"

lm::System *lm::System::m_instance = 0;

void lm::System::init()
{
	window_width = 1280;
	window_heigh = 720;
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
//	current_state_id = "main";
//	PushState("main", main_state);
	MainState::instance()->init();
	LoadingState::instance()->init();
	DebugWidget::instance()->init();
	current_state = MainState::instance();
	scale = float(screen_width) / float(window_width);
}

void lm::System::run()
{
	while (!ControlHandler::instance()->IsQuit())
	{
		ControlHandler::instance()->update();
		SDL_RenderClear(system_renderer);
//		m_state[current_state_id]->update();
		current_state->update();
		LoadingState::instance()->update();
		DebugWidget::instance()->update();
		ControlHandler::instance()->render();
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