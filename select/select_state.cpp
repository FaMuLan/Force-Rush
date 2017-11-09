#include "select_state.h"
#include <thread>
#include "../button.h"
#include "../sprite.h"
#include "../system.h"
#include "../texture_manager.h"
#include "../main/main_state.h"
#include "../loading/loading_state.h"
#include "../user/character.h"
#include "song_list.h"
#include "song_header.h"

lm::SelectState *lm::SelectState::m_instance = 0;

void lm::SelectState::init()
{
	TextureManager::instance()->loadfont("assets/Audiowide.ttf", 36);
	SongHeader::instance()->init();
	SongList::instance()->init();
	select_back = new Button;
	select_mod = new Button;
	background = new Sprite;
	search_bar = new Sprite;
	random_bar = new Sprite;
	select_footer = new Sprite;
	select_back->init("assets/sort_button.png", 0, System::instance()->GetWindowHeigh() - 80, 176, 80);
	select_back->AddPressedFrame( "assets/sort_button_pressed.png");
	select_mod->init("assets/sort_button.png", System::instance()->GetWindowWidth() - 176, System::instance()->GetWindowHeigh() - 80, 176, 80);
	select_mod->AddPressedFrame( "assets/sort_button_pressed.png");
	background->init("assets/background.jpg", 0, 0, 720, 1440);
	search_bar->init("assets/select_black_cell.png", System::instance()->GetWindowWidth() - 654, 300, 654, 60);
	random_bar->init("assets/select_black_cell.png", System::instance()->GetWindowWidth() - 654, System::instance()->GetWindowHeigh() - 140, 654, 60);
	select_footer->init("assets/select_footer.png", 0, System::instance()->GetWindowHeigh() - 80, 720, 80);
	select_back->AddText("Back", select_back->GetW() / 2, select_back->GetH() / 2, "assets/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
	select_mod->AddText("Refresh", select_mod->GetW() / 2, select_mod->GetH() / 2, "assets/Audiowide.ttf", 36, 0x00, 0x00, 0x00);

	if (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT)
	{
		search_bar->SetPos(System::instance()->GetWindowWidth() - 654, 300);
		random_bar->SetPos(System::instance()->GetWindowWidth() - 654, System::instance()->GetWindowHeigh() - 140);
		select_back->SetPos(0, System::instance()->GetWindowHeigh() - 80);
		select_mod->SetPos(System::instance()->GetWindowWidth() - 176, System::instance()->GetWindowHeigh() - 80);
	}
	else
	{
		search_bar->SetPos(System::instance()->GetWindowWidth() - 654, 0);
		random_bar->SetPos(System::instance()->GetWindowWidth() - 654, System::instance()->GetWindowHeigh() - 60);
		select_back->SetPos(0, System::instance()->GetWindowHeigh() - 80);
		select_mod->SetPos(System::instance()->GetWindowWidth() - 830, System::instance()->GetWindowHeigh() - 80);
	}
}	//void lm::SelectState::init()

void lm::SelectState::clear()
{
	SongList::instance()->clear();
	select_back->clear();
	background->clear();
}

void lm::SelectState::update()
{
	if (System::instance()->IsWindowModified())
	{
		if (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT)
		{
			search_bar->SetPos(System::instance()->GetWindowWidth() - 654, 300);
			random_bar->SetPos(System::instance()->GetWindowWidth() - 654, System::instance()->GetWindowHeigh() - 140);
			select_back->SetPos(0, System::instance()->GetWindowHeigh() - 80);
			select_mod->SetPos(System::instance()->GetWindowWidth() - 176, System::instance()->GetWindowHeigh() - 80);
		}
		else
		{
			search_bar->SetPos(System::instance()->GetWindowWidth() - 654, 0);
			random_bar->SetPos(System::instance()->GetWindowWidth() - 654, System::instance()->GetWindowHeigh() - 60);
			select_back->SetPos(0, System::instance()->GetWindowHeigh() - 80);
			select_mod->SetPos(System::instance()->GetWindowWidth() - 830, System::instance()->GetWindowHeigh() - 80);
		}
	}

	SongList::instance()->update();
	select_back->update();
	select_mod->update();

//	background->render();
	Character::instance()->render();
	SongList::instance()->render();
	SongHeader::instance()->render();
//	select_footer->render();
	select_back->render();
	select_mod->render();
	search_bar->render();
	random_bar->render();
	if (select_back->IsReleased())
	{
		LoadingState::instance()->init(MainState::instance(), this);
	}
	if (select_mod->IsReleased() && !SongList::instance()->IsRefreshing())
	{
		std::thread refresh_thread(&lm::SongList::RefreshList);
		refresh_thread.detach();
	}
}	//void lm::SelectState::update()