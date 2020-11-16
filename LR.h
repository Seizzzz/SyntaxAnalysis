#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stack>
using namespace std;

class LR
{
public:
	const int numState = 16;
	const int numTerminal = 8;
	const int numNonTerminal = 4;
	vector<pair<string, string>> syntax;
	
	void load();
	void generateTable();
	void printTable();

	int getTermNum(string);
	int getNonTermNum(string);

	vector<vector<string>> actionTable;
	vector<vector<int>> gotoTable;

	int splitNum(string);
	vector<string> splitNonTerminal(string);
	vector<string> splitTerminal(string);

	void analyse(string);
	void error();
	
	LR(string);
};