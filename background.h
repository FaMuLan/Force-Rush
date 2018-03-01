#ifndef FORCE_RUSH_BACKGROUND_H
#define FORCE_RUSH_BACKGROUND_H

namespace fr
{
	class Sprite;

	class Background
	{
		public:
			static Background *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new Background;
					return m_instance;
				}
				return m_instance;
			}
			void init();
			void clear();
			void render();
		private:
			Background() {}
			~Background() {}
			static Background *m_instance;
			Sprite *background_arrow_portrait;
			Sprite *background_arrow_landscape;
	};
};

#endif