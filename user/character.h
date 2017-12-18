#ifndef FORCE_RUSH_CHARACTER_H
#define FORCE_RUSH_CHARACTER_H

namespace fr
{
	class Sprite;

	class Character
	{
		public:
			static Character *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new Character;
					return m_instance;
				}
				return m_instance;
			}
			void init();
			void clear();
			void render();
		private:
			Character() {}
			~Character() {}
			static Character *m_instance;
			Sprite *m_char;
			char bg_r, bg_g, bg_b;
	};
};

#endif