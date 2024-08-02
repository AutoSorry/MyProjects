#include "expression.h"

map<Expression *, int> Expression::expToMem;

Expression::Expression()
{

}

SimpleExp::SimpleExp(Expression *exp, Token tk)
{
    obj = exp;
    op = tk.getContent();
}

SimpleExp::SimpleExp(Token tk)
{
    token = tk;
}

BinaryExp::BinaryExp(Expression *exp1, Expression *exp2, Token token)
{
    op = token.getContent();
    leftObj = exp1;
    rightObj = exp2;
}

SpecialExp::SpecialExp(Expression *front, Expression *exp1, Expression *exp2)
{
    frontObj = front;
    leftObj = exp1;
    rightObj = exp2;
}

Expression *Expression::readExpOrder0()
{
//    Token token = Lexer::lexer.getCurToken();
    Expression *exp1 = readExpOrder1();
    Token curToken = Lexer::lexer.getCurToken();
    if(curToken.getContent() == ";" || curToken.getContent() == ":")
    {
//        Lexer::lexer.getNextToken();
        return exp1;
    }
    if(curToken.getContent() == ")")
    {
//        Lexer::lexer.getNextToken();
        return exp1;
    }
//    if(curToken.isComparison())
//    {
//        Lexer::lexer.getNextToken();
//        Expression *exp2 = readExpOrder1();
//        Token token1 = Lexer::lexer.getCurToken();
//        if(token1.getContent() != "?")
//        {
//            return new BinaryExp(exp1, exp2, curToken);
//        }
//        Expression *exp3 = readExpOrder1();
//        Token token2 = Lexer::lexer.getCurToken();
//        if(token2.getContent() != ":")
//            return NULL;
//        Expression *exp4 = readExpOrder1();
//        Token token3 = Lexer::lexer.getCurToken();
//        if(token3.getContent() == ")")
//            Lexer::lexer.getNextToken();
//        Expression *front = new BinaryExp(exp1, exp2, curToken);
//        Expression *exp = new SpecialExp(front, exp3, exp4);
//        return exp;
//    }
//    else
        return exp1;
}

Expression *Expression::readExpOrder1()
{
    Expression *exp1 = readExpOrder2();
    Token token = Lexer::lexer.getCurToken();
    if(token.getContent() == ")" || token.getContent() == ";")
        return exp1;
    while(token.getContent() == "||")
    {
        Lexer::lexer.getNextToken();
        Expression *exp2 = readExpOrder2();
        exp1 = new BinaryExp(exp1, exp2, token);
        token = Lexer::lexer.getCurToken();
        if(token.getContent() == ")" || token.getContent() == ";")
            break;
    }
    return exp1;
}

Expression *Expression::readExpOrder2()
{
    Expression *exp1 = readExpOrder2p();
    Token token = Lexer::lexer.getCurToken();
    if(token.getContent() == ")" || token.getContent() == ";")
        return exp1;
    while(token.getContent() == "&&")
    {
        Lexer::lexer.getNextToken();
        Expression *exp2 = readExpOrder2p();
        exp1 = new BinaryExp(exp1, exp2, token);
        token = Lexer::lexer.getCurToken();
        if(token.getContent() == ")" || token.getContent() == ";")
            break;
    }
    return exp1;
}

Expression *Expression::readExpOrder2p()
{
    Expression *exp1 = readExpOrder3();
    Token token = Lexer::lexer.getCurToken();
    if(token.getContent() == ")" || token.getContent() == ";")
        return exp1;
    while(token.isComparison())
    {
        Lexer::lexer.getNextToken();
        Expression *exp2 = readExpOrder3();
        exp1 = new BinaryExp(exp1, exp2, token);
        token = Lexer::lexer.getCurToken();
        if(token.getContent() == ")" || token.getContent() == ";")
            break;
    }
    return exp1;
}

Expression *Expression::readExpOrder3()
{
    Expression *exp1 = readExpOrder4();
    Token token = Lexer::lexer.getCurToken();
    if(token.getContent() == ")" || token.getContent() == ";")
        return exp1;
    while(token.getContent() == "+" || token.getContent() == "-")
    {
        Lexer::lexer.getNextToken();
        Expression *exp2 = readExpOrder4();
        exp1 = new BinaryExp(exp1, exp2, token);
        token = Lexer::lexer.getCurToken();
        if(token.getContent() == ")" || token.getContent() == ";")
            break;
    }
    return exp1;
}

Expression *Expression::readExpOrder4()
{
    Expression *exp1 = readExpOrder5();
    Token token = Lexer::lexer.getCurToken();
    if(token.getContent() == ")" || token.getContent() == ";")
        return exp1;
    while(token.getContent() == "*" || token.getContent() == "/" || token.getContent() == "%")
    {
        Lexer::lexer.getNextToken();
        Expression *exp2 = readExpOrder5();
        exp1 = new BinaryExp(exp1, exp2, token);
        token = Lexer::lexer.getCurToken();
        if(token.getContent() == ")" || token.getContent() == ";")
            break;
    }
    return exp1;
}

Expression *Expression::readExpOrder5()
{
    Token token = Lexer::lexer.getCurToken();
    if(token.getContent() == "-" || token.getContent() == "!")
    {
        Lexer::lexer.getNextToken();
        Token token1 = Lexer::lexer.getCurToken();
        Expression *exp1 = readExpOrder6();
        Expression *exp = new SimpleExp(exp1, token);
        return exp;
    }
    return readExpOrder6();
}

Expression *Expression::readExpOrder6()
{
    Expression *exp;
    Token token = Lexer::lexer.getCurToken();
    if(token.getContent() == "(")
    {
        Lexer::lexer.getNextToken();
        exp = readExpOrder0();
    }
    else
        exp = new SimpleExp(token);
    Lexer::lexer.getNextToken();
    return exp;
}

void SimpleExp::generateCode()
{
    expToMem[this] = Memory::newForExp();
    if(op == "-")
    {
        this->obj->generateCode();
        Command::addTodo(5, Memory::zero, Expression::expToMem[obj], Expression::expToMem[this]);
    }
    if(op == "!")
    {
        this->obj->generateCode();
        Command::addTodo(14, Expression::expToMem[obj], Expression::expToMem[this]);
    }
    if(token.getType() == Identifier)
        Command::addTodo(3, Memory::varToMem[token.getContent()], Expression::expToMem[this]);
    else
        if(token.getType() == Integer)
            Command::addTodo(0, token.tranToNum(), Expression::expToMem[this]);
        else
            printf("%d\n", bug);
}

void BinaryExp::generateCode()
{
    expToMem[this] = Memory::newForExp();
    this->leftObj->generateCode();
    this->rightObj->generateCode();
    if(op == "+")
        Command::addTodo(4, Expression::expToMem[leftObj], Expression::expToMem[rightObj], Expression::expToMem[this]);
    if(op == "-")
        Command::addTodo(5, Expression::expToMem[leftObj], Expression::expToMem[rightObj], Expression::expToMem[this]);
    if(op == "*")
        Command::addTodo(6, Expression::expToMem[leftObj], Expression::expToMem[rightObj], Expression::expToMem[this]);
    if(op == "/")
        Command::addTodo(7, Expression::expToMem[leftObj], Expression::expToMem[rightObj], Expression::expToMem[this]);
    if(op == "%")
        Command::addTodo(8, Expression::expToMem[leftObj], Expression::expToMem[rightObj], Expression::expToMem[this]);
    if(op == "==")
        Command::addTodo(9, Expression::expToMem[leftObj], Expression::expToMem[rightObj], Expression::expToMem[this]);
    if(op == ">")
        Command::addTodo(10, Expression::expToMem[leftObj], Expression::expToMem[rightObj], Expression::expToMem[this]);
    if(op == "<")
        Command::addTodo(11, Expression::expToMem[leftObj], Expression::expToMem[rightObj], Expression::expToMem[this]);
    if(op == "&&")
        Command::addTodo(12, Expression::expToMem[leftObj], Expression::expToMem[rightObj], Expression::expToMem[this]);
    if(op == "||")
        Command::addTodo(13, Expression::expToMem[leftObj], Expression::expToMem[rightObj], Expression::expToMem[this]);
    if(op == ">=")
    {
        Command::addTodo(11, Expression::expToMem[leftObj], Expression::expToMem[rightObj], Expression::expToMem[this]);
        Command::addTodo(14, Expression::expToMem[this], Expression::expToMem[this]);
    }
    if(op == "<=")
    {
        Command::addTodo(10, Expression::expToMem[leftObj], Expression::expToMem[rightObj], Expression::expToMem[this]);
        Command::addTodo(14, Expression::expToMem[this], Expression::expToMem[this]);
    }
    if(op == "!=")
    {
        Command::addTodo(9, Expression::expToMem[leftObj], Expression::expToMem[rightObj], Expression::expToMem[this]);
        Command::addTodo(14, Expression::expToMem[this], Expression::expToMem[this]);
    }
}

void SpecialExp::generateCode()
{
    int todo;
    Memory::clearExp();
    Expression::expToMem.clear();
    expToMem[this] = Memory::newForExp();
    this->frontObj->generateCode();
    todo = Command::cmdIdx;
    Command::addTodo(20, Expression::expToMem[frontObj], todo);
    this->rightObj->generateCode();
    Command::todoList[todo][2] = Command::cmdIdx + 1;
    todo = Command::cmdIdx;
    Command::addTodo(30);
    this->leftObj->generateCode();
    Command::todoList[todo][1] = Command::cmdIdx;
}
