#ifndef LEXER_H
#define LEXER_H

#include <cstdio>
#include <string>
using namespace std;

enum TokenType{
    Identifier,
    Integer,
    Operator,
    Keyword,
    EndofInput,
    EndofCmd
};

class Token{
    string content = "";
    TokenType type;

public:

    void setType();
    void addContent(char c);
    bool isComparison();
    string getContent();
    bool isInteger();
    bool isOperator();
    bool isIdentifier();
    int tranToNum();
    static string tranToStr(int x);
    TokenType getType();
};

class Lexer{
    char afterBlank = 0;
    void skipBlank();
    bool isOperator(char c);
    Token curToken;
    Token getToken();

public:    
    void getNextToken(int k = 1);
    Token getCurToken();
    static Lexer lexer;
};

#endif // LEXER_H
