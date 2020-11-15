#pragma once
#include <string>
using namespace std;

class LR
{
public:
	void load();
	void generateTable();
	void printTable();

	void analyse(string);
	
	LR(string);
};