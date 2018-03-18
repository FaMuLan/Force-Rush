#ifndef FORCE_RUSH_SPRITE_H
#define FORCE_RUSH_SPRITE_H

#include <string>
#include <vector>
#include "shape.h"

namespace fr
{
	class Sprite
	{
		public:
			virtual void init(std::string path = "", Rect load_dest_rect = Rect(0, 0, 0, 0), Rect load_source_rect = Rect(0, 0, 0, 0), Point load_center = Point(0, 0), double load_angle = 0);
			virtual void update();
			virtual void render();
			virtual void render(int index);
			virtual void clear();

			void AddFrame(std::string path = "");
			void SetAnimate(int load_start, int load_end, int load_duration);
			int GetCurrentIndex();
			void SetBaseFrame(int index);
			void SetPos(int x, int y);
			void SetPos(int x, int y, int z);
			void SetScale(float load_scale);
			void SetSize(int w, int h);
			void SetSrcRect(Rect load_source_rect);
			void SetRotation(Point load_center, double load_angle);
			void SetVectrices(int *load_vectrices);
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
			Rect dest_rect;
			Rect texture_size;
			Rect source_rect;
			float scale;
			Point center;
			double angle;
			float *vectrices;
	};	//class Sprite
};	//namespace fr

#endif	//FORCE_RUSH_SPRITE_H