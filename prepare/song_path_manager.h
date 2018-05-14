#ifndef FORCE_RUSH_PREPARE_SONG_PATH_MANAGER_H
#define FORCE_RUSH_PREPARE_SONG_PATH_MANAGER_H

#include "../gui/list.h"

namespace fr
{
	class Button;

	class SongPathManagerWidget
	{
		public:
			static SongPathManagerWidget *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new SongPathManagerWidget;
					return m_instance;
				}
				return m_instance;
			}

			void init();
			void clear();
			void update();
			void render();
			void SwitchShown();
			bool IsShown();

			void OnEnter();
			void OnExit();
		private:
			SongPathManagerWidget() {}
			~SongPathManagerWidget() {}
			static SongPathManagerWidget *m_instance;
			Sprite *widget_base;
			bool is_shown;
			bool is_entered;
			bool is_exited;

			Button *new_path;
			Button *delete_path;
	};

	class SongPathList : public List
	{
		public:
			static SongPathList *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new SongPathList;
					return m_instance;
				}
				return m_instance;
			}
			void init();
			void clear();
			void update();
			void render();
			void RefreshList();
			void RefreshListSize();
		private:
			SongPathList() {}
			~SongPathList() {}
			static SongPathList *m_instance;
			std::vector<std::string> list_path;
	};
};

#endif