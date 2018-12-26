#ifndef FORCE_RUSH_SPRITE_H
#define FORCE_RUSH_SPRITE_H

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "data.h"

namespace fr
{
	class Sprite
	{
		public:
			virtual void init(std::string path = "", Rect input_dest_rect = Rect(0, 0, 0, 0), Rect input_source_rect = Rect(0, 0, 0, 0));
			virtual void update();
			virtual void render();
			virtual void render(int index);
			virtual void clear();

			void AddFrame(std::string input_path = "");
			void SetAnimate(int input_start, int input_end, int input_duration);
			int GetCurrentIndex();
			void SetBaseFrame(int index);
			void SetPos(int input_x, int input_y);
			void SetScale(float input_scale);
			void SetSize(int input_w, int input_h);
			void SetSrcRect(Rect input_source_rect);
			void SetAlpha(int input_alpha);
			void SetVectrices(float *input_vectrices);
			void SetMatrix(std::string id);
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
			int alpha;
			float *vectrices;
			glm::vec3 pos_3d;
			std::string matrix_id;
	};	//class Sprite
};	//namespace fr

#endif	//FORCE_RUSH_SPRITE_H