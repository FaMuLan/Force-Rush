#ifndef LUNATIC_MELODY_CRAFTING_CRAFTING_STATE_H
#define LUNATIC_MELODY_CRAFTING_CRAFTING_STATE_H

#include <string>
#include "../state.h"

namespace lm
{
	class Sprite;
	class Button;

	class CraftingState : public State
	{
		public:
			static CraftingState *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new CraftingState;
					return m_instance;
				}
				return m_instance;
			}
			void init();
			void clear();
			void update();
		private:
			CraftingState() {}
			~CraftingState() {}
			static CraftingState *m_instance;
			Button *crafting_back;
			Sprite *background;
	};
};

#endif	//LUNATIC_MELODY_CRAFTING_CRAFTING_STATE_H