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
	TextureManager::instance()->loadfont("assets/fonts/Audiowide.ttf", 36);
	SongHeader::instance()->init();
	SongList::instance()->init();
	select_back = new Button;
	select_mod = new Button;
	background = new Sprite;
	search_bar = new Sprite;
	select_back->init("assets/base/sort_button.png", 0, 0);
	select_back->AddPressedFrame( "assets/base/sort_button_pressed.png");
	select_mod->init("assets/base/sort_button.png", 0, 72);
	select_mod->AddPressedFrame( "assets/base/sort_button_pressed.png");
	search_bar->init("assets/select/search_bar.png", 0, 288);
	select_back->AddText("Back", select_back->GetW() / 2, select_back->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
	select_mod->AddText("Refresh", select_mod->GetW() / 2, select_mod->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);

	if (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT)
	{
		search_bar->SetPos(0, 288);
		select_back->SetPos(0, 0);
		select_mod->SetPos(0, 72);
	}
	else
	{
		search_bar->SetPos(280, 0);
		select_back->SetPos(0, System::instance()->GetWindowHeigh() - 72);
		select_mod->SetPos(System::instance()->GetWindowWidth() - 160, System::instance()->GetWindowHeigh() - 72);
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
			search_bar->SetPos(0, 288);
			select_back->SetPos(0, 0);
			select_mod->SetPos(0, 72);
		}
		else
		{
			search_bar->SetPos(280, 0);
			select_back->SetPos(0, System::instance()->GetWindowHeigh() - 72);
			select_mod->SetPos(System::instance()->GetWindowWidth() - 160, System::instance()->GetWindowHeigh() - 72);
		}
	}

	SongList::instance()->update();
	select_back->update();
	select_mod->update();

	Character::instance()->render();
	SongList::instance()->render();
	SongHeader::instance()->render();
	select_back->render();
	select_mod->render();
	search_bar->render();
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