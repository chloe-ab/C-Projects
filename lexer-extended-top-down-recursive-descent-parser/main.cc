#include "lexer.h"

int main()
{
	Parser parser;
	parser.parse_Main();
	parser.print();
	return 0;
}
