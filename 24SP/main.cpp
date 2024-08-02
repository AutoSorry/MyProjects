#include <iostream>
#include <command.h>
#include <expression.h>
#include <lexer.h>
#include <statement.h>

using namespace std;

int main()
{
//    Command *cmd = nullptr;
//    cmd->getCmd();
//    Command::cmd[0]->execute();
    Lexer::lexer.getNextToken();
    Memory::mem[200] = 303;
    Statement *sts = nullptr;
    sts = sts->getState();
    sts->generateCode();
    for(int i = 0; i < (int)Command::todoList.size(); i++)
    {
        printf("%d: ", i);
        for(int j = 0; j < (int)Command::todoList[i].size(); j++)
            printf(" %d", Command::todoList[i][j]);
        printf("\n");
    }
    Command *cmd = nullptr;
    cmd->getCmd();
    Command::cmd[0]->execute();
    return 0;
}
