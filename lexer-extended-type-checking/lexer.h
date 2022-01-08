#pragma once

#include <vector>
#include <string>

#include "inputbuf.h"

const std::string keyword[] = { "int", "real", "bool", "true",
					 "false", "if", "while", "switch",
					 "case", "public", "private" };

enum class TokenType : int
{
	END_OF_FILE, INT, REAL, BOOLEAN,
	TRUE, FALSE, IF, WHILE, SWITCH, CASE,
	PUBLIC, PRIVATE, NUM, REALNUM, NOT,
	PLUS, MINUS, MULT, DIV, GTEQ, GREATER,
	LTEQ, NOTEQUAL, LESS, LPAREN, RPAREN,
	EQUAL, COLON, COMMA, SEMICOLON, LBRACE,
	RBRACE, ID, ERROR
};

const std::string reserved[] = { "END_OF_FILE", "INT", "REAL", "BOOL",
					  "TRUE", "FALSE", "IF", "WHILE", "SWITCH",
					  "CASE", "PUBLIC", "PRIVATE", "NUM",
					  "REALNUM", "NOT", "PLUS", "MINUS", "MULT",
					  "DIV", "GTEQ", "GREATER", "LTEQ", "NOTEQUAL",
					  "LESS", "LPAREN", "RPAREN", "EQUAL", "COLON",
					  "COMMA", "SEMICOLON", "LBRACE", "RBRACE", "ID",
					  "ERROR"
};

class Token {
public:
	void Print();

	std::string lexeme;
	TokenType token_type = TokenType::END_OF_FILE;
	int line_no = 0;
};

class LexicalAnalyzer {
public:
	Token GetToken();
	TokenType UngetToken(Token);
	LexicalAnalyzer();

private:
	std::vector<Token> tokens;
	int line_no;
	Token tmp;
	InputBuffer input;

	bool SkipSpace();
	bool SkipComments();
	bool IsKeyword(std::string);
	TokenType FindKeywordIndex(std::string);
	Token ScanIdOrKeyword();
	Token ScanNumber();
};

void updateTypes(int LHS, int RHS);
void addList(std::string n, int lN, int t);
int searchList(std::string n);
