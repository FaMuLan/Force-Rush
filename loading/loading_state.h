#ifndef FORCE_RUSH_LOADING_LOADING_STATE_H
#define FORCE_RUSH_LOADING_LOADING_STATE_H

#include <string>
#include <vector>
#include "../state.h"

namespace fr
{
	class Sprite;
	class TextArea;

	enum ShutterType
	{
		SHUTTER_TOP = 0,
		SHUTTER_BOTTOM,
		SHUTTER_LEFT,
		SHUTTER_RIGHT
	};

	class LoadingState : public State
	{
		public:
			static LoadingState *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new LoadingState;
					return m_instance;
				}
				return m_instance;
			}
			void update();
			void init();
			void init(State *load_next_state, State *load_last_state);
			void clear();
			void OnEnter();
			void OnExit();
			void AddText(std::string text, std::string font_path, int font_size, int x, int y, char r, char g, char b, ShutterType type);
			bool IsSwitching();
		private:
			LoadingState() {}
			~LoadingState() {}
			static LoadingState *m_instance;
			Sprite *shutter_top;
			Sprite *shutter_bottom;
			Sprite *shutter_left;
			Sprite *shutter_right;
			Sprite *shutter_center;
			std::vector<TextArea*> text_area_top;
			std::vector<TextArea*> text_area_bottom;
			std::vector<TextArea*> text_area_left;
			std::vector<TextArea*> text_area_right;
			bool is_entered;
			bool is_loaded;
			bool is_exited;
			bool is_text_default;
			State* next_state;
			State* last_state;
	};
};

#endif	//FORCE_RUSH_LOADING_LOADING_STATE_H