#include "statement.h"

map<string, DefState *> DefState::funCopi;
map<string, vector<int> > FuncState::reJump;
stack<string> FuncState::onFunc;

Statement *Statement::getState(bool flag)
{
    Token token = Lexer::lexer.getCurToken();
    if(token.getContent() == "END")
    {
        Lexer::lexer.getNextToken();
        Token token1 = Lexer::lexer.getCurToken();
        return new EndState;
    }
    if(token.getContent() == "}" || token.getContent() == "else")
    {
        return nullptr;
    }
    Lexer::lexer.getNextToken();
    if(token.getContent() == "var") //变量声明语句
    {        
        Token token1 = Lexer::lexer.getCurToken();
        //为变量分配空间
        Memory::varToMem[token1.getContent()] = Memory::newForVar();
        Lexer::lexer.getNextToken();
        Token token2 = Lexer::lexer.getCurToken();
        while(token2.getContent() == ",")
        {
            Lexer::lexer.getNextToken();
            token1 = Lexer::lexer.getCurToken();
            Memory::varToMem[token1.getContent()] = Memory::newForVar();
            Lexer::lexer.getNextToken();
            token2 = Lexer::lexer.getCurToken();
        }
        Lexer::lexer.getNextToken();
        return new VarState(getState());
    }
    if(token.getContent() == "def") //函数定义语句
    {
        Token token1 = Lexer::lexer.getCurToken();
        //为函数分配空间
        Lexer::lexer.getNextToken(4);
        Token token2 = Lexer::lexer.getCurToken();        
        vector<string> itk;
        if(token2.getContent() == "var")
        {
            Lexer::lexer.getNextToken();
            Token token3 = Lexer::lexer.getCurToken();
            itk.push_back(token3.getContent());
            Lexer::lexer.getNextToken();
            Token token4 = Lexer::lexer.getCurToken();
            while(token4.getContent() == ",")
            {
                Lexer::lexer.getNextToken();
                token3 = Lexer::lexer.getCurToken();
                itk.push_back(token3.getContent());
                Lexer::lexer.getNextToken();
                token4 = Lexer::lexer.getCurToken();
            }
            Lexer::lexer.getNextToken();
        }
        Statement *ist = getState();
        Lexer::lexer.getNextToken();
        return new DefState(getState(), ist, token1, itk);
    }
    if(token.getType() == Identifier)
    {
        Token token1 = Lexer::lexer.getCurToken();
        Lexer::lexer.getNextToken();
        if(token1.getContent() == "(") //函数调用语句
        {
            Lexer::lexer.getNextToken(2);
            return new FuncState(getState(), token);
        }
        if(token1.getContent() == "=") //赋值语句
        {
            Token token2 = Lexer::lexer.getCurToken();
            Expression *exp = nullptr, *exp1;
            exp1 = exp->readExpOrder0();
            Token token3 = Lexer::lexer.getCurToken();
            if(token3.getContent() == ";")
            {
                Lexer::lexer.getNextToken();
                if(flag)
                    return new AssignState(token, exp1, nullptr);
                return new AssignState(token, exp1, getState());
            }
        }
        printf("ERROR!");
        return nullptr;
    }
    if(token.getContent() == "if") //if语句
    {        
        Expression *exp = nullptr;
        exp = exp->readExpOrder0();
        Lexer::lexer.getNextToken();
        Statement *ithen = getState(true), *ielse = nullptr;
        Token token1 = Lexer::lexer.getCurToken();
        if(token1.getContent() == "else")
        {
            Lexer::lexer.getNextToken();
            ielse = getState(true);
        }
        if(flag)
            return new JudgeState(nullptr, ithen, ielse, exp);
        return new JudgeState(getState(), ithen, ielse, exp);
    }
    if(token.getContent() == "while") //while语句
    {
        Expression *exp = nullptr, *exp1;
        exp1 = exp->readExpOrder0();
        Lexer::lexer.getNextToken();
        Statement *ist = getState(true);
        return new CycleState(getState(), ist, exp1);
    }
    if(token.getContent() == "{") //复合语句
    {       
        Statement *ist = getState();
        Token token1 = Lexer::lexer.getCurToken();
        if(token1.getContent() == "}")
        {
            Lexer::lexer.getNextToken();
            if(flag)
                return new ComplexState(nullptr, ist);
            return new ComplexState(getState(), ist);
        }
        printf("ERROR!");
        return nullptr;
    }
    if(token.getContent() == "input") //输入语句
    {
        Token token1 = Lexer::lexer.getCurToken();
        Lexer::lexer.getNextToken();
        if(token1.getType() == Identifier)
        {
            Token token2 = Lexer::lexer.getCurToken();
            Lexer::lexer.getNextToken();
            if(flag)
                return new InputState(nullptr, token1);
            return new InputState(getState(), token1);
        }
        printf("ERROR!");
        return nullptr;
    }
    if(token.getContent() == "output") //输出语句
    {
        Token token1 = Lexer::lexer.getCurToken();
        Lexer::lexer.getNextToken();
        if(token1.getType() == Identifier)
        {
            Token token2 = Lexer::lexer.getCurToken();
            Lexer::lexer.getNextToken();
            if(flag)
                return new OutputState(nullptr, token1);
            return new OutputState(getState(), token1);
        }
        printf("ERROR!");
        return nullptr;
    }
    if(token.getContent() == "return")
    {
        Token token1 = Lexer::lexer.getCurToken();
        Lexer::lexer.getNextToken();
        if(token1.getContent() != ";")
        {
            Lexer::lexer.getNextToken();
            return new ReturnState(token1);
        }
        return new ReturnState();
    }
    if(token.getContent() == ";")
    {
        Lexer::lexer.getNextToken();
        return new VoidState(getState());
    }
    return nullptr;
}

VoidState::VoidState(Statement *ist)
{
    nextState = ist;
}

void VoidState::generateCode()
{
    if(nextState != nullptr)
        nextState->generateCode();
}

VarState::VarState(Statement *ist)
{
    nextState = ist;
}

void VarState::generateCode()
{
    if(nextState != nullptr)
        nextState->generateCode();
}

AssignState::AssignState(Token itk, Expression *iex, Statement *ist)
{
    token = itk;
    exp = iex;
    nextState = ist;
}

void AssignState::generateCode()
{
    exp->generateCode();
    Memory::clearExp();
//    printf("%d", Memory::varToMem[token.getContent().c_str()]);
    Command::addTodo(3, Expression::expToMem[exp], Memory::varToMem[token.getContent()]);
    if(nextState != nullptr)
        nextState->generateCode();
}

DefState::DefState(Statement *inx, Statement *isb, Token itk, vector<string> ivc)
{
    nextState = inx;
    subState = isb;
    token = itk;
    for(int i = 0; i < (int)ivc.size(); i++)
        tmp.push_back(ivc[i]);
    funCopi[token.getContent()] = this;
}

void DefState::generateCode()
{    
    if(nextState != nullptr)
        nextState->generateCode();
}

vector<string> DefState::getVector()
{
    return tmp;
}

Statement *DefState::getSub()
{
    return subState;
}

int DefState::getSize()
{
    return tmp.size();
}

FuncState::FuncState(Statement *inx, Token itk)
{
    token = itk;
    nextState = inx;
}

FuncState::FuncState(Statement *inx, Token itk, Token tar)
{
    nextState = inx;
    token = itk;
    target = tar;
}

void FuncState::generateCode()
{
    FuncState::onFunc.push(token.getContent());
    int num = DefState::funCopi[token.getContent()]->getSize();
    vector<string> tmp = DefState::funCopi[token.getContent()]->getVector();
    Memory::mem[200] += num;
    for(int i = 0; i < num; i++)
        Memory::varToMem[token.getContent() + tmp[i] + Token::tranToStr(Memory::mem[200])] = Memory::mem[200] - i;
    DefState::funCopi[token.getContent()]->getSub()->generateCode();
    if(target.getContent() != "")
    {
        Command::addTodo(3, 100, Memory::varToMem[target.getContent()]);
    }
    for(int i = 0; i < (int)FuncState::reJump[FuncState::onFunc.top()].size(); i++)
    {
        Command::todoList[FuncState::reJump[FuncState::onFunc.top()][i]][1] = Command::cmdIdx;
    }
    FuncState::reJump[FuncState::onFunc.top()].clear();
    FuncState::onFunc.pop();
    Memory::mem[200] -= num;
    if(nextState != nullptr)
        nextState->generateCode();
}

JudgeState::JudgeState(Statement *inx, Statement *ith, Statement *iel, Expression *iex)
{
    nextState = inx;
    thenState = ith;
    elseState = iel;
    exp = iex;
}

void JudgeState::generateCode()
{
    int todo = -1;
    exp->generateCode();
    Memory::clearExp();
    Command::addTodo(20, Expression::expToMem[exp], Command::cmdIdx + 2);
    todo = Command::cmdIdx;
    Command::addTodo(30);    
    thenState->generateCode();
    Command::addTodo(30);
    Command::todoList[todo][1] = Command::cmdIdx;
    todo = Command::cmdIdx - 1;
    if(elseState != nullptr)
        elseState->generateCode();
    Command::todoList[todo][1] = Command::cmdIdx;
    if(nextState != nullptr)
        nextState->generateCode();
}

CycleState::CycleState(Statement *inx, Statement *isb, Expression *iex)
{
    nextState = inx;
    subState = isb;
    exp = iex;
}

void CycleState::generateCode()
{
    int todo, start = Command::cmdIdx;
    exp->generateCode();
    Memory::clearExp();
    todo = Command::cmdIdx;
    Command::addTodo(20, Expression::expToMem[exp]);
    Command::todoList[todo][2] = Command::cmdIdx + 1;
    todo = Command::cmdIdx;
    Command::addTodo(30);
    subState->generateCode();
    Command::addTodo(30, start);
    Command::todoList[todo][1] = Command::cmdIdx;
    if(nextState != nullptr)
        nextState->generateCode();
}

ComplexState::ComplexState(Statement *inx, Statement *isb)
{
    nextState = inx;
    subState = isb;
}

void ComplexState::generateCode()
{
    subState->generateCode();
    if(nextState != nullptr)
        nextState->generateCode();
}

InputState::InputState(Statement *inx, Token itk)
{
    nextState = inx;
    token = itk;
}

void InputState::generateCode()
{
    Command::addTodo(60, Memory::varToMem[token.getContent()]);
    if(nextState != nullptr)
        nextState->generateCode();
}

OutputState::OutputState(Statement *inx, Token itk)
{
    nextState = inx;
    token = itk;
}

void OutputState::generateCode()
{
    Command::addTodo(50, Memory::varToMem[token.getContent()]);
    if(nextState != nullptr)
        nextState->generateCode();
}

ReturnState::ReturnState()
{

}

ReturnState::ReturnState(Token itk)
{
    token = itk;
}

void ReturnState::generateCode()
{
    if(token.getContent() != "")
    {
        Command::addTodo(3, Memory::varToMem[token.getContent()], 100);
    }
    FuncState::reJump[FuncState::onFunc.top()].push_back(Command::cmdIdx);
    Command::addTodo(30);
}

EndState::EndState()
{

}

void EndState::generateCode()
{
    Command::addTodo(-1);
}
