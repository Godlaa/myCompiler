#include "ÑToken.h"
#include <fstream>
int main() {
	Token* cur = nullptr;
	size_t position = 0;
	string path = "D:\\myCompiler\\PascalCode.txt";
	string code = "";
	string tokens = "";
	string line;
	ifstream file(path);
	if (file.is_open())
	{
		while (getline(file, line))
		{
			code += line + '\n';
		}
	}
	file.close();
	file.open(path);
	cout << "Code \n" <<  code << "\n\nTokens\n";
	if (file.is_open())
	{
		while (getline(file, line))
		{
			while (position < line.size()) {
				Token* token = getNextToken(position, line); // reading tokens inline
				token->Print();
			}
			cout << '\n';
			position = 0;
		}
	}
	file.close();
	return 0;
}