#include "system.h"

int main()
{
	fr::System::instance()->init();
	fr::System::instance()->run();
	fr::System::instance()->clear();
	return 0;
}