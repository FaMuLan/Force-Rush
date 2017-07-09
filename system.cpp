#include "system.h"

lm::System *lm::System::m_instance = 0;

bool lm::System::init()
{
	screen_width = 1280;
	screen_heigh = 720;
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
//	current_state_id = "main";
//	PushState("main", main_state);
	MainState::instance()->init();
	LoadingState::instance()->init();
	current_state = MainState::instance();
}

bool lm::System::run()
{
	while (!ControlHandler::instance()->IsQuit())
	{
		SDL_RenderClear(system_renderer);
//		m_state[current_state_id]->update();
		current_state->update();
		LoadingState::instance()->update();
		SDL_RenderPresent(system_renderer);
		ControlHandler::instance()->update();
	}
}

bool lm::System::clear()
{
	SDL_DestroyRenderer(system_renderer);
	SDL_DestroyWindow(system_window);
	system_renderer = NULL;
	system_window = NULL;
	TTF_Quit();
	IMG_Quit();
}

bool lm::System::PushState(std::string id, State *s)
{

	m_state[id] = s;

}

bool lm::System::SwitchState(std::string id)
{
//	current_state_id = id;
	return true;
}