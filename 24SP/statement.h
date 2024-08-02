#ifndef STATEMENT_H
#define STATEMENT_H

#include <lexer.h>
#include <memory.h>
#include <expression.h>
#include <vector>
#include <stack>
using namespace std;

class Statement
{
public:
    Statement *getState(bool flag = false);
    virtual void generateCode() = 0;    
};

class VoidState : public Statement{

    Statement *nextState;
public:
    VoidState(Statement *ist);
    void generateCode() override;
};

class VarState : public Statement{

    Statement *nextState;
public:
    VarState(Statement *ist);
    void generateCode() override;
};

class AssignState : public Statement{

    Token token;
    Expression *exp;
    Statement *nextState;
public:
    AssignState(Token itk, Expression *iex, Statement *ist);
    void generateCode() override;
};

class DefState : public Statement{

    Token token;
    vector<string> tmp;
    Statement *nextState, *subState;
public:
    DefState(Statement *inx, Statement *isb, Token itk, vector<string> ivc);
    void generateCode() override;
    vector<string> getVector();
    Statement *getSub();
    int getSize();
    static map<string, DefState *> funCopi;
};

class FuncState : public Statement{

    Token token, target;
    Statement *nextState;
public:
    FuncState(Statement *inx, Token itk);
    FuncState(Statement *inx, Token itk, Token tar);
    void generateCode() override;\
    static map<string, vector<int> > reJump;
    static stack<string> onFunc;
};

class JudgeState : public Statement{

    Expression *exp;
    Statement *nextState, *thenState, *elseState;
public:
    JudgeState(Statement *inx, Statement *ith, Statement *iel, Expression *iex);
    void generateCode() override;
};

class CycleState : public Statement{

    Expression *exp;
    Statement *nextState, *subState;
public:
    CycleState(Statement *inx, Statement *isb, Expression *iex);
    void generateCode() override;
};

class ComplexState : public Statement{

    Statement *nextState, *subState;
public:
    ComplexState(Statement *inx, Statement *isb);
    void generateCode() override;
};

class InputState : public Statement{

    Token token;
    Statement *nextState;
public:
    InputState(Statement *inx, Token itk);
    void generateCode() override;
};

class OutputState : public Statement{

    Token token;
    Statement *nextState;
public:;
    OutputState(Statement *inx, Token itk);
    void generateCode() override;
};

class ReturnState : public Statement{

    Token token;
public:
    ReturnState();
    ReturnState(Token itk);
    void generateCode() override;
};

class EndState : public Statement{

public:
    EndState();
    void generateCode() override;
};

#endif // STATEMENT_H
