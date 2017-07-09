#ifndef LUNATIC_MELODY_SOUND_MANAGER_H
#define LUNATIC_MELODY_SOUND_MANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <map>
#include <vector>

namespace lm
{
	enum soundType
	{
		SOUNDTYPE_MUSIC,
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
			void clean();
			void load(std::string path, std::string id, soundType type);
			void setVolume(Uint16 loadVolume, soundType type);
			void output(std::string id, soundType type);
		private:
			SoundManager() {}
			~SoundManager() {}
			std::map<std::string, Mix_Music*> M;
			std::map<std::string, Mix_Chunk*> S;
			Uint16 soundVolume;
			static SoundManager *m_instance;
	};	//class SoundManager
};	//namespace lm

#endif	//LUNATIC_MELODY_SOUND_MANAGER_H