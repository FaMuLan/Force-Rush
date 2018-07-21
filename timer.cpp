#include "timer.h"

fr::Timer *fr::Timer::m_instance = 0;

void fr::Timer::ResetTimer(std::string id)
{
	is_start[id] = false;
	is_pause[id] = false;
}

void fr::Timer::RunTimer(std::string id)
{
	if (is_pause[id])
	{
		start_time[id] = SDL_GetTicks() - (pause_time[id] - start_time[id]);
		is_pause[id] = false;
	}
	if (!is_start[id])
	{
		start_time[id] = SDL_GetTicks();
		pause_time[id] = SDL_GetTicks();
		is_start[id] = true;
	}
}

void fr::Timer::PauseTimer(std::string id)
{
	pause_time[id] = SDL_GetTicks();
	is_pause[id] = true;
}

unsigned long fr::Timer::GetTime(std::string id)
{
	Uint32 output = 0;
	if (is_start[id])
	{
		if (is_pause[id])
		{
			output = pause_time[id] - start_time[id];
		}
		else
		{
			output = SDL_GetTicks() - start_time[id];
		}
	}

	return output;
}

unsigned long fr::Timer::GetSystemTime()
{
	return SDL_GetTicks();
}

bool fr::Timer::IsPaused(std::string id)
{
	return is_pause[id];
}