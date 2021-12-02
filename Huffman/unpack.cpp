#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <fstream>
#include <unordered_map>
using namespace std;

bool FindCode(string s, unordered_map<char, string> code)
{
	for(auto pair: code)
	{
		if(pair.second == s)
		{
			return true;
		}
	}
	return false;
}
char FindLatter(string s, unordered_map<char, string> code)
{
	for(auto pair: code)
	{
		if(pair.second == s)
		{
			return pair.first;
		}
	}
	return ' ';
}

void DecodeFile()
{
	ifstream f("binary.zipper", ios::binary);
    ifstream table("table.txt", ios::binary);
	ofstream u("./unpack/unpack.txt", ios::trunc);
	unordered_map<char, string> code;
	unsigned char c;
	char bit;
	string str = "";
	int flag = 0;
	cout << "qwe";
	while(table >> c)
	{
		//table >> c;
		
		table >> bit;
		while(bit != '*')
		{
			
			str.push_back(bit);
			table >> bit;
		}
		
		code[c] = str;
		
		cout << c << " " << code[c] << endl;
		str = "";
	}

	while(!f.eof())
	{
		f >> c;
		for(int i = 0; i < 8; i++)
		{
            if(((c >> i) & 1) == 1)
			str += "1";
            else str += "0";
            
		}
		
	}
    cout << endl;
    cout << str;
    cout << endl;
	string tmpstr = "";
	for(char ch: str)
	{
        
		tmpstr += ch;
        cout << tmpstr << endl;
		if(FindCode(tmpstr, code)) {
			
			u << FindLatter(tmpstr, code);
			
			tmpstr = "";
		}
		
	}
}


int main()
{
    cout << "qe";
    DecodeFile();
    return 0;
}