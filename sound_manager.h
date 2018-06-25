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
		unsigned char *file_start;
		unsigned int file_length;
		std::vector<unsigned char*> buffer;
		unsigned long current_buffer_index;
		unsigned int current_buffer_duration;
		unsigned int current_buffer_process;
		unsigned int sound_duration;
		unsigned int sound_process;
		bool is_playing;
	};

	namespace ogg
	{
		
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
			void play(std::string path);
			void play(std::string path, int time);
			void stop();	//music type only
			void SwitchPause();	//music type only
			void SetVolume(Uint16 load_volume, SoundType type);
			void LoadMP3(std::string path);
			void LoadOGG(std::string path);
			static void AudioCallback(void *userdata, unsigned char *stream, int len);

			bool IsPlayingMusic();
		private:
			SoundManager() {}
			~SoundManager() {}
			static std::map<std::string, Sound*> m_sound;
			static std::string current_sound_data;
			static unsigned short SoundVolume;
			static SoundManager *m_instance;
	};	//class SoundManager
};	//namespace fr

#endif	//FORCE_RUSH_SOUND_MANAGER_H