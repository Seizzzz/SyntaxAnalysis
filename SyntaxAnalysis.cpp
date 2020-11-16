#include <iostream>
#include "LL1.h"
#include "LR.h"
using namespace std;

int main()
{
	cout << "Enter string to analyse:" << endl;
	string s;
	cin >> s;

	cout << "======LL1======" << endl;
	try
	{
		LL1 ll(s);
	}
	catch (const char* msg)
	{
		cout << msg << endl;
	}
	cout << "==============" << endl;
	cout << endl;
	cout << "======LR======" << endl;
	try
	{
		LR lr(s);
	}
	catch (const char* msg)
	{
		cout << msg << endl;
	}
	cout << "==============" << endl;
}
