#include "ÑToken.h"

int main() {
	CToken* cur = nullptr;
	do
	{
		cur = next();
		if (cur) Print(cur);
	} while (cur);


	return 0;
}