#ifndef LUNATIC_MELODY_TIMER_H
#define LUNATIC_MELODY_TIMER_H

#include <SDL2/SDL.h>
#include <string>
#include <map>

namespace lm
{
	class Timer
	{
		public:
			static Timer *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new Timer;
					return m_instance;
				}
				return m_instance;
			}
			void ResetTimer(std::string id);
			void RunTimer(std::string id);
			void PauseTimer(std::string id);

			unsigned long GetTime(std::string id);
			unsigned long GetSystemTime();
		private:
			Timer() {}
			~Timer() {}
			std::map<std::string, unsigned long> start_time;
			std::map<std::string, unsigned long> pause_time;
			std::map<std::string, bool> is_start;
			std::map<std::string, bool> is_pause;
			static Timer *m_instance;
	};	//class Timer
};	//namespace lm

#endif	//LUNATIC_MELODY_TIMER_H