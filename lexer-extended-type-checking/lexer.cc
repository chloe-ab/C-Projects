#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

#include "lexer.h"
#include "inputbuf.h"

using namespace std;

#define KEYWORDS_COUNT 11

int line = 0;

void Token::Print()
{
	cout << "{" << this->lexeme << " , "
		<< reserved[(int)this->token_type] << " , "
		<< this->line_no << "}\n";
}

LexicalAnalyzer::LexicalAnalyzer()
{
	this->line_no = 1;
	tmp.lexeme = "";
	tmp.line_no = 1;
	line = 1;
	tmp.token_type = TokenType::ERROR;
}

bool LexicalAnalyzer::SkipSpace()
{
	char c;
	bool space_encountered = false;

	input.GetChar(c);
	line_no += (c == '\n');
	line = line_no;

	while (!input.EndOfInput() && isspace(c))
	{
		space_encountered = true;
		input.GetChar(c);
		line_no += (c == '\n');
		line = line_no;
	}

	if (!input.EndOfInput())
	{
		input.UngetChar(c);
	}
	return space_encountered;
}

bool LexicalAnalyzer::SkipComments()
{
	bool comments = false;
	char c = '\0';
	if (input.EndOfInput())
	{
		input.UngetChar(c);
		return comments;
	}

	input.GetChar(c);

	if (c == '/')
	{
		input.GetChar(c);
		if (c == '/')
		{
			comments = true;
			while (c != '\n')
			{
				comments = true;
				input.GetChar(c);
			}
			line_no++;
			line = line_no;
			SkipComments();
		}
		else
		{
			comments = false;
			cout << "Syntax Error\n";
			exit(0);
		}
	}
	else
	{
		input.UngetChar(c);
		return comments;
	}
	return true;
}

bool LexicalAnalyzer::IsKeyword(string s)
{
	for (int i = 0; i < KEYWORDS_COUNT; i++)
	{
		if (s == keyword[i])
		{
			return true;
		}
	}
	return false;
}

TokenType LexicalAnalyzer::FindKeywordIndex(string s)
{
	for (int i = 0; i < KEYWORDS_COUNT; i++)
	{
		if (s == keyword[i])
		{
			return (TokenType)(i + 1);
		}
	}
	return TokenType::ERROR;
}

Token LexicalAnalyzer::ScanNumber()
{
	char c;
	bool realNUM = false;
	input.GetChar(c);
	if (isdigit(c))
	{
		if (c == '0')
		{
			tmp.lexeme = "0";
			input.GetChar(c);
			if (c == '.')
			{
				input.GetChar(c);

				if (!isdigit(c))
				{
					input.UngetChar(c);
				}
				else
				{
					while (!input.EndOfInput() && isdigit(c))
					{
						tmp.lexeme += c;
						input.GetChar(c);
						realNUM = true;
					}
					input.UngetChar(c);
				}
			}
			else
			{
				input.UngetChar(c);
			}
		}
		else
		{
			tmp.lexeme = "";
			while (!input.EndOfInput() && isdigit(c))
			{
				tmp.lexeme += c;
				input.GetChar(c);
			}
			if (c == '.')
			{
				input.GetChar(c);

				if (!isdigit(c))
				{
					input.UngetChar(c);
				}
				else
				{
					while (!input.EndOfInput() && isdigit(c))
					{
						tmp.lexeme += c;
						input.GetChar(c);
						realNUM = true;
					}
				}
			}
			if (!input.EndOfInput())
			{
				input.UngetChar(c);
			}
		}
		if (realNUM)
		{
			tmp.token_type = TokenType::REALNUM;
		}
		else
		{
			tmp.token_type = TokenType::NUM;
		}
		tmp.line_no = line_no;
		return tmp;
	}
	else
	{
		if (!input.EndOfInput())
		{
			input.UngetChar(c);
		}
		tmp.lexeme = "";
		tmp.token_type = TokenType::ERROR;
		tmp.line_no = line_no;
		return tmp;
	}
}

Token LexicalAnalyzer::ScanIdOrKeyword()
{
	char c;
	input.GetChar(c);

	if (isalpha(c))
	{
		tmp.lexeme = "";
		while (!input.EndOfInput() && isalnum(c))
		{
			tmp.lexeme += c;
			input.GetChar(c);
		}
		if (!input.EndOfInput())
		{
			input.UngetChar(c);
		}
		tmp.line_no = line_no;

		if (IsKeyword(tmp.lexeme))
		{
			tmp.token_type = FindKeywordIndex(tmp.lexeme);
		}
		else
		{
			tmp.token_type = TokenType::ID;
		}
	}
	else
	{
		if (!input.EndOfInput())
		{
			input.UngetChar(c);
		}
		tmp.lexeme = "";
		tmp.token_type = TokenType::ERROR;
	}
	return tmp;
}

TokenType LexicalAnalyzer::UngetToken(Token tok)
{
	tokens.push_back(tok);;
	return tok.token_type;
}

Token LexicalAnalyzer::GetToken()
{
	char c;

	if (!tokens.empty())
	{
		tmp = tokens.back();
		tokens.pop_back();
		return tmp;
	}

	SkipSpace();
	SkipSpace();
	tmp.lexeme = "";
	tmp.line_no = line_no;
	input.GetChar(c);
	switch (c)
	{
	case '!':
		tmp.token_type = TokenType::NOT;
		return tmp;
	case '+':
		tmp.token_type = TokenType::PLUS;
		return tmp;
	case '-':
		tmp.token_type = TokenType::MINUS;
		return tmp;
	case '*':
		tmp.token_type = TokenType::MULT;
		return tmp;
	case '/':
		tmp.token_type = TokenType::DIV;
		return tmp;
	case '>':
		input.GetChar(c);
		if (c == '=')
		{
			tmp.token_type = TokenType::GTEQ;
		}
		else
		{
			input.UngetChar(c);
			tmp.token_type = TokenType::GREATER;
		}
		return tmp;
	case '<':
		input.GetChar(c);
		if (c == '=')
		{
			tmp.token_type = TokenType::LTEQ;
		}
		else if (c == '>')
		{
			tmp.token_type = TokenType::NOTEQUAL;
		}
		else
		{
			input.UngetChar(c);
			tmp.token_type = TokenType::LESS;
		}
		return tmp;
	case '(':
		tmp.token_type = TokenType::LPAREN;
		return tmp;
	case ')':
		tmp.token_type = TokenType::RPAREN;
		return tmp;
	case '=':
		tmp.token_type = TokenType::EQUAL;
		return tmp;
	case ':':
		tmp.token_type = TokenType::COLON;
		return tmp;
	case ',':
		tmp.token_type = TokenType::COMMA;
		return tmp;
	case ';':
		tmp.token_type = TokenType::SEMICOLON;
		return tmp;
	case '{':
		tmp.token_type = TokenType::LBRACE;
		return tmp;
	case '}':
		tmp.token_type = TokenType::RBRACE;
		return tmp;
	default:
		if (isdigit(c))
		{
			input.UngetChar(c);
			return ScanNumber();
		}
		else if (isalpha(c))
		{
			input.UngetChar(c);
			return ScanIdOrKeyword();
		}
		else if (input.EndOfInput())
		{
			tmp.token_type = TokenType::END_OF_FILE;
		}
		else
		{
			tmp.token_type = TokenType::ERROR;
		}
		return tmp;
	}
}
