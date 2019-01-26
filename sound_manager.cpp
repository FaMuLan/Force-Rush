#include "sound_manager.h"
#include <pthread.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <samplerate.h>
#include <assert.h>

fr::SoundManager *fr::SoundManager::m_instance = 0;
std::map<std::string, fr::Sound*> fr::SoundManager::m_sound;
std::map<std::string, fr::SoundProcess*> fr::SoundManager::m_process;

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

void *fr::mp3::decode(void *arguments)
{
	Sound *load_sound = (Sound*)arguments;
	MP3File *interface = (MP3File*)load_sound->interface;
	struct mad_stream stream;
	struct mad_frame frame;
	struct mad_synth synth;
	mad_stream_init(&stream);
	mad_frame_init(&frame);
	mad_synth_init(&synth);
	mad_stream_buffer(&stream, interface->frame[interface->decode_frame]->file_frame_start, load_sound->file_size + load_sound->file_start - interface->frame[interface->decode_frame]->file_frame_start);

	while (1)
	{
		if (interface->decode_frame + 1 >= interface->frame.size() || load_sound->abort)
		{
			load_sound->abort = true;
			break;
		}
		else if (stream.error != MAD_ERROR_BUFLEN)
		{
			static SRC_DATA src_data;
			int samplerate_error = -1;
			int mad_error = 0;
			unsigned int channels_count, samples_count, frequency;
			mad_fixed_t const *left_ch, *right_ch;

//			assert(stream.error != MAD_ERROR_BUFLEN);

			mad_error = mad_frame_decode(&frame, &stream);
			mad_synth_frame(&synth, &frame);
			struct mad_pcm *pcm = &synth.pcm;

			channels_count  = pcm->channels;
			samples_count   = pcm->length;
			frequency = pcm->samplerate;
			left_ch    = pcm->samples[0];
			right_ch   = pcm->samples[1];
			load_sound->buffer_duration = 4096 * 4;
			if (frequency != 44100)
			{
				src_data.end_of_input = 0;
				src_data.data_in = load_sound->new_buffer;
				src_data.input_frames = 0;
				src_data.src_ratio = 44100.f / frequency;
				src_data.data_out = load_sound->converted_buffer;
				src_data.output_frames = 4096 * 44100.f / frequency;
			}

			while (samples_count--)
			{
				float sample;
				sample = scale(*left_ch++);
				load_sound->new_buffer[load_sound->src_sample_index++] = sample;
				if (channels_count == 2)
				{
					sample = scale(*right_ch++);
				}
				load_sound->new_buffer[load_sound->src_sample_index++] = sample;
				if (load_sound->src_sample_index >= 4096 * 2)
				{
					load_sound->src_sample_index = 0;
					//默认频率是44100，如果音频不是44100音频的话要交给libsamplerate转换
					if (frequency != 44100)
					{
						src_data.data_in = load_sound->new_buffer;
						src_data.input_frames = 4096;
						if (src_simple(&src_data, SRC_LINEAR, 2) == 0)
						{
							for (int i = 0; i < src_data.output_frames_gen * 2; i++)
							{
								load_sound->output_buffer[load_sound->dest_sample_index++] = (int(load_sound->converted_buffer[i]) >> 0) & 0xFF;
								load_sound->output_buffer[load_sound->dest_sample_index++] = (int(load_sound->converted_buffer[i]) >> 8) & 0xFF;
						if (load_sound->dest_sample_index >= 4096 * 4)
						{
									load_sound->dest_sample_index = 0;
									load_sound->buffer.push_back(load_sound->output_buffer);
									load_sound->output_buffer = new unsigned char[4096 * 4];
								}
							}
						}
					}
					//是44100的话还需要做float -> unsigned char的转换工作
					else
					{
						for (int i = 0; i < 4096 * 2; i++)
						{
							load_sound->output_buffer[load_sound->dest_sample_index++] = (int(load_sound->new_buffer[i]) >> 0) & 0xFF;
							load_sound->output_buffer[load_sound->dest_sample_index++] = (int(load_sound->new_buffer[i]) >> 8) & 0xFF;
						}
						load_sound->buffer.push_back(load_sound->output_buffer);
						load_sound->dest_sample_index = 0;
						load_sound->output_buffer = new unsigned char[4096 * 4];
					}
				}
			}

			interface->decode_frame++;
		}
		else
		{
//			exit(0);
			break;
		}
	}
	mad_stream_finish(&stream);
	mad_frame_finish(&frame);
	mad_synth_finish(&synth);
}

void fr::mp3::prepare(Sound *load_sound)
{
	fseek(load_sound->fp, 0, SEEK_END);
	load_sound->file_size = ftell(load_sound->fp);
	fseek(load_sound->fp, 0, SEEK_SET);
	load_sound->file_start = new unsigned char[load_sound->file_size];
	fread(load_sound->file_start, 1, load_sound->file_size, load_sound->fp);
	MP3File *new_interface = new MP3File;
	load_sound->interface = new_interface;
	new_interface->decode_start_time = 0;
	new_interface->decode_frame = 0;
	float current_time = 0;
	int mad_error = 0;
	struct mad_stream stream;
	struct mad_header header;
	mad_stream_init(&stream);
	mad_header_init(&header);
	mad_stream_buffer(&stream, load_sound->file_start, load_sound->file_size);
	while (stream.error != MAD_ERROR_BUFLEN)
	{
		mad_header_decode(&header, &stream);
		if (MAD_RECOVERABLE(stream.error))
		{
			MP3Frame *new_frame = new MP3Frame;
			new_frame->file_frame_start = stream.this_frame;
			new_frame->file_frame_size = stream.next_frame - stream.this_frame;
			if (new_frame->file_frame_size == 0)
			{
				new_frame->file_frame_size = load_sound->file_size - ((uintptr_t)stream.this_frame - (uintptr_t)load_sound->file_start);
			}
			new_frame->start_time = current_time;
			new_frame->duration = 1000 * header.duration.seconds;
			new_frame->duration += 1000.f * header.duration.fraction / MAD_TIMER_RESOLUTION;
			current_time += new_frame->duration;
			new_frame->frequency = header.samplerate;
			switch (header.mode)
			{
				case MAD_MODE_SINGLE_CHANNEL:
					new_frame->channels = 1;
				break;
				case MAD_MODE_DUAL_CHANNEL:
				case MAD_MODE_JOINT_STEREO:
				case MAD_MODE_STEREO:
					new_frame->channels = 2;
				break;
			}
			new_interface->frame.push_back(new_frame);
		}
	}
	mad_header_finish(&header);
	mad_stream_finish(&stream);
}

void fr::mp3::seek(Sound *load_sound, unsigned int time)
{
	MP3File *m_interface = (MP3File*)load_sound->interface;
	while (1)
	{
		int temp_size = m_interface->frame.size();
		if (m_interface->decode_frame + 2 < m_interface->frame.size())
		{
			if (m_interface->frame[m_interface->decode_frame]->start_time < time)
			{
				m_interface->decode_frame++;
				int temp_time = m_interface->frame[m_interface->decode_frame]->start_time;
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}
	/*
	while (m_interface->frame[m_interface->decode_frame]->start_time < time)
	{
		m_interface->decode_frame++;
		if (m_interface->decode_frame + 1 == m_interface->frame.size())
		{
			break;
		}
	}
	m_interface->decode_frame--;
	m_interface->decode_start_time = time;
	*/
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

bool fr::SoundManager::load(std::string path, unsigned int time)
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
		ogg::seek(new_sound, time);
		pthread_t thread;
		pthread_create(&thread, NULL, &fr::ogg::decode, new_sound);
		pthread_detach(thread);
	}
	if (memcmp(magic, "ID3", 3) == 0 || ((magic[0] == 0xFF) && (magic[1] & 0xFE == 0xFA)))
	{
		mp3::prepare(new_sound);
		new_sound->type = AUDIOFILETYPE_MP3;
		mp3::seek(new_sound, time);
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

void fr::SoundManager::seek(std::string process_name, unsigned int time)
{
	Sound *load_sound = m_sound[m_process[process_name]->path];
	/*
	m_process[process_name]->sound_process = time;
	m_process[process_name]->buffer_index = float(time) * 44.1f / 4096.f;
	m_process[process_name]->buffer_process = int(time * 44.1f) % 4096;
	*/
	switch (load_sound->type)
	{
		case AUDIOFILETYPE_MP3:
			mp3::seek(load_sound, time);
		break;
		case AUDIOFILETYPE_OGG:
			ogg::seek(load_sound, time);
		break;
	}
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