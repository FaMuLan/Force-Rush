#ifndef LUNATIC_MELODY_ANIMATOR_H
#define LUNATIC_MELODY_ANIMATOR_H

#include <string>
#include <map>

namespace lm
{
	enum AnimationType
	{
		ANIMATIONTYPE_UNIFORMLY_ACCELERATED,
		ANIMATIONTYPE_UNIFORMLY_DECELERATED,
	};

	struct Animation
	{
		AnimationType type;
		int duration;
	};

	class Animator
	{
		public:
			static Animator *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new Animator;
					return m_instance;
				}
				return m_instance;
			}
			void AddAnimation(std::string id, AnimationType type, int duration);
			void Animate(std::string id);
			void ResetAnimation(std::string id);

			float GetProcess(std::string id);
			bool IsTimeUp(std::string id);
		private:
			Animator() {}
			~Animator() {}
			static Animator *m_instance;
			std::map<std::string, Animation*> m_anim;
	};
};

#endif	//LUNATIC_MELODY_ANIMATOR