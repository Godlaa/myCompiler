#include "СToken.h"
#include <fstream>
int main() {
	Token* cur = nullptr;
	size_t position = 0;
	string path = "D:\\myCompiler\\PascalCode.txt";
	string line;
	ifstream file(path);
	if (file.is_open())
	{
		while (getline(file, line))
		{
			while (position < line.size()) {
				Token* token = getNextToken(position, line);
				token->Print();
			}
			position = 0;
		}
	}
	file.close();     // закрываем файл
	return 0;
}