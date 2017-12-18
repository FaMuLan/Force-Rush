#include "sound_manager.h"

fr::SoundManager *fr::SoundManager::m_instance = 0;

void fr::SoundManager::init()
{
	if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == 0)
	{
		exit(0);
	}
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
}

void fr::SoundManager::clear()
{
	m_sfx.clear();
	m_music.clear();
	Mix_CloseAudio();
}

void fr::SoundManager::clear(std::string path, SoundType type)
{
	if (type == SOUNDTYPE_SFX)
	{
		Mix_FreeChunk(m_sfx[path]);
	}
	if (type == SOUNDTYPE_MUSIC)
	{
		Mix_FreeMusic(m_music[path]);
	}
}

void fr::SoundManager::load(std::string path, SoundType type)
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

void fr::SoundManager::play(std::string path, SoundType type)
{
	if (type == SOUNDTYPE_SFX)
	{
		Mix_PlayChannel(-1, m_sfx[path], 0);
	}
	if (type == SOUNDTYPE_MUSIC)
	{
		Mix_PlayMusic(m_music[path], 0);
		Mix_SetMusicPosition(0);
	}
}

void fr::SoundManager::play(std::string path, int time)
{
	Mix_PlayMusic(m_music[path], 0);
	Mix_SetMusicPosition(double(time) / double(1000));
}

void fr::SoundManager::stop()
{
	Mix_HaltMusic();
}

void fr::SoundManager::SetVolume(Uint16 load_volume, SoundType type)
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

bool fr::SoundManager::IsPlayingMusic()
{
	return Mix_PlayingMusic();
}