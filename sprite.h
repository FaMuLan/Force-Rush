#ifndef FORCE_RUSH_SPRITE_H
#define FORCE_RUSH_SPRITE_H

#include <string>
#include <vector>

namespace fr
{
	class Sprite
	{
		public:
			virtual void init(std::string path = "", int x = 0, int y = 0, int w = 0, int h = 0);
			virtual void update();
			virtual void render();
			virtual void render(int index);
			virtual void clear();

			void AddFrame(std::string path = "");
			void SetAnimate(int load_start, int load_end, int load_duration);
			int GetCurrentIndex();
			//
			void SetBaseFrame(int index);
			void SetPos(int x, int y);
			void SetScale(float load_scale);
			void SetSize(int w, int h);
			int GetX();
			int GetY();
			int GetW();
			int GetH();
		protected:
			int base_index;
			int current_index;
			std::vector<std::string> frame;
			unsigned long start_time;
			int duration;
			int frame_start;
			int frame_end;
			int m_x;
			int m_y;
			int m_w;
			int m_h;
			float scale;
	};	//class Sprite
};	//namespace fr

#endif	//FORCE_RUSH_SPRITE_H