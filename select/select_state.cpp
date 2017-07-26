#include "select_state.h"

lm::SelectState *lm::SelectState::m_instance = 0;

void lm::SelectState::init()
{
	SongList::instance()->init();
	background = new Sprite;
	background->load("assets/bg_light.png", 0, 0, 1286, 965);
}

void lm::SelectState::clear()
{
	SongList::instance()->clear();
}

void lm::SelectState::update()
{
	SongList::instance()->update();

	background->render();
	SongList::instance()->render();
}