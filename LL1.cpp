#include "LL1.h"

vector<vector<string>> Follow(5);
map<int, string> rowMap = { {0,"E"},{1,"E'"},{2,"T"},{3,"T'"},{4,"F"} };
map<string, int> rowMapReverse = { {"E",0},{"E'",1},{"T",2},{"T'",3},{"F",4} };

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

vector<string> LL1::divide(string s)
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

vector<string> LL1::getFirst(string s)
{
	vector<string> res;

	string s0;
	if (s[1] == '\'') s0 = s.substr(0, 2);
	else s0 = s.substr(0, 1);
	
	if (isTerminal(s)) res.push_back(s);
	else if (isTerminal(s0)) res.push_back(s0);
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
	Follow[0].emplace_back("$"); //起始符的FOLLOW集添加'$'
	while (changed)
	{
		changed = false;
		for (int i = 0; i < syntax.size(); i++) //遍历产生式
		{
			auto right = divide(syntax[i].second); //划分出产生式右侧的每个符号
			for (int j = 0; j < right.size(); ++j)
			{
				string generate = right[j];
				if (isTerminal(generate)) continue; //跳过终结符

				vector<string> followset;
				
				if (j == right.size() - 1)
				{
					followset = Follow[rowMapReverse.find(generate)->second];
					vector<string> tempFollow = Follow[rowMapReverse.find(syntax[i].first)->second];
					followset.insert(followset.end(), tempFollow.begin(), tempFollow.end());
					sort(followset.begin(), followset.end());
					auto it = unique(followset.begin(), followset.end());              //将新的follow集插入进去
					followset.erase(it, followset.end());

					if (followset != Follow[rowMapReverse.find(generate)->second])  //判断有误更新
					{
						Follow[rowMapReverse.find(generate)->second] = followset;
						changed = true;
					}

				}
				else                                                                        //不是在生成式末尾
				{
					followset = Follow[rowMapReverse.find(generate)->second];    //原来的follow
					string remain;
					for (int k = j + 1; k < right.size(); ++k) remain += right[k];
					
					vector<string> firstset = getFirst(remain);

					bool epsilon = false;
					{
						auto iter = find(firstset.begin(), firstset.end(), "^");
						if (iter != firstset.end())
						{
							epsilon = true;
							firstset.erase(iter);
						}
					}
					
					followset.insert(followset.end(), firstset.begin(), firstset.end());
					if (epsilon)                                                             //有空字符串，加入生成式左边的非终结符的Follow集合
					{
						vector<string> tempFollow = Follow[rowMapReverse.find(syntax[i].first)->second];
						followset.insert(followset.end(), tempFollow.begin(), tempFollow.end());
					}
					sort(followset.begin(), followset.end());
					auto iter = unique(followset.begin(), followset.end());
					followset.erase(iter, followset.end());
					if (followset != Follow[rowMapReverse.find(generate)->second])
					{
						Follow[rowMapReverse.find(generate)->second] = followset;
						changed = true;
					}
				}
				followset.clear();
				generate.clear();
			}
		}
	}

	for(auto i:Follow)
	{
		for (auto j: i)
		{
			cout << j;
		}
		cout << endl;
	}
}

LL1::LL1()
{
	load();
	for (auto i:syntax)
	{
		cout << i.first << "->" << i.second << endl;
	}
}
