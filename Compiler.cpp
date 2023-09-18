#include "ÑToken.h"

int main() {
	ÑToken* cur = nullptr;
	do
	{
		cur = next();
		if (cur) Print(cur);
	} while (cur);


	return 0;
}