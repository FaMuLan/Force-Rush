#include "timer.h"

lm::Timer *lm::Timer::m_instance = 0;

void lm::Timer::ResetTimer(std::string id)
{
	is_start[id] = false;
	is_pause[id] = false;
}

void lm::Timer::RunTimer(std::string id)
{
	if (is_pause[id])
	{
		start_time[id] += SDL_GetTicks() - pause_time[id];
		is_pause[id] = false;
	}
	if (!is_start[id])
	{
		start_time[id] = SDL_GetTicks();
		pause_time[id] = SDL_GetTicks();
		is_start[id] = true;
	}
}

inline void lm::Timer::PauseTimer(std::string id)
{
	pause_time[id] = SDL_GetTicks();
	is_pause[id] = true;
}

Uint32 lm::Timer::GetTime(std::string id)
{
	Uint32 output = 0;
	if (is_start[id])
	{
		if (is_pause[id])
		{
			output = start_time[id] - pause_time[id];
		}
		else
		{
			output = SDL_GetTicks() - start_time[id];
		}
	}

	return output;
}