#ifndef LUNATIC_MELODY_SOUND_MANAGER_H
#define LUNATIC_MELODY_SOUND_MANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <map>
#include <vector>

namespace lm
{
	enum SoundType
	{
		SOUNDTYPE_MUSIC = 0,
		SOUNDTYPE_SFX
	};

	class SoundManager
	{
		public:
			static SoundManager *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new SoundManager;
				}
				return m_instance;
			}
			void init();
			void clear();
			void load(std::string path, SoundType type);
			void play(std::string path, SoundType type);
			void stop();	//music type only
			void SetVolume(Uint16 load_volume, SoundType type);
		private:
			SoundManager() {}
			~SoundManager() {}
			std::map<std::string, Mix_Music*> m_music;
			std::map<std::string, Mix_Chunk*> m_sfx;
			Uint16 SoundVolume;
			static SoundManager *m_instance;
	};	//class SoundManager
};	//namespace lm

#endif	//LUNATIC_MELODY_SOUND_MANAGER_H