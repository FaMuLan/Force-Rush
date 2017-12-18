#ifndef FORCE_RUSH_TIMER_H
#define FORCE_RUSH_TIMER_H

#include <SDL2/SDL.h>
#include <string>
#include <map>

namespace fr
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
};	//namespace fr

#endif	//FORCE_RUSH_TIMER_H