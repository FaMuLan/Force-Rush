#include "sound_manager.h"
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

fr::SoundManager *fr::SoundManager::m_instance = 0;
std::map<std::string, fr::Sound*> fr::SoundManager::m_sound;
std::string fr::SoundManager::current_sound_data;
unsigned short fr::SoundManager::SoundVolume;

enum mad_flow fr::mp3::input(void *data, struct mad_stream *stream)
{
	Sound *m_sound = (Sound*)data;
	if (!m_sound->file_length)
	{
		return MAD_FLOW_STOP;
	}
	mad_stream_buffer(stream, m_sound->file_start, m_sound->file_length);
	m_sound->file_length = 0;
	return MAD_FLOW_CONTINUE;
}

inline signed int fr::mp3::scale(mad_fixed_t sample)
{
	sample += (1L << (MAD_F_FRACBITS - 16));
	if (sample >= MAD_F_ONE)
	{
		sample = MAD_F_ONE - 1;
	}
	else if (sample < -MAD_F_ONE)
	{
		sample = -MAD_F_ONE;
	}
	return sample >> (MAD_F_FRACBITS + 1 - 16);
}

enum mad_flow fr::mp3::output(void *data, struct mad_header const *header, struct mad_pcm *pcm)
{
	unsigned int channels_count, samples_count, frequency;
	mad_fixed_t const *left_ch, *right_ch;

	Sound *m_sound = (Sound*)data;
	SDL_AudioCVT cvt;
	

	channels_count  = pcm->channels;
	samples_count   = pcm->length;
	frequency = pcm->samplerate;
	left_ch    = pcm->samples[0];
	right_ch   = pcm->samples[1];
	m_sound->current_buffer_duration = 2048 * 4;

	static unsigned char *new_buffer = new unsigned char[2048 * 4];

	static int i = 0;
	while (samples_count--)
	{
		signed int sample;
		sample = scale(*left_ch++);
		new_buffer[i++] = (sample >> 0) & 0xFF;
		new_buffer[i++] = (sample >> 8) & 0xFF;
		if (channels_count == 2)
		{
			sample = scale(*right_ch++);
			new_buffer[i++] = (sample >> 0) & 0xFF;
			new_buffer[i++] = (sample >> 8) & 0xFF;
		}
		else
		{
			new_buffer[i++] = (sample >> 0) & 0xFF;
			new_buffer[i++] = (sample >> 8) & 0xFF;
		}
		if (i >= 2048 * 4)
		{
			i = 0;
			m_sound->buffer.push_back(new_buffer);
			new_buffer = new unsigned char[2048 * 4];
		}
	}
	SDL_BuildAudioCVT(&cvt, AUDIO_S16, channels_count, frequency, AUDIO_S16, 2, 44100);
	cvt.buf = new_buffer;
	cvt.len = samples_count * 4;
	SDL_ConvertAudio(&cvt);
	return MAD_FLOW_CONTINUE;
}

enum mad_flow fr::mp3::error(void *data, struct mad_stream *stream, struct mad_frame *frame)
{
	Sound* m_sound = (Sound*)data;
	fprintf(stderr, "decoding error 0x%04x (%s) at byte offset %u\n", stream->error, mad_stream_errorstr(stream), stream->this_frame - m_sound->file_start);
	return MAD_FLOW_CONTINUE;
}

int fr::mp3::decode(Sound *load_sound)
{
	struct mad_decoder m_decoder;
	int result;

	mad_decoder_init(&m_decoder, load_sound, mp3::input, 0, 0, mp3::output, mp3::error, 0);
	mad_decoder_options(&m_decoder, 0);
	result = mad_decoder_run(&m_decoder, MAD_DECODER_MODE_SYNC);
	mad_decoder_finish(&m_decoder);

	load_sound->sound_duration = load_sound->buffer.size() * 4096.f / 44100.f * 1000.f;
	load_sound->sound_process = 0;
	load_sound->current_buffer_process = 0;
	load_sound->current_buffer_index = 0;

	return result;
}

bool fr::mp3::IsMP3File(unsigned char *magic)
{
	if (strncmp((char*)magic, "ID3", 3) == 0)
	{
		return false;
	}

	if (((magic[0] & 0xFF) != 0xFF) || ((magic[1] & 0xF0) != 0xF0) || ((magic[2] & 0xF0) == 0x00) || ((magic[2] & 0xF0) == 0xF0) || ((magic[2] & 0x0C) == 0x0C) || ((magic[1] & 0x06) == 0x00))
	{
		return true;
	}
	return false;
}

void fr::SoundManager::init()
{
/*
	if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == 0)
	{
		exit(0);
	}
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
*/
	SDL_AudioSpec want, have;
	want.freq = 44100;
	want.format = AUDIO_S16;
	want.channels = 2;
	want.samples = 2048;
	want.callback = &SoundManager::AudioCallback;
	SDL_OpenAudio(&want, &have);

}

void fr::SoundManager::clear()
{
	
}

void fr::SoundManager::clear(std::string path)
{
	
}

bool fr::SoundManager::load(std::string path)
{
	Sound *new_sound = new Sound;
	new_sound->buffer.reserve(100000);
	int fd = open(path.c_str(), O_RDWR);
	void *fdm;
	struct stat st;
	if (fstat(fd, &st) == -1 || st.st_size == 0)
	{
		return false;
	}
	fdm = mmap(0, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if (fdm == MAP_FAILED)
	{
		return false;
	}
	unsigned char *file_ch = (unsigned char*)fdm;
	new_sound->file_start = file_ch;
	new_sound->file_length = st.st_size;
	unsigned int magic = 0;
	magic += file_ch[0] << 24;
	magic += file_ch[1] << 16;
	magic += file_ch[2] << 8;
	magic += file_ch[3] << 0;
	switch (magic)
	{
		case 0x57415645:	//WAVE
		break;
		case 0x4F676753:	//OGGS
		break;
		default:
			if (mp3::IsMP3File((Uint8*)&magic))
			{
				mp3::decode(new_sound);
			}
			else
			{
				return false;
			}
		break;
	}
	//分析SDL_mixer得知，MP3格式的要另外检测
	if (munmap(fdm, st.st_size) == -1)
	{
		return false;
	}
	m_sound[path] = new_sound;
	return true;
}

void fr::SoundManager::play(std::string path)
{
	m_sound[path]->is_playing = true;
	m_sound[path]->sound_process = 0;
	SDL_PauseAudio(0);
}

void fr::SoundManager::play(std::string path, int time)
{
	SDL_PauseAudio(0);
}

void fr::SoundManager::stop()
{
	SDL_PauseAudio(1);
}

void fr::SoundManager::SwitchPause()
{
	
}

void fr::SoundManager::SetVolume(Uint16 load_volume, SoundType type)
{
	
}

void fr::SoundManager::LoadMP3(std::string path)
{
	
}

void fr::SoundManager::LoadOGG(std::string path)
{
	
}

void fr::SoundManager::AudioCallback(void *userdata, unsigned char *stream, int len)
{
	SDL_memset(stream, 0, len);
	for (std::map<std::string, Sound*>::iterator iter = m_sound.begin(); iter != m_sound.end(); iter++)
	{
		Sound *load_data = iter->second;
		if (load_data->sound_process < load_data->sound_duration && load_data->is_playing)
		{
			len = (load_data->current_buffer_process + len > load_data->current_buffer_duration) ? load_data->current_buffer_duration - load_data->current_buffer_process : len;
			SDL_MixAudio(stream, load_data->buffer[load_data->current_buffer_index], len, SDL_MIX_MAXVOLUME);
			load_data->current_buffer_process += len;
			if (load_data->current_buffer_process >= load_data->current_buffer_duration)
			{
				load_data->current_buffer_index++;
				load_data->current_buffer_process = 0;
			}
		}
	}
}

bool fr::SoundManager::IsPlayingMusic()
{
	
}