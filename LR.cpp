#include "LR.h"

void LR::load()
{
}

void LR::generateTable()
{
	//
	actionTable.resize(numState);
	for (auto& i : actionTable) i.resize(numTerminal);
	gotoTable.resize(numState);
	for (auto& i : gotoTable) i.resize(numTerminal);

	//
	syntax.emplace_back(pair<string, string>("E'", "E"));
	syntax.emplace_back(pair<string, string>("E", "E+T"));
	syntax.emplace_back(pair<string, string>("E", "E-T"));
	syntax.emplace_back(pair<string, string>("E", "T"));
	syntax.emplace_back(pair<string, string>("T", "T*F"));
	syntax.emplace_back(pair<string, string>("T", "T/F"));
	syntax.emplace_back(pair<string, string>("T", "F"));
	syntax.emplace_back(pair<string, string>("F", "(E)"));
	syntax.emplace_back(pair<string, string>("F", "num"));

	//
	const int dollar = getTermNum("$");
	const int left = getTermNum("(");
	const int right = getTermNum(")");
	const int plus = getTermNum("+");
	const int minus = getTermNum("-");
	const int multi = getTermNum("*");
	const int div = getTermNum("/");
	const int num = getTermNum("num");
	const int E1 = getNonTermNum("E'");
	const int E = getNonTermNum("E");
	const int T = getNonTermNum("T");
	const int F = getNonTermNum("F");

	//
	actionTable[0][left] = "S4"; actionTable[0][num] = "S5";
	actionTable[1][dollar] = "ACC"; actionTable[1][plus] = "S6"; actionTable[1][minus] = "S7";
	actionTable[2][dollar] = actionTable[2][right] = actionTable[2][plus] = actionTable[2][minus] = "R3"; actionTable[2][multi] = "S8"; actionTable[2][div] = "S9";
	actionTable[3][dollar] = actionTable[3][right] = actionTable[3][plus] = actionTable[3][minus] = actionTable[3][multi] = actionTable[3][div] = "R6";
	actionTable[4][left] = "S4"; actionTable[4][num] = "S5";
	actionTable[5][dollar] = actionTable[5][right] = actionTable[5][plus] = actionTable[5][minus] = actionTable[5][multi] = actionTable[5][div] = "R8";
	actionTable[6][left] = "S4"; actionTable[6][num] = "S5";
	actionTable[7][left] = "S4"; actionTable[7][num] = "S5";
	actionTable[8][left] = "S4"; actionTable[8][num] = "S5";
	actionTable[9][left] = "S4"; actionTable[9][num] = "S5";
	actionTable[10][right] = "S15"; actionTable[10][plus] = "S6"; actionTable[10][minus] = "S7";
	actionTable[11][dollar] = actionTable[11][right] = actionTable[11][plus] = actionTable[11][minus] = "R1"; actionTable[11][multi] = "S8"; actionTable[11][div] = "S9";
	actionTable[12][dollar] = actionTable[12][right] = actionTable[12][plus] = actionTable[12][minus] = "R2"; actionTable[12][multi] = "S8"; actionTable[12][div] = "S9";
	actionTable[13][dollar] = actionTable[13][right] = actionTable[13][plus] = actionTable[13][minus] = actionTable[13][multi] = actionTable[13][div] = "R4";
	actionTable[14][dollar] = actionTable[14][right] = actionTable[14][plus] = actionTable[14][minus] = actionTable[14][multi] = actionTable[14][div] = "R5";
	actionTable[15][dollar] = actionTable[15][right] = actionTable[15][plus] = actionTable[15][minus] = actionTable[15][multi] = actionTable[15][div] = "R7";

	//
	gotoTable[0][E] = 1; gotoTable[0][T] = 2; gotoTable[0][F] = 3;
	gotoTable[4][E] = 10; gotoTable[4][T] = 2; gotoTable[4][F] = 3;
	gotoTable[6][T] = 11; gotoTable[6][F] = 3;
	gotoTable[7][T] = 12; gotoTable[7][F] = 3;
	gotoTable[8][F] = 13;
	gotoTable[9][F] = 14;
	gotoTable[10][F] = 15;
	
}

void LR::printTable()
{
}

int LR::getTermNum(string index)
{
	if (index == "$") return 0;
	else if (index == "(") return 1;
	else if (index == ")") return 2;
	else if (index == "+") return 3;
	else if (index == "-") return 4;
	else if (index == "*") return 5;
	else if (index == "/") return 6;
	else if (index == "num") return 7;
	else error();
}

int LR::getNonTermNum(string index)
{
	if (index == "E'") return 0;
	else if (index == "E") return 1;
	else if (index == "T") return 2;
	else if (index == "F") return 4;
	else error();
}

int LR::splitNum(string s)
{
	int state = 0;
	int p = 0;
	string str;

	while (p < s.size())
	{
		char c = s[p];
		switch (state)
		{
			case 0:
				if (isdigit(c))
					state = 2;
				break;
			case 2: //整数
				if (isdigit(c))
					state = 2;
				else if (c == '.')
					state = 3;
				else if (c == 'E')
					state = 5;
				else
					return p;
				break;
			case 3: //小数点状态
				if (isdigit(c))
					state = 4;
				else
					return p - 1;
				break;
			case 4:
				if (isdigit(c))
					state = 4;
				else if (c == 'E')
					state = 5;
				else
					return p;
				break;
			case 5: //指数
				if (c == '+' || c == '-')
					state = 6;
				else if (isdigit(c))
					state = 7;
				else
					return p;
				break;
			case 6: //指数符号
				if (isdigit(c))
					state = 7;
				else
					return p;
				break;
			case 7: //指数数字
				if (isdigit(c))
					state = 7;
				else
					return p;
				break;
		}
		p++;
	}
	return p;
}

vector<string> LR::splitNonTerminal(string s)
{
	vector<string> res;

	while (s.length())
	{
		if (s[1] == '\'')
		{
			res.push_back(s.substr(0, 2));
			s = s.substr(2, s.length() - 2);
		}
		else if (s.substr(0, 3) == "num")
		{
			res.emplace_back("num");
			s = s.substr(3, s.length() - 3);
		}
		else
		{
			res.push_back(s.substr(0, 1));
			s = s.substr(1, s.length() - 1);
		}
	}

	return res;
}

vector<string> LR::splitTerminal(string s)
{
	vector<string> res;

	while (s.length())
	{
		if (isdigit(s[0]))
		{
			int div = splitNum(s);
			res.emplace_back("num");
			s = s.substr(div, s.length() - div);
		}
		else
		{
			res.push_back(s.substr(0, 1));
			s = s.substr(1, s.length() - 1);
		}
	}

	return res;
}

void LR::analyse(string s)
{
	auto input = splitTerminal(s);
	int pos = 0;
	stack<string> stkSymbol;
	stack<int> stkState;
	stkSymbol.push("#");
	stkState.push(0);
	
	while(true)
	{
		string action = actionTable[stkState.top()][getTermNum(input[pos])];
		int numOpt;
		if (!action.empty())
		{
			if(action == "ACC")
			{
				cout << "ACC" << endl;
				exit(0);
			}
			numOpt = atoi(action.substr(1, action.length() - 1).c_str());
		}
		else error();
		
		if(action[0] == 'S') //移进
		{
			stkSymbol.push(input[pos]);
			stkState.push(numOpt);
			++pos;
		}
		else //规约
		{
			cout << syntax[numOpt].first << "->" << syntax[numOpt].second << endl;
			auto toCheck = splitNonTerminal(syntax[numOpt].second);
			for (int i = toCheck.size() - 1; i >= 0; --i)
			{
				if (toCheck[i] == stkSymbol.top())
				{
					stkSymbol.pop();
					stkState.pop();
				}
				else error();
			}
			stkSymbol.push(syntax[numOpt].first);
			stkState.push(gotoTable[stkState.top()][getNonTermNum(stkSymbol.top())]);
		}
	}
}

void LR::error()
{
	throw "ERR";
}

LR::LR(string s)
{
	load();
	generateTable();
	printTable();

	analyse(s + "$");
}
