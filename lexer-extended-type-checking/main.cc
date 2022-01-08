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

struct sTableEntry
{
	std::string name = "";
	int lineNo = 0;
	int type = 0;
	int printed = 0;
};

struct sTable
{
	sTableEntry* item = nullptr;
	sTable* prev = nullptr;
	sTable* next = nullptr;
};

LexicalAnalyzer input;
Token token;
int enumCount = 4;
sTable* symbolTable = nullptr;
std::string output = "";

int parse_body(void);

void addList(std::string name, int line, int type)
{
	if (symbolTable == NULL)
	{
		sTable* newEntry = new sTable();
		sTableEntry* newItem = new sTableEntry();

		newItem->name = name;
		newItem->lineNo = token.line_no;
		newItem->type = type;
		newItem->printed = 0;

		newEntry->item = newItem;
		newEntry->next = NULL;
		newEntry->prev = NULL;

		symbolTable = newEntry;

	}
	else
	{
		sTable* temp = symbolTable;
		while (temp->next != NULL)
		{
			if (temp->item->name == name)
			{
				temp->item->type = type;
				return;
			}
			temp = temp->next;
		}

		sTable* newEntry = new sTable();
		sTableEntry* newItem = new sTableEntry();

		newItem->name = name;
		newItem->lineNo = token.line_no;
		newItem->type = type;
		newItem->printed = 0;

		newEntry->item = newItem;
		newEntry->next = NULL;
		newEntry->prev = temp;
		temp->next = newEntry;
	}
}

int searchList(std::string n)
{
	sTable* temp = symbolTable;
	bool found = false;
	if (symbolTable == nullptr)
	{
		addList(n, token.line_no, enumCount);
		enumCount++;
		return (enumCount - 1);
	}
	else
	{
		while (temp->next != nullptr)
		{
			if (strcmp(temp->item->name.c_str(), n.c_str()) == 0)
			{
				return(temp->item->type);
			}
			else
			{
				temp = temp->next;
			}
		}
		if (strcmp(temp->item->name.c_str(), n.c_str()) == 0)
		{
			return(temp->item->type);
		}
		if (!found)
		{
			addList(n, token.line_no, enumCount);
			enumCount++;
			int t = enumCount - 1;
			return(t);
		}
	}
	return 0;
}

int parse_var_list(void)
{
	token = input.GetToken();
	int var;
	addList(token.lexeme, token.line_no, 0);

	if (token.token_type == TokenType::ID)
	{
		token = input.GetToken();
		if (token.token_type == TokenType::COMMA)
		{
			var = parse_var_list();
		}
		else if (token.token_type == TokenType::COLON)
		{
			input.UngetToken(token);
		}
		else
		{
			std::cout << "\n Syntax Error \n";
		}
	}
	else
	{
		std::cout << "\n Syntax Error \n";
	}
	return(0);
}

int parse_unaryOperator(void)
{
	token = input.GetToken();

	if (token.token_type == TokenType::NOT)
	{
		return(1);
	}
	else
	{
		std::cout << "\n Syntax Error\n";
		return(0);
	}
}

int parse_binaryOperator(void)
{
	token = input.GetToken();
	if (token.token_type == TokenType::PLUS)
	{
		return(15);
	}
	else if (token.token_type == TokenType::MINUS)
	{
		return(16);
	}
	else if (token.token_type == TokenType::MULT)
	{
		return(17);
	}
	else if (token.token_type == TokenType::DIV)
	{
		return(18);
	}
	else if (token.token_type == TokenType::GREATER)
	{
		return(20);
	}
	else if (token.token_type == TokenType::LESS)
	{
		return(23);
	}
	else if (token.token_type == TokenType::GTEQ)
	{
		return(19);
	}
	else if (token.token_type == TokenType::LTEQ)
	{
		return(21);
	}
	else if (token.token_type == TokenType::EQUAL)
	{
		return(26);
	}
	else if (token.token_type == TokenType::NOTEQUAL)
	{
		return(22);
	}
	else
	{
		std::cout << "\n Syntax Error \n";
		return(-1);
	}
}

int parse_primary(void)
{
	token = input.GetToken();
	if (token.token_type == TokenType::ID)
	{
		return(searchList(token.lexeme));
	}
	else if (token.token_type == TokenType::NUM)
	{
		return(1);
	}
	else if (token.token_type == TokenType::REALNUM)
	{
		return(2);
	}
	else if (token.token_type == TokenType::TRUE)
	{
		return(3);
	}
	else if (token.token_type == TokenType::FALSE)
	{
		return(3);
	}
	else
	{
		std::cout << "\n Syntax Error \n";
		return(0);
	}
}

bool isExpress(int c) {
	if (c != 15 && c != 16 && c != 17 && c != 18 && c != 19 && c != 20 && c != 21 && c != 22 && c != 23 && c != 26) {
		return true;
	}
	else {
		return false;
	}
}

int parse_expression(void)
{
	int var;
	token = input.GetToken();
	if (token.token_type == TokenType::ID || token.token_type == TokenType::NUM || token.token_type == TokenType::REALNUM || token.token_type == TokenType::TRUE || token.token_type == TokenType::FALSE)
	{
		input.UngetToken(token);
		var = parse_primary();
	}
	else if (token.token_type == TokenType::PLUS || token.token_type == TokenType::MINUS || token.token_type == TokenType::MULT || token.token_type == TokenType::DIV || token.token_type == TokenType::GREATER || token.token_type == TokenType::LESS || token.token_type == TokenType::GTEQ || token.token_type == TokenType::LTEQ || token.token_type == TokenType::EQUAL || token.token_type == TokenType::NOTEQUAL)
	{
		input.UngetToken(token);
		var = parse_binaryOperator();
		int lExpr = parse_expression();
		int rExpr = parse_expression();
		if ((lExpr != rExpr) || isExpress(var))
		{
			if (var == 15 || var == 16 || var == 17 || var == 18)
			{
				if (lExpr <= 2 && rExpr > 3)
				{
					updateTypes(rExpr, lExpr);
					rExpr = lExpr;
				}
				else if (lExpr > 3 && rExpr <= 2)
				{
					updateTypes(rExpr, lExpr);
					lExpr = rExpr;
				}
				else if (lExpr > 3 && rExpr > 3)
				{
					updateTypes(rExpr, lExpr);
					rExpr = lExpr;
				}
				else
				{
					std::cout << "TYPE MISMATCH " << token.line_no << " C2" << std::endl;
					exit(1);
				}
			}
			else if (var == 19 || var == 20 || var == 21 || var == 22 || var == 23 || var == 26)
			{
				if (rExpr >= 3 && lExpr >= 3)
				{
					updateTypes(rExpr, lExpr);
					return(3);
				}
				else
				{
					std::cout << "TYPE MISMATCH " << token.line_no << " C2" << std::endl;
					exit(1);
				}
			}
			else
			{
				std::cout << "TYPE MISMATCH " << token.line_no << " C2" << std::endl;
				exit(1);
			}
		}
		if (var == 19 || var == 20 || var == 21 || var == 23 || var == 26 || var == 22)
		{
			var = 3;
		}
		else
		{
			var = rExpr;
		}
	}
	else if (token.token_type == TokenType::NOT)
	{
		input.UngetToken(token);
		var = parse_unaryOperator();
		if (var == 1) 
		{
			var = parse_expression();
			if (var != 3)
			{
				std::cout << "TYPE MISMATCH " << token.line_no << " C3" << std::endl;
				exit(1);
			}
		}
	}
	else
	{
		std::cout << "\n Syntax Error \n";
		return(0);
	}
	return var;
}

void compareLine(int line_No, int token_Type)
{
	sTable* temp = symbolTable;
	while (temp->next != NULL)
	{
		if (temp->item->lineNo == line_No)
		{
			temp->item->type = token_Type;
		}
		temp = temp->next;
	}
	if (temp->item->lineNo == line_No)
	{
		temp->item->type = token_Type;
	}
}

void updateTypes(int currentType, int newType)
{
	sTable* temp = symbolTable;
	while (temp->next != NULL)
	{
		if (temp->item->type == currentType)
		{
			temp->item->type = newType;
		}
		temp = temp->next;
	}
	if (temp->item->type == currentType)
	{
		temp->item->type = newType;
	}
}

int parse_assstmt(void)
{
	std::string name;
	int LHS = 0;
	int RHS = 0;
	token = input.GetToken();
	if (token.token_type == TokenType::ID)
	{
		LHS = searchList(token.lexeme);
		token = input.GetToken();
		if (token.token_type == TokenType::EQUAL)
		{
			token = input.GetToken();
			if (token.token_type == TokenType::ID || token.token_type == TokenType::NUM || token.token_type == TokenType::REALNUM || token.token_type == TokenType::TRUE || token.token_type == TokenType::FALSE || token.token_type == TokenType::PLUS || token.token_type == TokenType::MINUS || token.token_type == TokenType::MULT || token.token_type == TokenType::DIV || token.token_type == TokenType::LESS || token.token_type == TokenType::GREATER || token.token_type == TokenType::GTEQ || token.token_type == TokenType::LTEQ || token.token_type == TokenType::EQUAL || token.token_type == TokenType::NOTEQUAL || token.token_type == TokenType::NOT)
			{
				input.UngetToken(token);
				RHS = parse_expression();

				if (LHS <= 3)
				{
					if (LHS == RHS)
					{

					}
					else
					{
						if (LHS == 3 && RHS == 6)
						{
							updateTypes(LHS, RHS);
							LHS = RHS;
						}

						else if (LHS <= 3)
						{
							std::cout << "TYPE MISMATCH " << token.line_no << " C1" << std::endl;
							exit(1);
						}
					}
				}
				else
				{
					updateTypes(LHS, RHS);
					LHS = RHS;
				}
				token = input.GetToken();
				if (token.token_type == TokenType::SEMICOLON)
				{

				}
				else
				{
					std::cout << "\n Syntax Error " << (int)token.token_type << " \n";
				}
			}
			else
			{
				std::cout << "\n Syntax Error \n";
			}
		}
		else
		{
			std::cout << "\n Syntax Error \n";
		}
	}
	else
	{
		std::cout << "\n Syntax Error \n";
	}
	return(0);
}

int parse_case(void)
{
	int var;
	token = input.GetToken();
	if (token.token_type == TokenType::CASE)
	{
		token = input.GetToken();
		if (token.token_type == TokenType::NUM)
		{
			token = input.GetToken();
			if (token.token_type == TokenType::COLON)
			{
				var = parse_body();
			}
			else
			{
				std::cout << "\n Syntax Error \n";
			}
		}
		else
		{
			std::cout << "\n Syntax Error \n";
		}
	}
	else
	{
		std::cout << "\n Syntax Error \n";
	}
	return 0;
}

int parse_caselist(void)
{
	int var;
	token = input.GetToken();
	if (token.token_type == TokenType::CASE)
	{
		input.UngetToken(token);
		var = parse_case();
		token = input.GetToken();
		if (token.token_type == TokenType::CASE)
		{
			input.UngetToken(token);
			var = parse_caselist();
		}
		else if (token.token_type == TokenType::RBRACE)
		{
			input.UngetToken(token);
		}
	}
	return(0);
}

int parse_switchstmt(void)
{
	int var;
	token = input.GetToken();
	if (token.token_type == TokenType::SWITCH)
	{
		token = input.GetToken();
		if (token.token_type == TokenType::LPAREN)
		{
			var = parse_expression();
			if (var <= 3 && var != 1)
			{
				std::cout << "TYPE MISMATCH " << token.line_no << " C5" << std::endl;
				exit(1);
			}
			token = input.GetToken();
			if (token.token_type == TokenType::RPAREN)
			{
				token = input.GetToken();
				if (token.token_type == TokenType::LBRACE)
				{
					var = parse_caselist();
					token = input.GetToken();
					if (token.token_type == TokenType::RBRACE)
					{
						
					}
					else
					{
						std::cout << "\n Syntax Error \n";
					}
				}
				else
				{
					std::cout << "\n Syntax Error \n";
				}
			}
			else
			{
				std::cout << "\n Syntax Error \n";
			}
		}
		else
		{
			std::cout << "\n Syntax Error \n";
		}
	}
	else
	{
		std::cout << "\n Syntax Error \n";
	}
	return(0);
}

int parse_whilestmt(void)
{
	int var;

	token = input.GetToken();
	if (token.token_type == TokenType::WHILE)
	{
		token = input.GetToken();
		if (token.token_type == TokenType::LPAREN)
		{
			var = parse_expression();
			if (var == 4 || var == 5)
			{
				var = 3;
			}
			if (var != 3)
			{
				std::cout << "TYPE MISMATCH " << token.line_no << " C4" << std::endl;
				exit(1);
			}
			token = input.GetToken();
			if (token.token_type == TokenType::RPAREN)
			{
				var = parse_body();
			}
			else
			{
				std::cout << "\n Syntax Error \n";
			}
		}
		else
		{
			std::cout << "\n Syntax Error \n";
		}
	}
	else
	{
		std::cout << "\n Syntax Error \n";
	}
	return(0);
}

int parse_ifstmt(void)
{
	int var;
	token = input.GetToken();
	if (token.token_type == TokenType::IF)
	{
		token = input.GetToken();
		if (token.token_type == TokenType::LPAREN)
		{
			var = parse_expression();
			if (var != 3)
			{
				std::cout << "TYPE MISMATCH " << token.line_no << " C4" << std::endl;
				exit(1);
			}
			token = input.GetToken();
			if (token.token_type == TokenType::RPAREN)
			{
				var = parse_body();

			}
			else
			{
				std::cout << "\n Syntax Error \n";
			}
		}
		else
		{
			std::cout << "\n Syntax Error \n";
		}
	}
	else
	{
		std::cout << "\n Syntax Error \n";
	}
	return(0);
}

int parse_stmt(void)
{
	int var;
	token = input.GetToken();
	if (token.token_type == TokenType::ID)
	{
		input.UngetToken(token);
		var = parse_assstmt();

	}
	else if (token.token_type == TokenType::IF)
	{
		input.UngetToken(token);
		var = parse_ifstmt();
	}
	else if (token.token_type == TokenType::WHILE)
	{
		input.UngetToken(token);
		var = parse_whilestmt();
	}
	else if (token.token_type == TokenType::SWITCH)
	{
		input.UngetToken(token);
		var = parse_switchstmt();
	}
	else
	{
		std::cout << "\n Syntax Error \n";
	}
	return(0);
}

int parse_stmtlist(void)
{
	token = input.GetToken();
	int var;
	if (token.token_type == TokenType::ID || token.token_type == TokenType::IF || token.token_type == TokenType::WHILE || token.token_type == TokenType::SWITCH)
	{
		input.UngetToken(token);
		var = parse_stmt();
		token = input.GetToken();
		if (token.token_type == TokenType::ID || token.token_type == TokenType::IF || token.token_type == TokenType::WHILE || token.token_type == TokenType::SWITCH)
		{
			input.UngetToken(token);
			var = parse_stmtlist();

		}
		else if (token.token_type == TokenType::RBRACE)
		{
			input.UngetToken(token);
		}
	}
	else
	{
		std::cout << "\n Syntax Error \n";
	}
	return(0);
}

int parse_body(void)
{
	token = input.GetToken();
	int var;
	if (token.token_type == TokenType::LBRACE)
	{
		var = parse_stmtlist();
		token = input.GetToken();
		if (token.token_type == TokenType::RBRACE)
		{
			return(0);
		}
		else
		{
			std::cout << "\n Syntax Error\n ";
			return(0);
		}
	}
	else if (token.token_type == TokenType::END_OF_FILE)
	{
		input.UngetToken(token);
		return(0);
	}
	else
	{
		std::cout << "\n Syntax Error \n ";
		return(0);
	}
}

int parse_typename(void)
{
	token = input.GetToken();
	if (token.token_type == TokenType::INT || token.token_type == TokenType::REAL || token.token_type == TokenType::BOOLEAN)
	{
		compareLine(token.line_no, (int)token.token_type);
	}
	else
	{
		std::cout << "\n Syntax Error \n";
	}
	return((int)token.token_type);
}

int parse_vardecl(void)
{
	int var;
	token = input.GetToken();
	if (token.token_type == TokenType::ID)
	{
		input.UngetToken(token);
		var = parse_var_list();
		token = input.GetToken();
		if (token.token_type == TokenType::COLON)
		{
			var = parse_typename();
			token = input.GetToken();
			if (token.token_type == TokenType::SEMICOLON)
			{
				
			}
			else
			{
				std::cout << "\n Syntax Error \n";
			}
		}
		else
		{
			std::cout << "\n Syntax Error \n";
		}
	}
	else
	{
		std::cout << "\n Syntax Error \n";
	}
	return(0);
}

int parse_vardecllist(void)
{
	int var;
	token = input.GetToken();
	while (token.token_type == TokenType::ID)
	{
		input.UngetToken(token);
		var = parse_vardecl();
		token = input.GetToken();
		if (token.token_type != TokenType::ID)
		{
			
		}
		else
		{
			
		}
	}
	input.UngetToken(token);
	return(0);
}

int parse_globalVars(void)
{
	token = input.GetToken();
	int var;
	if (token.token_type == TokenType::ID)
	{
		input.UngetToken(token);
		var = parse_vardecllist();
	}
	else
	{
		std::cout << "Syntax Error";
	}
	return(0);
}

int parse_program(void)
{
	token = input.GetToken();
	int var;
	while (token.token_type != TokenType::END_OF_FILE)
	{
		if (token.token_type == TokenType::ID)
		{
			input.UngetToken(token);
			var = parse_globalVars();
			var = parse_body();
		}
		else if (token.token_type == TokenType::LBRACE)
		{
			input.UngetToken(token);
			var = parse_body();
		}
		else if (token.token_type == TokenType::END_OF_FILE)
		{
			return(0);
		}
		else
		{
			std::cout << "\n Syntax Error\n";
			return(0);
		}
		token = input.GetToken();
	}
	return 0;
}

void printList(void)
{
	sTable* temp = symbolTable;
	int temp1;

	while (temp->next != NULL)
	{
		if (temp->item->type > 3 && temp->item->printed == 0)
		{
			temp1 = temp->item->type;
			output += temp->item->name;
			temp->item->printed = 1;
			while (temp->next != NULL)
			{
				temp = temp->next;
				if (temp->item->type == temp1)
				{
					output += ", " + temp->item->name;
					temp->item->printed = 1;
				}
				else
				{

				}
			}
			output += ": ? #";
			std::cout << output << std::endl;
			temp->item->printed = 1;
			output = "";
			temp = symbolTable;
		}
		else if (temp->item->type < 4 && temp->item->printed == 0)
		{
			std::string lCase = keyword[(temp->item->type) - 1];
			int temp1 = temp->item->type;
			output = temp->item->name + ": " + lCase + " #";
			std::cout << output << std::endl;
			output = "";
			temp->item->printed = 1;

			while (temp->next != NULL && temp->next->item->type == temp1)
			{
				temp = temp->next;
				std::string lCase2 = keyword[(temp->item->type) - 1];
				output = temp->item->name + ": " + lCase2 + " #";
				std::cout << output << std::endl;
				temp->item->printed = 1;
				output = "";
			}
		}
		else
		{
			temp = temp->next;
		}
	}
	if (temp->item->type <= 3 && temp->item->printed == 0)
	{
		std::string lCase3 = keyword[(temp->item->type) - 1];
		output += temp->item->name + ": " + lCase3 + " #";
		std::cout << output << std::endl;
		output = "";
	}
	else if (temp->item->type > 3 && temp->item->printed == 0)
	{
		output += temp->item->name + ":" + " ? " + "#";
		std::cout << output << std::endl;
		output = "";
	}
	else
	{

	}
}

int main()
{
	int p;
	p = parse_program();
	printList();
	return 0;
}
