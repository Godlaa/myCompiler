#include "�Token.h"
#include <fstream>
#include "InOutModule.h"
#include "Syntax.h"

extern "C" int somefunction();

int main() {
	setlocale(LC_ALL, "RUSSIAN");
	shared_ptr<Lexer> lexer = make_shared<Lexer>();
	shared_ptr<Syntax> syntax = make_shared<Syntax>(lexer);
	std ::shared_ptr<Token> token = nullptr;
	syntax->run_syntax();
	lexer->print_lex_errors();

	cout << "Res: " << somefunction();
	return 0;
}


