#ifndef LUNATIC_MELODY_STATE_H
#define LUNATIC_MELODY_STATE_H

#include <SDL2/SDL.h>

namespace lm
{
	class State
	{
		public:
			virtual void update() = 0;

			virtual void init() = 0;
			virtual void clear() = 0;
	};
};

#endif	//LUNATIC_MELODY_STATE_H