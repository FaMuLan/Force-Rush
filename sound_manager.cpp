#include "sound_manager.h"
#include <pthread.h>
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
	MP3File *m_interface = (MP3File*)m_sound->interface;
	if (m_interface->decode_frame + 1 == m_interface->frame.size() || m_sound->abort)
	{
		return MAD_FLOW_STOP;
	}
	mad_stream_buffer(stream, m_interface->frame[m_interface->decode_frame]->file_frame_start, m_interface->frame[m_interface->decode_frame]->file_frame_size);
	m_interface->decode_frame++;
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

	channels_count  = pcm->channels;
	samples_count   = pcm->length;
	frequency = pcm->samplerate;
	left_ch    = pcm->samples[0];
	right_ch   = pcm->samples[1];
	m_sound->buffer_duration = 4096 * 4;
	if (frequency != 44100)
	{
		src_data.end_of_input = 0;
		src_data.data_in = m_sound->new_buffer;
		src_data.input_frames = 0;
		src_data.src_ratio = 44100.f / frequency;
		src_data.data_out = m_sound->converted_buffer;
		src_data.output_frames = 4096 * 44100.f / frequency;
	}

	while (samples_count--)
	{
		float sample;
		sample = scale(*left_ch++);
		m_sound->new_buffer[m_sound->src_sample_index++] = sample;
		if (channels_count == 2)
		{
			sample = scale(*right_ch++);
		}
		m_sound->new_buffer[m_sound->src_sample_index++] = sample;
		if (m_sound->src_sample_index >= 4096 * 2)
		{
			m_sound->src_sample_index = 0;
			//默认频率是44100，如果音频不是44100音频的话要交给libsamplerate转换
			if (frequency != 44100)
			{
				src_data.data_in = m_sound->new_buffer;
				src_data.input_frames = 4096;
				if (src_simple(&src_data, SRC_LINEAR, 2) == 0)
				{
					for (int i = 0; i < src_data.output_frames_gen * 2; i++)
					{
						m_sound->output_buffer[m_sound->dest_sample_index++] = (int(m_sound->converted_buffer[i]) >> 0) & 0xFF;
						m_sound->output_buffer[m_sound->dest_sample_index++] = (int(m_sound->converted_buffer[i]) >> 8) & 0xFF;
						if (m_sound->dest_sample_index >= 4096 * 4)
						{
							m_sound->dest_sample_index = 0;
							m_sound->buffer.push_back(m_sound->output_buffer);
							m_sound->output_buffer = new unsigned char[4096 * 4];
						}
					}
				}
			}
			//是44100的话还需要做float -> unsigned char的转换工作
			else
			{
				for (int i = 0; i < 4096 * 2; i++)
				{
					m_sound->output_buffer[m_sound->dest_sample_index++] = (int(m_sound->new_buffer[i]) >> 0) & 0xFF;
					m_sound->output_buffer[m_sound->dest_sample_index++] = (int(m_sound->new_buffer[i]) >> 8) & 0xFF;
				}
				m_sound->buffer.push_back(m_sound->output_buffer);
				m_sound->dest_sample_index = 0;
				m_sound->output_buffer = new unsigned char[4096 * 4];
			}
		}
	}

	return MAD_FLOW_CONTINUE;
}

enum mad_flow fr::mp3::error(void *data, struct mad_stream *stream, struct mad_frame *frame)
{
	Sound *m_sound = (Sound*)data;
	fprintf(stderr, "decoding error 0x%04x (%s) at byte offset %u\n", stream->error, mad_stream_errorstr(stream), stream->this_frame - m_sound->file_start);
	return MAD_FLOW_CONTINUE;
}

void *fr::mp3::decode(void *arguments)
{
	Sound *load_sound = (Sound*)arguments;
	struct mad_decoder m_decoder;
	mad_decoder_init(&m_decoder, load_sound, mp3::input, 0, 0, mp3::output, mp3::error, 0);
	mad_decoder_run(&m_decoder, MAD_DECODER_MODE_ASYNC);
	mad_decoder_finish(&m_decoder);
}

void fr::mp3::prepare(Sound *load_sound)
{
	fseek(load_sound->fp, 0, SEEK_END);
	load_sound->file_size = ftell(load_sound->fp);
	fseek(load_sound->fp, 0, SEEK_SET);
	load_sound->file_start = new unsigned char[load_sound->file_size];
	fread(load_sound->file_start, 1, load_sound->file_size, load_sound->fp);
	unsigned int i = 0;
	if (memcmp(load_sound->file_start, "ID3", 3) == 0)
	{
		int offset = (load_sound->file_start[6] & 0x7F) * 0x200000 + (load_sound->file_start[7] & 0x7F) * 0x400 + (load_sound->file_start[8] & 0x7F) * 0x80 + (load_sound->file_start[9] & 0x7F);
		i += offset;
	}
	MP3File *new_interface = new MP3File;
	load_sound->interface = new_interface;
	new_interface->decode_start_time = 0;
	new_interface->decode_frame = 0;

	MP3Frame *last_frame;
	while (i < load_sound->file_size)
	{
		if (load_sound->file_start[i] == 0xFF)
		{
			MP3Frame *new_frame = new MP3Frame;
			new_frame->file_frame_start = load_sound->file_start + i;
			if (new_interface->frame.size() > 0)
			{
				last_frame->file_frame_size = &(new_frame->file_frame_start) - &(last_frame->file_frame_start);
			}
			/*
				samples count:
				+----------+----------+----------+----------+
				|          |  MPEG 1  |  MPEG 2  | MPEG 2.5 |
				| Layer  1 |   384    |   384    |   384    |
				| Layer  2 |   1152   |   1152   |   1152   |
				| Layer  3 |   1152   |   576    |   576    |
				+----------+----------+----------+----------+
				frequency(kHz):
				+----------+----------+----------+----------+
				|          |  MPEG 1  |  MPEG 2  | MPEG 2.5 |
				|    00    |   44.1   |   22.05  |  11.025  |
				|    01    |   48.0   |   24.0   |   12.0   |
				|    10    |   32.0   |   16.0   |   8.0    |
				+----------+----------+----------+----------+
			*/
			//[11111111] [xxx11000] before
			//[11111111] [000xxx11] after '> 3'
			//00000011 = 0x03
			//这TM叫RNA转录.jpg
			unsigned int version = (load_sound->file_start[i + 1] > 3) & 0x03;
			/*
				version:
					MPEG 1:    11
					MPEG 2:    10
					UNDEFINED: 01
					MPEG 2.5:  00
			*/
			unsigned int layer = load_sound->file_start[i + 1] & 0x03;
			/*
				layer:
					UNDEFINED: 00
					Layer 1:   11
					Layer 2:   10
					Layer 3:   01
				
			*/
			switch ((load_sound->file_start[i + 3] > 2) & 0x03)
			{
				case 0x00:
					new_frame->frequency = 44100;
				break;
				case 0x01:
					new_frame->frequency = 48000;
				break;
				case 0x02:
					new_frame->frequency = 32000;
				break;
			}
			switch (layer)
			{
				case 0x03:
					new_frame->samples_count = 384;
				break;
				case 0x02:
					new_frame->samples_count = 1152;
				break;
				case 0x01:
					new_frame->samples_count = 1152;
				break;
			}
			switch (version)
			{
				case 0x03:
					
				break;
				case 0x02:
					new_frame->frequency /= 2;
					if (layer == 0x01)
					{
						new_frame->samples_count /= 2;
					}
				break;
				case 0x00:
					new_frame->frequency /= 4;
					if (layer == 0x01)
					{
						new_frame->samples_count /= 2;
					}
				break;
			}
			new_frame->start_time = new_interface->samples_count * 1000.f / new_frame->frequency;
			new_interface->samples_count += new_frame->samples_count;

			new_interface->frame.push_back(new_frame);
			last_frame = new_frame;
		}
		i++;
	}
	last_frame->file_frame_size = &(last_frame->file_frame_start) - &(load_sound->file_start) + load_sound->file_size;
}

void fr::mp3::seek(Sound *load_sound, unsigned int time)
{
	MP3File *m_interface = (MP3File*)load_sound->interface;
	while (m_interface->frame[m_interface->decode_frame]->start_time < time)
	{
		m_interface->decode_frame++;
		if (!m_interface->frame[m_interface->decode_frame])
		{
			break;
		}
	}
	m_interface->decode_frame--;
	m_interface->decode_start_time = time;
}

void fr::mp3::clear(Sound *load_sound)
{
	MP3File *m_interface = (MP3File*)load_sound->interface;
	m_interface->frame.clear();
	delete m_interface;
}

void fr::ogg::prepare(Sound *load_sound)
{
	OGGFile *new_interface = new OGGFile;
	ov_open_callbacks(load_sound->fp, &new_interface->file, NULL, 0, OV_CALLBACKS_NOCLOSE);
	new_interface->info = ov_info(&new_interface->file, -1);
	load_sound->interface = new_interface;
}

void *fr::ogg::decode(void *arguments)
{
	Sound *m_sound = (Sound*)arguments;
	OGGFile *interface = (OGGFile*)m_sound->interface;
	static SRC_DATA src_data;
	int samplerate_error = -1;
	int current_section = 0;
	int channels_count;
	int frequency;
	bool eof = false;
	float **load_buffer;
	channels_count = interface->info->channels;
	frequency = interface->info->rate;
	m_sound->buffer_duration = 4096 * 4;
	if (frequency != 44100)
	{
		src_data.end_of_input = 0;
		src_data.data_in = m_sound->new_buffer;
		src_data.input_frames = 0;
		src_data.src_ratio = 44100.f / frequency;
		src_data.data_out = m_sound->converted_buffer;
		src_data.output_frames = 4096 * 44100.f / frequency;
	}
	while (!eof && !m_sound->abort)
	{
		long samples_count = ov_read_float(&interface->file, &load_buffer, 4096, &current_section);
		if (samples_count <= 0)
		{
			eof = true;
		}
		if (samples_count > 0)
		{
			for (int i = 0; i < samples_count; i++)
			{
				m_sound->new_buffer[m_sound->src_sample_index++] = load_buffer[0][i];
								m_sound->new_buffer[m_sound->src_sample_index++] = channels_count == 1 ? load_buffer[0][i] : load_buffer[1][i];
				//强行双声道
				if (m_sound->src_sample_index >= 4096 * 2)
				{
					m_sound->src_sample_index = 0;
					//默认频率是44100，如果音频不是44100音频的话要交给libsamplerate转换
					if (frequency != 44100)
					{
						src_data.data_in = m_sound->new_buffer;
						src_data.input_frames = 4096;
						if (src_simple(&src_data, SRC_LINEAR, 2) == 0)
						{
							for (int j = 0; j < src_data.output_frames_gen * 2; j++)
							{
								m_sound->output_buffer[m_sound->dest_sample_index++] = (int(m_sound->converted_buffer[j] * 28000) >> 0) & 0xFF;
								m_sound->output_buffer[m_sound->dest_sample_index++] = (int(m_sound->converted_buffer[j] * 28000) >> 8) & 0xFF;
								if (m_sound->dest_sample_index >= 4096 * 4)
								{
									m_sound->dest_sample_index = 0;
									m_sound->buffer.push_back(m_sound->output_buffer);
									m_sound->output_buffer = new unsigned char[4096 * 4];
								}
							}
						}
					}
					else
					{
						for (int j = 0; j < 4096 * 2; j++)
						{
							int sample = m_sound->new_buffer[j] * 28000;
							m_sound->output_buffer[m_sound->dest_sample_index++] = (sample >> 0) & 0xFF;
							m_sound->output_buffer[m_sound->dest_sample_index++] = (sample >> 8) & 0xFF;
						}
						m_sound->buffer.push_back(m_sound->output_buffer);
						m_sound->dest_sample_index = 0;
						m_sound->output_buffer = new unsigned char[4096 * 4];
					}
				}
			}
		}
	}
	return 0;
}

void fr::ogg::seek(Sound *load_sound, unsigned int time)
{
	OGGFile *interface = (OGGFile*)load_sound->interface;
	ov_time_seek(&interface->file, time / 1000.f);
}

void fr::ogg::clear(Sound *load_sound)
{
	OGGFile *interface = (OGGFile*)load_sound->interface;
	ov_clear(&interface->file);
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
	if (m_sound[path])
	{
		for (int i = 0; i < m_sound[path]->buffer.size(); i++)
		{
			delete [] m_sound[path]->buffer[i];
		}
		m_sound[path]->buffer.clear();
	}
}

bool fr::SoundManager::load(std::string path)
{
	Sound *new_sound = new Sound;
	new_sound->buffer.reserve(100000);
	FILE *fp = fopen(path.c_str(), "r");
	new_sound->path = path;
	new_sound->fp = fp;
	new_sound->new_buffer = new float[4096 * 2];
	new_sound->converted_buffer = new float[4096 * 2];
	new_sound->output_buffer = new unsigned char[4096 * 4];
	new_sound->src_sample_index = 0;
	new_sound->dest_sample_index = 0;
	new_sound->abort = false;

	char *magic = new char[4];
	fread(magic, 1, 4, fp);
	fseek(fp, 0, SEEK_SET);
	if (memcmp(magic, "OggS", 4) == 0)
	{
		ogg::prepare(new_sound);
		new_sound->type = AUDIOFILETYPE_OGG;
		pthread_t thread;
		pthread_create(&thread, NULL, &fr::ogg::decode, new_sound);
		pthread_detach(thread);
	}
	if (memcmp(magic, "ID3", 3) == 0 || ((magic[0] == 0xFF) && (magic[1] & 0xFE == 0xFA)))
	{
		mp3::prepare(new_sound);
		new_sound->type = AUDIOFILETYPE_MP3;
		pthread_t thread;
		pthread_create(&thread, NULL, &fr::mp3::decode, new_sound);
		pthread_detach(thread);
	}
	delete [] magic;
	

//	new_sound->sound_duration = new_sound->buffer.size() * new_sound->buffer_duration / 44.1f;
	m_sound[path] = new_sound;
	return true;
}

void fr::SoundManager::play(std::string process_name)
{
	m_process[process_name]->is_playing = true;
	m_process[process_name]->buffer_process = 0;
	m_process[process_name]->buffer_index = 0;
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

void fr::SoundManager::AudioCallback(void *userdata, unsigned char *stream, int len)
{
	SDL_memset(stream, 0, len);
	for (std::map<std::string, SoundProcess*>::iterator i = m_process.begin(); i != m_process.end(); i++)
	{
		SoundProcess *load_process = i->second;
		if (m_sound[load_process->path])
		{
			if (load_process->buffer_index < m_sound[load_process->path]->buffer.size() && load_process->is_playing)
			{
				len = (load_process->buffer_process + len > m_sound[load_process->path]->buffer_duration) ?  m_sound[load_process->path]->buffer_duration - load_process->buffer_process : len;
				SDL_MixAudio(stream,  m_sound[load_process->path]->buffer[load_process->buffer_index], len, SDL_MIX_MAXVOLUME);
				load_process->buffer_process += len;
				if (load_process->buffer_process >=  m_sound[load_process->path]->buffer_duration)
				{
					load_process->buffer_index++;
					load_process->buffer_process %= m_sound[load_process->path]->buffer_duration;
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