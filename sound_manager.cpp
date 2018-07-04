#include "sound_manager.h"
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <samplerate.h>

fr::SoundManager *fr::SoundManager::m_instance = 0;
std::map<std::string, fr::Sound*> fr::SoundManager::m_sound;
std::map<std::string, fr::SoundProcess*> fr::SoundManager::m_process;

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
	static SRC_DATA src_data;
	int samplerate_error = -1;
	unsigned int channels_count, samples_count, frequency;
	mad_fixed_t const *left_ch, *right_ch;

	Sound *m_sound = (Sound*)data;
	SDL_AudioCVT cvt;

	channels_count  = pcm->channels;
	samples_count   = pcm->length;
	frequency = pcm->samplerate;
	left_ch    = pcm->samples[0];
	right_ch   = pcm->samples[1];
	m_sound->buffer_duration = 4096 * 4;
	static float *new_buffer = new float[4096 * 2];
	static float *converted_buffer = new float[4096 * 2];
	static unsigned char *output_buffer = new unsigned char[4096 * 4];
	if (frequency != 44100)
	{
		src_data.end_of_input = 0;
		src_data.data_in = new_buffer;
		src_data.input_frames = 0;
		src_data.src_ratio = 44100.f / frequency;
		src_data.data_out = converted_buffer;
		src_data.output_frames = 4096 * 44100.f / frequency;
	}

	static int i = 0;
	static int j = 0;
	while (samples_count--)
	{
		float sample;
		sample = scale(*left_ch++);
		new_buffer[i++] = sample;
		if (channels_count == 2)
		{
			sample = scale(*right_ch++);
		}
		new_buffer[i++] = sample;
		if (i >= 4096 * 2)
		{
			i = 0;
			//默认频率是44100，如果音频不是44100音频的话要交给libsamplerate转换
			if (frequency != 44100)
			{
				src_data.data_in = new_buffer;
				src_data.input_frames = 4096;
				if (src_simple(&src_data, SRC_LINEAR, 2) == 0)
				{
					for (int k = 0; k < src_data.output_frames_gen * 2; k++)
					{
						output_buffer[j++] = (int(converted_buffer[k]) >> 0) & 0xFF;
						output_buffer[j++] = (int(converted_buffer[k]) >> 8) & 0xFF;
						if (j >= 4096 * 4)
						{
							j = 0;
							m_sound->buffer.push_back(output_buffer);
							output_buffer = new unsigned char[4096 * 4];
						}
					}
				}
			}
			//是44100的话还需要做float -> unsigned char的转换工作
			else
			{
				for (int k = 0; k < 4096 * 2; k++)
				{
					output_buffer[j++] = (int(new_buffer[k]) >> 0) & 0xFF;
					output_buffer[j++] = (int(new_buffer[k]) >> 8) & 0xFF;
				}
				m_sound->buffer.push_back(output_buffer);
				j = 0;
				output_buffer = new unsigned char[4096 * 4];
			}
		}
	}
	
//	SDL_BuildAudioCVT(&cvt, AUDIO_S16, channels_count, frequency, AUDIO_S16, 2, 44100);
//	cvt.buf = output_buffer;
//	cvt.len = samples_count * 4;
//	SDL_ConvertAudio(&cvt);
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

	load_sound->sound_duration = load_sound->buffer.size() * load_sound->buffer_duration / 44.1f;

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
	SDL_AudioSpec want, have;
	want.freq = 44100;
	want.format = AUDIO_S16;
	want.channels = 2;
	want.samples = 4096;
	want.callback = &SoundManager::AudioCallback;
	SDL_OpenAudio(&want, &have);
	SoundProcess *new_default_music = new SoundProcess;
	new_default_music->buffer_process = 0;
	new_default_music->buffer_index = 0;
	new_default_music->sound_process = 0;
	new_default_music->is_playing = false;
	m_process["default_music"] = new_default_music;
	SDL_PauseAudio(0);
}

void fr::SoundManager::clear()
{
	
}

void fr::SoundManager::clear(std::string path)
{
	m_sound[path]->buffer.clear();
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

void fr::SoundManager::play(std::string process_name)
{
	m_process[process_name]->is_playing = true;
	m_process[process_name]->buffer_process = 0;
	m_process[process_name]->sound_process = 0;
}

void fr::SoundManager::seek(std::string process_name, int time)
{
	m_process[process_name]->sound_process = time;
	m_process[process_name]->buffer_index = float(time) * 44.1f / 4096.f;
	m_process[process_name]->buffer_process = int(time * 44.1f) % 4096;
}

void fr::SoundManager::SwitchPause(std::string process_name)
{
	m_process[process_name]->is_playing != m_process[process_name]->is_playing;
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
	for (std::map<std::string, SoundProcess*>::iterator i = m_process.begin(); i != m_process.end(); i++)
	{
		SoundProcess *load_process = i->second;
		if (m_sound[load_process->path])
		{
			if (load_process->sound_process < m_sound[load_process->path]->sound_duration && load_process->is_playing)
			{
				len = (load_process->buffer_process + len > m_sound[load_process->path]->buffer_duration) ?  m_sound[load_process->path]->buffer_duration - load_process->buffer_process : len;
				SDL_MixAudio(stream,  m_sound[load_process->path]->buffer[load_process->buffer_index], len, SDL_MIX_MAXVOLUME);
				load_process->buffer_process += len;
				if (load_process->buffer_process >=  m_sound[load_process->path]->buffer_duration)
				{
					load_process->buffer_index++;
					load_process->buffer_process = 0;
				}
			}
		}
	}
}

void fr::SoundManager::AddProcess(std::string name, SoundProcess *load_process)
{
	m_process[name] = load_process;
}

fr::SoundProcess *fr::SoundManager::GetProcess(std::string name)
{
	return m_process[name];
}