#include "sound_manager.h"

lm::SoundManager *lm::SoundManager::m_instance = 0;

void lm::SoundManager::init()
{
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
}

void lm::SoundManager::clear()
{
	m_sfx.clear();
	m_music.clear();
	Mix_CloseAudio();
}

void lm::SoundManager::load(std::string path, SoundType type)
{
	if (type == SOUNDTYPE_SFX)
	{
		Mix_Chunk *new_sound = Mix_LoadWAV(path.c_str());
		m_sfx[path] = new_sound;
	}
	if (type == SOUNDTYPE_MUSIC)
	{
		Mix_Music *new_music = Mix_LoadMUS(path.c_str());
		m_music[path] = new_music;
	}
}

void lm::SoundManager::play(std::string path, SoundType type)
{
	if (type == SOUNDTYPE_SFX)
	{
		Mix_PlayChannel(-1, m_sfx[path], 0);
	}
	if (type == SOUNDTYPE_MUSIC)
	{
		Mix_PlayMusic(m_music[path], -1);
	}
}

void lm::SoundManager::stop()
{
	Mix_HaltMusic();
}

void lm::SoundManager::SetVolume(Uint16 load_volume, SoundType type)
{
	if (type == SOUNDTYPE_MUSIC)
	{
		Mix_VolumeMusic(load_volume);
	}
	if (type == SOUNDTYPE_SFX)
	{
		for (std::map<std::string, Mix_Chunk*>::iterator i = m_sfx.begin(); i != m_sfx.end(); i++)
		{
			Mix_VolumeChunk(i->second, load_volume);
		}
	}
}