#ifndef FORCE_RUSH_SOUND_MANAGER_H
#define FORCE_RUSH_SOUND_MANAGER_H

#include <SDL2/SDL.h>
#include <mad.h>
#include <string>
#include <map>
#include <vector>
#include <mad.h>
#include <vorbis/vorbisfile.h>
#include <vorbis/codec.h>

namespace fr
{
	enum SoundType
	{
		SOUNDTYPE_MUSIC = 0,
		SOUNDTYPE_SFX
	};

	enum AudioFileType
	{
		AUDIOFILETYPE_MP3 = 0,
		AUDIOFILETYPE_OGG,
		AUDIOFILETYPE_WAV
	};

	struct Sound
	{
		std::string path;
		FILE *fp;
		void *interface;
		AudioFileType type;
		unsigned char *file_start;
		unsigned int file_size;
		int src_sample_index;
		int dest_sample_index;
		float *new_buffer;
		float *converted_buffer;
		unsigned char *output_buffer;
		//以上是加载才会用到的数据 
		//其中两个sample索引是方便音频缓冲区凑4096个sample
		std::vector<unsigned char*> buffer;
		unsigned int buffer_duration;
		unsigned int sound_duration;
		bool abort;
	};

	struct SoundProcess
	{
		std::string path;
		unsigned long buffer_index;
		unsigned int sound_process;
		unsigned int buffer_process;
		unsigned char volume;
		bool is_playing;
	};

	namespace mp3
	{
		struct MP3Frame
		{
			int frequency;
			int channels;
			int time;
			unsigned char *file_frame_start;
			unsigned int file_frame_size;
			unsigned int samples_count;
			unsigned int start_time;
		};
		struct MP3File
		{
			std::string path;
			std::vector<MP3Frame*> frame;
			int samples_count;
			unsigned int decode_start_time;
			unsigned int decode_frame;
		};
		static enum mad_flow input(void *data, struct mad_stream *stream);
		static inline signed int scale(mad_fixed_t sample);
		static enum mad_flow output(void *data, struct mad_header const *header, struct mad_pcm *pcm);
		static enum mad_flow error(void *data, struct mad_stream *stream, struct mad_frame *frame);
		void prepare(Sound *load_sound);
		void *decode(void *arguments);
		void seek(Sound *load_sound, unsigned int time);
		void clear(Sound *load_sound);
	};

	namespace ogg
	{
		struct OGGFile
		{
			OggVorbis_File file;
			vorbis_info *info;
		};
		//你以为vorbisfile那么方便就不需要写struct了吗.jpg
		void prepare(Sound *load_sound);
		void *decode(void *arguments);
		void seek(Sound *load_sound, unsigned int time);
		void clear(Sound *load_sound);
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
			void clear(std::string path);
			bool load(std::string path);
			void play(std::string process_name);
			void seek(std::string process_name, int time);
			void SwitchPause(std::string process_name);
			static void AudioCallback(void *userdata, unsigned char *stream, int len);
			void AddProcess(std::string name, SoundProcess *load_process);
			SoundProcess *GetProcess(std::string name);
		private:
			SoundManager() {}
			~SoundManager() {}
			static std::map<std::string, Sound*> m_sound;
			static std::map<std::string, SoundProcess*> m_process;
			static SoundManager *m_instance;
	};	//class SoundManager
};	//namespace fr

#endif	//FORCE_RUSH_SOUND_MANAGER_H