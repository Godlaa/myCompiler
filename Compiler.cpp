#include "ÑToken.h"

int main() {
	Token* cur = nullptr;
	std::string pascalCode = "program HelloWorld; begin writeln('Hello, world!'); end.";
	while(cur)
	{
		cur = getNextToken(0, pascalCode);
		if (cur) cur->Print();
	}


	return 0;
}