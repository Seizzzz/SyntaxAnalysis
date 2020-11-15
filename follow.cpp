#include "follow.h"

vector<string>& Follow::operator[](string index)
{
	if (index == "E") return E;
	else if (index == "E'") return E1;
	else if (index == "T") return T;
	else if (index == "T'") return T1;
	else if (index == "F") return F;
}

vector<string>& Follow::operator[](int index)
{
    switch(index)
    {
		case 0: return E;
		case 1: return E1;
		case 2: return T;
		case 3: return T1;
		case 4: return F;
		default:
		{
			cout << "ERR" << endl;
			exit(0);
		}
    }
}

int Follow::getNum(string& index)
{
	if (index == "E") return 0;
	else if (index == "E'") return 1;
	else if (index == "T") return 2;
	else if (index == "T'") return 3;
	else if (index == "F") return 4;
	else
	{
		cout << "ERR" << endl;
		exit(0);
	}
}

Follow::Follow()
{
	
}
