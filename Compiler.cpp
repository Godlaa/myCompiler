#include "ÑToken.h"
#include <fstream>
int main() {
	Token* cur = nullptr;
	Lexer lex;
	InOutModule io;
	io.ReadCode();
	io.ReadTokens(lex);

	return 0;
}