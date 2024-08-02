#include "lexer.h"

Lexer Lexer::lexer;

TokenType Token::getType()
{
    return this->type;
}

int Token::tranToNum()
{
    int x = 0;
    for(int i = 0; i < (int)content.size(); i++)
        x = x * 10 + (int)(content[i] - '0');
    return x;
}

string Token::tranToStr(int x)
{
    string s = "";
    while(x)
    {
        s += (char)('0' + x % 10);
        x /= 10;
    }
    return s;
}

void Token::setType()
{
    if(content == "")
    {
        this->type = EndofInput;
        return;
    }
    if(content == ";")
    {
        this->type = EndofCmd;
        return;
    }
    if(content == "output" || content == "var" || content == "input" || content == "def" \
            || content == "if" || content == "else" || content == "while" || content == "return")
    {
        this->type = Keyword;
        return;
    }
    if(isInteger())
    {
        this->type = Integer;
        return;
    }
    if(isIdentifier())
    {
        this->type = Identifier;
        return;
    }
    if(isOperator())
    {
        this->type = Operator;
        return;
    }
    printf("InValidInput!");
    return;
}

void Token::addContent(char c)
{
    content = content + c;
}

bool Token::isInteger()
{
    for(int i = 0; i < (int)content.size(); i++)
        if(!isdigit(content[i]))
            return false;
    return true;
}

bool Token::isIdentifier()
{
    if(!isalpha(content[0]) || content == "input" || content == "output" \
            || content == "return" || content == "def" || content == "var" \
            || content == "if" || content == "else" || content == "while")
        return false;
    for(int i = 1; i < (int)content.size(); i++)
        if(!isdigit(content[i]) && !isalpha(content[i]))
            return false;
    return true;
}

bool Token::isOperator()
{
    if(content == "+" || content == "-" || content == "*" || content == "/" || content == "%"\
            || content == ">" || content == "<" || content == "==" || content == ">="\
            || content == "=" || content == "?" || content == ":" || content == "<="\
            || content == "(" || content == ")" || content == ";" || content == "," \
            || content == "{" || content == "}" || content == "&&" || content == "||"\
            || content == "!")
        return true;
    return false;
}

bool Token::isComparison()
{
    if(content == ">" || content == "<" || content == "==" \
            || content == ">=" || content == "<=" || content == "!=")
        return true;
    return false;
}

string Token::getContent()
{
    return content;
}

void Lexer::skipBlank()
{
    while(afterBlank == 0)
    {
        char tmp = getchar();
        if(!isspace(tmp))
        {
            afterBlank = tmp;
            break;
        }
    }
}

bool Lexer::isOperator(char c)
{
    if(c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == ';' || c == '!' || c == '|' || c == '%'\
            || c == '>' || c == '<' || c == '?' || c == ':' || c == '(' || c == ')' || c == ',' || c == '&')
        return true;
    return false;
}

Token Lexer::getToken()
{
    this->skipBlank();
    Token nowToken;
    if(afterBlank > 0)
    {
        nowToken.addContent(afterBlank);
        while(1)
        {
            char tmp = getchar();
            if(tmp <= 0 || isspace(tmp))
            {
                afterBlank = 0;
                break;
            }
            if(isOperator(afterBlank))
            {
                if((afterBlank == '=' ||afterBlank == '>' || afterBlank == '<') && tmp == '=')
                {
                    nowToken.addContent(tmp);
                    afterBlank = 0;
                    break;
                }
                if((afterBlank == '|' && tmp == '|') || (afterBlank == '&' && tmp == '&'))
                {
                    nowToken.addContent(tmp);
                    afterBlank = 0;
                    break;
                }
                afterBlank = tmp;
                break;
            }
            if(isOperator(tmp))
            {
                afterBlank = tmp;
                break;
            }
            nowToken.addContent(tmp);
            afterBlank = 0;
        }
    }
    nowToken.setType();
    curToken = nowToken;
//    printf("This token is: %s\n", nowToken.getContent().c_str());
    return nowToken;
}

Token Lexer::getCurToken()
{
    return curToken;
}

void Lexer::getNextToken(int k)
{
    for(int i = 0; i < k; i++)
        curToken = this->getToken();
}
