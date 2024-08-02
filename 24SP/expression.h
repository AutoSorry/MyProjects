#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <cstdio>
#include <lexer.h>
#include <map>
#include <memory.h>
#include <command.h>
#include <stack>

using namespace std;

#define bug 231502003

class Expression{

public:
    Expression();
    virtual void generateCode() = 0;
    Expression *readExpOrder0();
    Expression *readExpOrder1();
    Expression *readExpOrder2();
    Expression *readExpOrder3();
    Expression *readExpOrder4();
    Expression *readExpOrder5();
    Expression *readExpOrder6();
    Expression *readExpOrder2p();
    static map<Expression *, int> expToMem;
};

class SimpleExp : public Expression{
    Expression *obj;
    string op;
    Token token;
public:
    SimpleExp(Expression *exp, Token tk);
    SimpleExp(Token tk);
    void generateCode() override;
};

class BinaryExp : public Expression{
    Expression *leftObj, *rightObj;
    string op;
public:
    BinaryExp(Expression *exp1, Expression *exp2, Token token);
    void generateCode() override;
};

class SpecialExp : public Expression{
    Expression *leftObj, *rightObj, *frontObj;
public:
    SpecialExp(Expression *front, Expression *exp1, Expression *exp2);
    void generateCode() override;
};

#endif // EXPRESSION_H
