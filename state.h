#ifndef FORCE_RUSH_STATE_H
#define FORCE_RUSH_STATE_H

#include <SDL2/SDL.h>

namespace fr
{
	enum StateID
	{
		STATE_MAIN,
		STATE_PREPARE,
		STATE_GAME,
		STATE_RESULT,
		STATE_GAMEPLAY_WIZARD,
		STATE_ABOUT
	};

	class State
	{
		public:
			virtual void update() = 0;

			virtual void init() = 0;
			virtual void clear() = 0;
	};
};

#endif	//FORCE_RUSH_STATE_H