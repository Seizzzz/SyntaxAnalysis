#pragma once
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <vector>
#include <string>
using namespace std;

class LL1
{
public:
	vector<pair<string, string>> syntax;
	void load();
	bool isTerminal(string&);
	
	vector<string> divide(string);
	vector<string> getFirst(string);
	void getFollow();

	LL1();
};