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
#include "mod_widget.h"

fr::SelectState *fr::SelectState::m_instance = 0;

void fr::SelectState::init()
{
	TextureManager::instance()->loadfont("assets/fonts/Audiowide.ttf", 36);
	TextureManager::instance()->loadfont("assets/fonts/Audiowide.ttf", 32);
	SongHeader::instance()->init();
	SongList::instance()->init();
	ModWidget::instance()->init();
	select_back = new Button;
	select_mod = new Button;
	select_refresh = new Button;
	background = new Sprite;
	search_bar = new Sprite;

	select_back->init("assets/base/sort_button.png", 0, 0);
	select_back->AddPressedFrame( "assets/base/sort_button_pressed.png");
	select_mod->init("assets/base/sort_button.png", 0, 72);
	select_mod->AddPressedFrame( "assets/base/sort_button_pressed.png");
	select_refresh->init("assets/base/sort_button.png", 0, 144);
	select_refresh->AddPressedFrame( "assets/base/sort_button_pressed.png");

	search_bar->init("assets/select/search_bar.png", 0, 288);
	select_back->AddText("Back", select_back->GetW() / 2, select_back->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
	select_mod->AddText("Mod", select_mod->GetW() / 2, select_mod->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
	select_refresh->AddText("Refresh", select_refresh->GetW() / 2, select_refresh->GetH() / 2, "assets/fonts/Audiowide.ttf", 32, 0x00, 0x00, 0x00);

	if (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT)
	{
		search_bar->SetPos(0, 288);
		select_back->SetPos(0, 0);
		select_mod->SetPos(0, 72);
		select_refresh->SetPos(0, 144);
	}
	else
	{
		search_bar->SetPos(280, 0);
		select_back->SetPos(0, System::instance()->GetWindowHeigh() - 72);
		select_mod->SetPos(System::instance()->GetWindowWidth() - 160, System::instance()->GetWindowHeigh() - 72);
		select_refresh->SetPos(System::instance()->GetWindowWidth() - 160, System::instance()->GetWindowHeigh() - 144);
	}
}	//void fr::SelectState::init()

void fr::SelectState::clear()
{
	SongList::instance()->clear();
	select_back->clear();
	background->clear();
}

void fr::SelectState::update()
{
	if (System::instance()->IsWindowModified())
	{
		if (System::instance()->GetWindowRotation() == WINDOWROTATION_PORTRAIT)
		{
			search_bar->SetPos(0, 288);
			select_back->SetPos(0, 0);
			select_mod->SetPos(0, 72);
			select_refresh->SetPos(0, 144);
		}
		else
		{
			search_bar->SetPos(280, 0);
			select_back->SetPos(0, System::instance()->GetWindowHeigh() - 72);
			select_mod->SetPos(System::instance()->GetWindowWidth() - 160, System::instance()->GetWindowHeigh() - 72);
			select_refresh->SetPos(System::instance()->GetWindowWidth() - 160, System::instance()->GetWindowHeigh() - 144);
		}
	}

	SongList::instance()->update();
	ModWidget::instance()->update();
	select_back->update();
	select_mod->update();
	select_refresh->update();

	Character::instance()->render();
	SongList::instance()->render();
	SongHeader::instance()->render();
	select_back->render();
	select_mod->render();
	select_refresh->render();
	search_bar->render();
	ModWidget::instance()->render();
	if (!ModWidget::instance()->IsShown())
	{
		if (select_back->IsReleased())
		{
			LoadingState::instance()->init(MainState::instance(), this);
		}
		if (select_mod->IsReleased())
		{
			ModWidget::instance()->SwitchShown();
		}
		if (select_refresh->IsReleased() && !SongList::instance()->IsRefreshing())
		{
			std::thread refresh_thread(&fr::SongList::RefreshList);
			refresh_thread.detach();
		}
	}
}	//void fr::SelectState::update()