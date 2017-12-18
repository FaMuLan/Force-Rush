#ifndef FORCE_RUSH_CONSTRUCTING_CONSTRUCTING_STATE_H
#define FORCE_RUSH_CONSTRUCTING_CONSTRUCTING_STATE_H

#include <string>
#include <vector>
#include "../state.h"

namespace fr
{
	class Sprite;
	class Button;
	class TextArea;

	class ConstructingState : public State
	{
		public:
			static ConstructingState *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new ConstructingState;
					return m_instance;
				}
				return m_instance;
			}
			void init();
			void clear();
			void update();
		private:
			ConstructingState() {}
			~ConstructingState() {}
			static ConstructingState *m_instance;
			std::vector<TextArea*> dialog_system;
			std::vector<TextArea*> dialog_user;
			int text_index;
			Button *b_back;
			Button *b_communicate;
	};
};

#endif	//FORCE_RUSH_CONSTRUCTING_CONSTRUCTING_STATE_H