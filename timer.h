#ifndef LUNATIC_MELODY_TIMER_H
#define LUNAITC_MELODY_TIMER_H

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

			Uint32 GetTime(std::string id);
		private:
			Timer() {}
			~Timer() {}
			std::map<std::string, Uint32> start_time;
			std::map<std::string, Uint32> pause_time;
			std::map<std::string, bool> is_start;
			std::map<std::string, bool> is_pause;
			static Timer *m_instance;
	};	//class Timer
};	//namespace lm

#endif	//LUNATIC_MELODY_TIMER_H