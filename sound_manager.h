#ifndef FORCE_RUSH_SOUND_MANAGER_H
#define FORCE_RUSH_SOUND_MANAGER_H

#include <SDL2/SDL.h>
#include <mad.h>
#include <string>
#include <map>
#include <vector>

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
		unsigned char *file_start;
		unsigned int file_length;
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

	namespace ogg
	{
		static int decode(Sound *load_sound);
	};

	namespace mp3
	{
		static enum mad_flow input(void *data, struct mad_stream *stream);
		static inline signed int scale(mad_fixed_t sample);
		static enum mad_flow output(void *data, struct mad_header const *header, struct mad_pcm *pcm);
		static enum mad_flow error(void *data, struct mad_stream *stream, struct mad_frame *frame);
		static int decode(Sound *load_sound);
		static bool IsMP3File(unsigned char *magic);
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
			void LoadMP3(std::string path);
			void LoadOGG(std::string path);
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