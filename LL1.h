#pragma once
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <vector>
#include <stack>
#include <string>
#include <cctype>
#include "follow.h"
using namespace std;

class LL1
{
public:
	const int numTerminal = 8;
	const int numNonTerminal = 5;
	
	vector<pair<string, string>> syntax;
	void load();
	bool isTerminal(string&);
	
	vector<string> splitNonTerminal(string);
	vector<string> splitTerminal(string);
	int splitNum(string);
	vector<string> getFirst(string);
	void getFollow();

	vector<vector<string>> table;
	void generateTable();
	void printTable();
	int getTermNum(string&);
	int getNonTermNum(string&);

	void analyse(string);
	void error();

	class Follow FOLLOW;

	LL1(string);
	~LL1();
};