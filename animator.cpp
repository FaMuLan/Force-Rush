#include "animator.h"
#include "timer.h"

lm::Animator *lm::Animator::m_instance = 0;

void lm::Animator::AddAnimation(std::string id, AnimationType type, int duration)
{
	Animation *new_animation = new Animation;
	new_animation->type = type;
	new_animation->duration = duration;
	m_anim[id] = new_animation;
}

void lm::Animator::Animate(std::string id)
{
	Timer::instance()->RunTimer(std::string("animation_") + id);
}

void lm::Animator::ResetAnimation(std::string id)
{
	Timer::instance()->ResetTimer(std::string("animation_") + id);
}

float lm::Animator::GetProcess(std::string id)
{
	float output = 0;
	if (!IsTimeUp(id))
	{
		switch (m_anim[id]->type)
		{
			case ANIMATIONTYPE_UNIFORMLY_ACCELERATED:
				output = float(Timer::instance()->GetTime(std::string("animation_") + id) * Timer::instance()->GetTime(std::string("animation_") + id)) / float(m_anim[id]->duration * m_anim[id]->duration);
			break;
			case ANIMATIONTYPE_UNIFORMLY_DECELERATED:
				/*
				a = 2 / T^2
				s = 2 / T * t + T^2 * (t ^ 2)
				  = (2 * 2 / T - 2 / T^2 * t) / 2 * t
				vt = v0 - at = 0
				v0 = aT
				s = (2 * (2 * straight / T^2) * T - (straight / T^2) * t) / 2 * t
				  = (4 * straight / T - straight / T^2 * t) / 2 * t
				  = (2 * straight / T - 0.5 * straight / T^2 *t) * t
				*/
				output = (2.f / m_anim[id]->duration - 1.f / m_anim[id]->duration / m_anim[id]->duration * Timer::instance()->GetTime(std::string("animation_") + id)) * float(Timer::instance()->GetTime(std::string("animation_") + id));
//				output =  float(Timer::instance()->GetTime(std::string("animation_") + id) * Timer::instance()->GetTime(std::string("animation_") + id)) / float(m_anim[id]->duration * m_anim[id]->duration);
			break;
		}
	}
	else
	{
		output = 1;
	}
	return output;
}

bool lm::Animator::IsTimeUp(std::string id)
{
	return Timer::instance()->GetTime(std::string("animation_") + id) > m_anim[id]->duration;
}