#ifndef FORCE_RUSH_TOOLS_FILE_EXPLORER_H
#define FORCE_RUSH_TOOLS_FILE_EXPLORER_H

#include <string>
#include <vector>
#include "../gui/list.h"

namespace fr
{
	class Sprite;
	class Button;
	class TextArea;

	class FileExplorerWidget
	{
		public:
			static FileExplorerWidget *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new FileExplorerWidget;
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
			bool GetOutput(std::string &result);

			void OnEnter();
			void OnExit();
		private:
			FileExplorerWidget() {}
			~FileExplorerWidget() {}
			static FileExplorerWidget *m_instance;
			Sprite *widget_base_head;
			Sprite *widget_base_body;
			Sprite *widget_base_foot;
			bool is_outputed;
			bool is_shown;
			bool is_entered;
			bool is_exited;
			int body_count;
			int widget_y;
			int widget_h;

			Button *confirm;
			TextArea *current_path;
			Button *parent_dir;
	};

	class FileList : public List
	{
		public:
			static FileList *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new FileList;
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

			std::string GetCurrentPath();
			void PathBack();
		private:
			FileList() {}
			~FileList() {}
			static FileList *m_instance;
			std::vector<std::string> dir_list;
			std::string current_path;
			TextArea *no_dir_label;
	};
};

#endif