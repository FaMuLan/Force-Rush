#ifndef FORCE_RUSH_LOADING_LOADING_STATE_H
#define FORCE_RUSH_LOADING_LOADING_STATE_H

#include <string>
#include <vector>
#include "../state.h"

namespace fr
{
	class Sprite;
	class TextArea;

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
			void render();
			void init();
			void init(StateID next_state_id);
			void clear();
			void OnEnter();
			void OnExit();
			static void OnLoad();	//要扔到其他線程裡面
			bool IsLoading();
//			void AddText(std::string text, std::string font_path, int font_size, int x, int y, char r, char g, char b, ShutterType type);
			bool IsSwitching();
		private:
			LoadingState() {}
			~LoadingState() {}
			static LoadingState *m_instance;
			Sprite *shutter_middle_portrait;
			Sprite *shutter_middle_landscape;
			Sprite *shutter_left_portrait;
			Sprite *shutter_top_landscape;
			Sprite *shutter_right_portrait;
			Sprite *shutter_bottom_landscape;
			int background_length;
			bool is_entered;
			static bool is_loaded;
			bool is_exited;
			bool is_text_default;
			static State *next_state;
			static State *last_state;
	};
};

#endif	//FORCE_RUSH_LOADING_LOADING_STATE_H