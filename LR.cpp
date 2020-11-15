#include "LR.h"

LR::LR(string s)
{
	load();
	generateTable();
	printTable();

	analyse(s + "$");
}
