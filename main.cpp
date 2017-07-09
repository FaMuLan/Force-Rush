#include "system.h"

int main()
{
	lm::System::instance()->init();
	lm::System::instance()->run();
	lm::System::instance()->clear();
	return 0;
}