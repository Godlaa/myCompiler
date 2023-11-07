#include "ÑToken.h"
#include <fstream>
#include "InOutModule.h"
int main() {
	unique_ptr<Lexer> lexer = make_unique<Lexer>();
	std ::unique_ptr<Token> token = nullptr;
	do {
		token = lexer -> getNextToken();
		if (token) token->Print();
		cout << '\n';
	} while (token);
	return 0;
}