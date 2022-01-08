#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>

#include "lexer.h"
#include "inputbuf.h"

using namespace std;

string reserved[] = { "END_OF_FILE",
    "IF", "WHILE", "DO", "THEN", "PRINT",
    "PLUS", "MINUS", "DIV", "MULT",
    "EQUAL", "COLON", "COMMA", "SEMICOLON",
    "LBRAC", "RBRAC", "LPAREN", "RPAREN",
    "NOTEQUAL", "GREATER", "LESS", "LTEQ", "GTEQ",
    "DOT", "NUM", "ID", "ERROR", "REALNUM", "BASE08NUM","BASE16NUM"
};

bool test = false;
bool ispdigit8(char i) {
 return (i >= '1' && i <= '7');
}
bool ispdigit16(char i) {
 return (i >= '1' && i <= '9') || (i >= 'A' && i <= 'F');
}
bool ispdigit(char i) {
 return (i >= '1' && i <= '9');
}
bool isdigit8(char i) {
 return (i >= '0' && i <= '7');
}
bool isdigit16(char i) {
 return ((i >= '0' && i <= '9') || (i >= 'A' && i <= 'F'));
}


#define KEYWORDS_COUNT 5
string keyword[] = { "IF", "WHILE", "DO", "THEN", "PRINT" };

void Token::Print()
{
    cout << "{" << this->lexeme << " , "
         << reserved[(int) this->token_type] << " , "
         << this->line_no << "}\n";
}

LexicalAnalyzer::LexicalAnalyzer()
{
    this->line_no = 1;
    tmp.lexeme = "";
    tmp.line_no = 1;
    tmp.token_type = ERROR;
}

bool LexicalAnalyzer::SkipSpace()
{
    char c;
    bool space_encountered = false;

    input.GetChar(c);
    line_no += (c == '\n');

    while (!input.EndOfInput() && isspace(c)) {
        space_encountered = true;
        input.GetChar(c);
        line_no += (c == '\n');
    }

    if (!input.EndOfInput()) {
        input.UngetChar(c);
    }
    return space_encountered;
}

bool LexicalAnalyzer::IsKeyword(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return true;
        }
    }
    return false;
}

TokenType LexicalAnalyzer::FindKeywordIndex(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return (TokenType) (i + 1);
        }
    }
    return ERROR;
}

Token LexicalAnalyzer::ScanNumber()
{
	if(test){
		cout << "******* Start \n";
	}
    char c;
    std::string a = "";
    std::string number = "";
    std::string base08 = "";
    std::string basse16 = "";


    input.GetChar(c);
    if (isdigit(c)) {
		 if(test){
		 cout << "Number 0 \n";
		 }

		 a = "";
		 while (!input.EndOfInput() && isdigit(c)) {
 if(test){
 cout << "Number while loop "<< c << "\n";
 }
 a += c;
 input.GetChar(c);
 if(c == '.'){
 if(test){
 cout << "Number . found ! \n";
 }
 a += c;
 input.GetChar(c);
 if(!isdigit(c)){
 if(test){
 cout << "breaking loop \n";
 }
 break;
 }

 while (!input.EndOfInput() && isdigit(c)) {

 a += c;
 input.GetChar(c);
 }
 input.UngetChar(c);
 tmp.lexeme = a;
 tmp.token_type = REALNUM;
 tmp.line_no = line_no;
 return tmp;
 }
 }
 if (!input.EndOfInput()) {
 if(test){
 cout << "Final unget real for value : " << c << " \n";
 }
 input.UngetChar(c);
 }

 if(test){
 cout << "Value outside of number loop : " << a << "\n";
 }
 // Reset input
 input.UngetString(a);
 // TODO: You can check for REALNUM, BASE08NUM and BASE16NUM here!
 // Check for Base08Num
 tmp.lexeme = "";
 input.GetChar(c);
 if(!input.EndOfInput() && (ispdigit8(c) || c == '0')){
 if(test){
 cout << "base08 entered "<< c << "\n";
 }
 if (c == '0'){
 base08 += c;
 input.GetChar(c);
 } else {
 base08 += c;
 input.GetChar(c);
		 while (!input.EndOfInput() && isdigit8(c)) {
 if(test){
 cout << "base08 while loop "<< c << "\n";
 }
 base08 += c;
 input.GetChar(c);
 }
 }
 if(c == 'x'){
 if(test){
 cout << "base08 X found ! \n";
 }
 base08 += c;
 input.GetChar(c);
 if(c == '0'){
 if(test){
 cout << "base08 0 found ! \n";
 }
 base08 += c;
 input.GetChar(c);
 if(c == '8'){
 if(test){
 cout << "base08 8 found ! \n";
 }
 base08 += c;
 tmp.lexeme = base08;
 tmp.token_type = BASE08NUM;
 tmp.line_no = line_no;
 return tmp;
 }
 }
 }
 if (!input.EndOfInput()) {
 input.UngetChar(c);
 }
 } else if (!input.EndOfInput()) {
 if(test){
 cout << "Final unget base08 for value : " << c << " \n";
 }
 input.UngetChar(c);
 }
 // Reset input
 if(test){
 cout << "Value outside of base08 loop : " << base08 << "\n";
 }
 a = base08;
 input.UngetString(a);

 // Check for Base16Num
 input.GetChar(c);
 if(!input.EndOfInput() && (ispdigit16(c) || c == '0')){
 if(test){
 cout << "base16 entered "<< c << "\n";
 }

 if (c == '0'){
 basse16 += c;
 input.GetChar(c);
 } else {
		while (!input.EndOfInput() && isdigit16(c)) {
 if(test){
 cout << "base16 while loop "<< c << "\n";
 }
 basse16 += c;
 input.GetChar(c);
 }
 }
 if(c == 'x'){
 if(test){
 cout << "base16 X found ! \n";
 }
 basse16 += c;
 input.GetChar(c);
 if(c == '1'){
 if(test){
 cout << "base16 1 found ! \n";
 }
 basse16 += c;
 input.GetChar(c);
 if(c == '6'){
 if(test){
 cout << "base16 6 found ! \n";
 }
 basse16 += c;
 tmp.lexeme = basse16;
 tmp.token_type = BASE16NUM;
 tmp.line_no = line_no;
 return tmp;
 }
 }
 }
 if (!input.EndOfInput()) {
 input.UngetChar(c);
 }
 } else if (!input.EndOfInput()) {
 if(test){
 cout << "Final unget base16 for value : " << c << " \n";
 }
 input.UngetChar(c);
 }

 // Reset input
 a = basse16;
 if(test){
 cout << "Value outside of base16 loop : " << basse16 << "\n";
 }
 input.UngetString(a);
 input.GetChar(c);
 if (isdigit(c)) {
 if (c == '0') {
 tmp.lexeme = "0";
 } else {
 tmp.lexeme = "";
 while (!input.EndOfInput() && isdigit(c)) {
 tmp.lexeme += c;
 input.GetChar(c);
 }
 if (!input.EndOfInput()) { 
		 input.UngetChar(c);
 }
 }
 // TODO: You can check for REALNUM, BASE08NUM and BASE16NUM here!
 tmp.token_type = NUM;
 tmp.line_no = line_no;
 return tmp;
 } else {
 if (!input.EndOfInput()) {
 input.UngetChar(c);
 }
 tmp.lexeme = "";
 tmp.token_type = ERROR;
 tmp.line_no = line_no;
 return tmp;
 }
 tmp.lexeme = number;
 tmp.token_type = NUM;
 tmp.line_no = line_no;
 return tmp;
 } else {
 if (!input.EndOfInput()) {
 input.UngetChar(c);
 }
 tmp.lexeme = "";
 tmp.token_type = ERROR;
 tmp.line_no = line_no;
 return tmp;
 }
}
		 

Token LexicalAnalyzer::ScanIdOrKeyword()
{
    char c;
    input.GetChar(c);

    if (isalpha(c)) {
        tmp.lexeme = "";
        while (!input.EndOfInput() && isalnum(c)) {
            tmp.lexeme += c;
            input.GetChar(c);
        }
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.line_no = line_no;
        if (IsKeyword(tmp.lexeme))
            tmp.token_type = FindKeywordIndex(tmp.lexeme);
        else
            tmp.token_type = ID;
    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
    }
    return tmp;
}

// you should unget tokens in the reverse order in which they
// are obtained. If you execute
//
//    t1 = lexer.GetToken();
//    t2 = lexer.GetToken();
//    t3 = lexer.GetToken();
//
// in this order, you should execute
//
//    lexer.UngetToken(t3);
//    lexer.UngetToken(t2);
//    lexer.UngetToken(t1);
//
// if you want to unget all three tokens. Note that it does not
// make sense to unget t1 without first ungetting t2 and t3
//
TokenType LexicalAnalyzer::UngetToken(Token tok)
{
    tokens.push_back(tok);;
    return tok.token_type;
}

Token LexicalAnalyzer::GetToken()
{
    char c;

    if (!tokens.empty()) {
        tmp = tokens.back();
        tokens.pop_back();
        return tmp;
    }
	
	string read = "";
	 if(test){
	 cout << "**** processing input **** \n";
	 while(!input.EndOfInput()){
		input.GetChar(c);
		read += c;
		cout << c;
	  }
		input.UngetString(read);
	 }

    SkipSpace();
    tmp.lexeme = "";
    tmp.line_no = line_no;
    input.GetChar(c);
    switch (c) {
        case '.':
            tmp.token_type = DOT;
            return tmp;
        case '+':
            tmp.token_type = PLUS;
            return tmp;
        case '-':
            tmp.token_type = MINUS;
            return tmp;
        case '/':
            tmp.token_type = DIV;
            return tmp;
        case '*':
            tmp.token_type = MULT;
            return tmp;
        case '=':
            tmp.token_type = EQUAL;
            return tmp;
        case ':':
            tmp.token_type = COLON;
            return tmp;
        case ',':
            tmp.token_type = COMMA;
            return tmp;
        case ';':
            tmp.token_type = SEMICOLON;
            return tmp;
        case '[':
            tmp.token_type = LBRAC;
            return tmp;
        case ']':
            tmp.token_type = RBRAC;
            return tmp;
        case '(':
            tmp.token_type = LPAREN;
            return tmp;
        case ')':
            tmp.token_type = RPAREN;
            return tmp;
        case '<':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = LTEQ;
            } else if (c == '>') {
                tmp.token_type = NOTEQUAL;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = LESS;
            }
            return tmp;
        case '>':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = GTEQ;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = GREATER;
            }
            return tmp;
        default:
            if (isdigit(c)) {
                input.UngetChar(c);
                return ScanNumber();
            } else if (isalpha(c)) {
                input.UngetChar(c);
                return ScanIdOrKeyword();
            } else if (input.EndOfInput())
                tmp.token_type = END_OF_FILE;
            else
                tmp.token_type = ERROR;

            return tmp;
    }
}

int main()
{
    LexicalAnalyzer lexer;
    Token token;

    token = lexer.GetToken();
    token.Print();
    while (token.token_type != END_OF_FILE)
    {
        token = lexer.GetToken();
        token.Print();
    }
}
