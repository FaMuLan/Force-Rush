#ifndef LUNATIC_MELODY_SPRITE_H
#define LUNATIC_MELODY_SPRITE_H

#include <string>

namespace lm
{
	class Sprite
	{
		public:
			virtual void load(std::string path, int x = 0, int y = 0, int w = 0, int h = 0);
			virtual void update();
			virtual void render();
			virtual void clear();

			void SetPos(int x, int y);
			void SetScale(float load_scale);
			int GetX();
			int GetY();
		protected:
			std::string m_path;
			int m_x;
			int m_y;
			int m_w;
			int m_h;
			float scale;
	};	//class Sprite
};	//namespace lm

#endif	//LUNATIC_MELODY_SPRITE_H