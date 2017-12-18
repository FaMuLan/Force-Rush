#ifndef FORCE_RUSH_STATE_H
#define FORCE_RUSH_STATE_H

#include <SDL2/SDL.h>

namespace fr
{
	class State
	{
		public:
			virtual void update() = 0;

			virtual void init() = 0;
			virtual void clear() = 0;
	};
};

#endif	//FORCE_RUSH_STATE_H