#include "LL1.h"

vector<string> vectorTerminal = { "$", "(", ")", "+", "-", "*", "/", "num" };
vector<string> vectorNonTerminal = { "E", "E'", "T", "T'", "F" };

void LL1::load()
{
	syntax.emplace_back(pair<string, string>("E", "TE'"));
	syntax.emplace_back(pair<string, string>("E'", "+TE'"));
	syntax.emplace_back(pair<string, string>("E'", "-TE'"));
	syntax.emplace_back(pair<string, string>("E'", "^"));
	syntax.emplace_back(pair<string, string>("T", "FT'"));
	syntax.emplace_back(pair<string, string>("T'", "*FT'"));
	syntax.emplace_back(pair<string, string>("T'", "/FT'"));
	syntax.emplace_back(pair<string, string>("T'", "^"));
	syntax.emplace_back(pair<string, string>("F", "(E)"));
	syntax.emplace_back(pair<string, string>("F", "num"));
}

bool LL1::isTerminal(string& s)
{
	if (s == "num" || s == "^" || s == "+" || s == "-" || s == "*" || s == "/" || s == "(" || s == ")") return true;
	return false;
}

vector<string> LL1::splitNonTerminal(string s)
{
	vector<string> res;

	while(s.length())
	{
		if (s[1] == '\'')
		{
			res.push_back(s.substr(0, 2));
			s = s.substr(2, s.length() - 2);
		}
		else if(s.substr(0,3) == "num")
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

vector<string> LL1::splitTerminal(string s)
{
	vector<string> res;

	while(s.length())
	{
		if(isdigit(s[0]))
		{
			int div = splitNum(s);
			res.emplace_back("num");
			s = s.substr(div, s.length() - div);
		}
		else
		{
			res.push_back(s.substr(0,1));
			s = s.substr(1, s.length() - 1);
		}
	}

	return res;
}

int LL1::splitNum(string s)
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

vector<string> LL1::getFirst(string s)
{
	vector<string> res;

	string s0;
	if (s[1] == '\'') s0 = s.substr(0, 2);
	else s0 = s.substr(0, 1);
	
	if (isTerminal(s)) res.push_back(s); //num
	else if (isTerminal(s0)) res.push_back(s0); //+ - * /
	else //非终结符
	{
		for(auto syx:syntax)
		{
			if(syx.first == s0) 
			{
				vector<string> r0 = getFirst(syx.second);
				
				if(find(r0.begin(), r0.end(), "^") != r0.end()) //首符号若可能为空 再寻找接下来串的FIRST集
				{
					vector<string> r1 = getFirst(s.substr(1, s.length() - 1));
					r0.insert(r0.end(), r1.begin(), r1.end());
				}

				res.insert(res.end(), r0.begin(), r0.end());
			}
		}

		auto iter = unique(res.begin(), res.end());
		res.erase(iter, res.end());
	}
	
	return res;
}

void LL1::getFollow()
{
	bool changed = true; //有改变 持续循环判断
	FOLLOW[0].emplace_back("$"); //起始符的FOLLOW集添加'$'
	while (changed)
	{
		changed = false;
		for (int i = 0; i < syntax.size(); i++) //遍历产生式
		{
			auto right = splitNonTerminal(syntax[i].second); //划分出产生式右侧的每个符号
			for (int j = 0; j < right.size(); ++j)
			{
				string generate = right[j];
				if (isTerminal(generate)) continue; //跳过终结符

				vector<string> temp;
				
				if (j == right.size() - 1)
				{
					temp = FOLLOW[generate];
					vector<string> tempFollow = FOLLOW[syntax[i].first];
					temp.insert(temp.end(), tempFollow.begin(), tempFollow.end());
					sort(temp.begin(), temp.end());
					auto iter = unique(temp.begin(), temp.end());
					temp.erase(iter, temp.end());

					if (temp != FOLLOW[generate])
					{
						FOLLOW[generate] = temp;
						changed = true;
					}

				}
				else
				{
					temp = FOLLOW[generate];
					string remain;
					for (int k = j + 1; k < right.size(); ++k) remain += right[k];
					
					vector<string> tempFirst = getFirst(remain);

					bool epsilon = false;
					{
						auto iter = find(tempFirst.begin(), tempFirst.end(), "^");
						if (iter != tempFirst.end())
						{
							epsilon = true;
							tempFirst.erase(iter);
						}
					}
					
					temp.insert(temp.end(), tempFirst.begin(), tempFirst.end());
					if (epsilon) //A->aBb b=epsilon : FOLLOW(A)->FOLLOW(B)
					{
						vector<string> tempFollow = FOLLOW[syntax[i].first];
						temp.insert(temp.end(), tempFollow.begin(), tempFollow.end());
					}
					sort(temp.begin(), temp.end());
					auto iter = unique(temp.begin(), temp.end());
					temp.erase(iter, temp.end());
					if (temp != FOLLOW[generate])
					{
						FOLLOW[generate] = temp;
						changed = true;
					}
				}
				temp.clear();
			}
		}
	}
}

void LL1::generateTable()
{
	table.resize(numNonTerminal);
	for (int i = 0; i < table.size(); ++i) table[i].resize(numTerminal);
	getFollow();
	
	for(auto syx : syntax)
	{
		auto first = getFirst(syx.second);
		for(auto i : first)
		{
			if (i == "^") //FIRST集含有致空符
			{
				auto follow = FOLLOW[syx.first];
				for (auto j : follow)
				{
					if (table[FOLLOW.getNum(syx.first)][getTermNum(j)].empty())
						table[FOLLOW.getNum(syx.first)][getTermNum(j)] = syx.first + "->" + syx.second;
					else error();
				}
			}
			else //FIRST集不含致空符
			{
				if (table[FOLLOW.getNum(syx.first)][getTermNum(i)].empty())
					table[FOLLOW.getNum(syx.first)][getTermNum(i)] = syx.first + "->" + syx.second;
				else error();
			}
		}
	}
}

void LL1::printTable()
{
	cout << "\t";
	for (auto i : vectorTerminal) cout << i << "\t";
	cout << endl;

	for (int i = 0; i < numNonTerminal; ++i)
	{
		cout << vectorNonTerminal[i] << "\t";
		for (int j = 0; j < numTerminal; ++j)
		{
			cout << table[FOLLOW.getNum(vectorNonTerminal[i])][getTermNum(vectorTerminal[j])] << "\t";
		}
		cout << endl;
	}
}

int LL1::getTermNum(string& index)
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

int LL1::getNonTermNum(string& s)
{
	int ret;
	try
	{
		ret = FOLLOW.getNum(s);
	}
	catch (const char* msg)
	{
		throw msg;
	}
	return ret;
}

void LL1::analyse(string s)
{
	auto input = splitTerminal(s);
	int pos = 0;
	
	stack<string> stk;
	stk.push("$");
	stk.push("E");
	
	while(!stk.empty())
	{
		while (stk.top() == "^") stk.pop();
		if (stk.top() == input[pos])
		{
			stk.pop();
			++pos;
			if (stk.empty()) break;
		}
		
		auto syx = table[getNonTermNum(stk.top())][getTermNum(input[pos])];
		if (!syx.empty()) //匹配到
		{
			cout << syx << endl;
			stk.pop();

			auto dis = syx.find("->") + 2;

			auto temp = splitNonTerminal(syx.substr(dis, syx.length() - dis));
			for (int i = temp.size() - 1; i >= 0; --i) stk.push(temp[i]);
		}
		else error();
	}
	cout << "ACC" << endl;
}

void LL1::error()
{
	throw "ERR";
}

LL1::LL1(string s)
{
	load();
	generateTable();
	//printTable();

	analyse(s + "$");
}

LL1::~LL1()
{
}
