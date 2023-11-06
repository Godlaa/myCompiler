#include "ÑToken.h"
#include <fstream>
#include "InOutModule.h"
int main() {
	Lexer lex;
	InOutModule io;
	io.ReadCode();
	io.ReadTokens(lex);

	return 0;
}