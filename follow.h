#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Follow
{
public:
	vector<string>& operator[](string index);
	vector<string>& operator[](int index);

	int getNum(string&);
	
	vector<string> E;
	vector<string> E1;
	vector<string> T;
	vector<string> T1;
	vector<string> F;

	Follow();
};