#include "sound_manager.h"
#include "math.h"

lm::SoundManager *lm::SoundManager::m_instance = 0;

void lm::SoundManager::init()
{
	Mix_OpenAudio(22050, AUDIO_S16, 2, 2048);
}

void lm::SoundManager::clean()
{
	S.clear();
	M.clear();
	Mix_CloseAudio();
}

void lm::SoundManager::load(std::string path, std::string id, soundType type)
{
	if (type == SOUNDTYPE_SFX)
	{
		Mix_Chunk *newSound = Mix_LoadWAV(path.c_str());
		//加載聲音文件.
		S[id] = newSound;
		//將指針放入容器.
	}
	if (type == SOUNDTYPE_MUSIC)
	{
		Mix_Music *newMusic = Mix_LoadMUS(path.c_str());
		M[id] = newMusic;
	}
}

void lm::SoundManager::setVolume(Uint16 loadVolume, soundType type)
{
	if (type == SOUNDTYPE_MUSIC)
	{
		Mix_VolumeMusic(loadVolume);
	}
	if (type == SOUNDTYPE_SFX)
	{
		for (std::map<std::string, Mix_Chunk*>::iterator i = S.begin(); i != S.end(); i++)
		{
			Mix_VolumeChunk(i->second, loadVolume);
		}
	}
}

void lm::SoundManager::output(std::string id, soundType type)
{
	if (type == SOUNDTYPE_SFX)
	{
		Mix_PlayChannel(0, S[id], 1);
	}
	if (type == SOUNDTYPE_MUSIC)
	{
		Mix_PlayMusic(M[id], 1);
	}
}