#include "command.h"

int Command::tot = 0, Command::cmdIdx = 0;
map <int, Command *> Command::cmd;
vector<vector<int> > Command::todoList;

void Command::addTodo(int op, int x, int y, int z)
{
    vector<int> tmp;
    tmp.push_back(op);
    tmp.push_back(x);
    tmp.push_back(y);
    tmp.push_back(z);
    Command::todoList.push_back(tmp);
    Command::cmdIdx++;
}

void Command::getCmd()
{
    int op = Command::todoList[Command::tot][0];
    if(op == -1)
        return;
    Command *tmp;
    switch (op) {
    case 0:
        tmp = new ConstSave(Command::todoList[Command::tot]);
        break;
    case 1:
        tmp = new ArraySave(Command::todoList[Command::tot]);
        break;
    case 2:
        tmp = new ArrayRead(Command::todoList[Command::tot]);
        break;
    case 3:
        tmp = new ValuePass(Command::todoList[Command::tot]);
        break;
    case 4:
        tmp = new Plus(Command::todoList[Command::tot]);
        break;
    case 5:
        tmp = new Minus(Command::todoList[Command::tot]);
        break;
    case 6:
        tmp = new Multiply(Command::todoList[Command::tot]);
        break;
    case 7:
        tmp = new Divide(Command::todoList[Command::tot]);
        break;
    case 8:
        tmp = new Mod(Command::todoList[Command::tot]);
        break;
    case 9:
        tmp = new Equal(Command::todoList[Command::tot]);
        break;
    case 10:
        tmp = new Greater(Command::todoList[Command::tot]);
        break;
    case 11:
        tmp = new Less(Command::todoList[Command::tot]);
        break;
    case 12:
        tmp = new And(Command::todoList[Command::tot]);
        break;
    case 13:
        tmp = new Or(Command::todoList[Command::tot]);
        break;
    case 14:
        tmp = new Not(Command::todoList[Command::tot]);
        break;
    case 20:
        tmp = new ConJump(Command::todoList[Command::tot]);
        break;
    case 30:
        tmp = new BJump(Command::todoList[Command::tot]);
        break;
    case 40:
        tmp = new MxJump(Command::todoList[Command::tot]);
        break;
    case 50:
        tmp = new Write(Command::todoList[Command::tot]);
        break;
    case 60:
        tmp = new Read(Command::todoList[Command::tot]);
        break;
    }
    Command::cmd[Command::tot] = tmp;
    tmp->ord = Command::tot;
    Command::tot++;
    this->getCmd();
}

void Command::executeNext()
{
    if(ord < Command::tot)
        Command::cmd[ord + 1]->execute();
}

ConstSave::ConstSave(vector<int> vec)
{
    i = vec[1];
    addr = vec[2];
}

void ConstSave::execute()
{
    Memory::mem[addr] = i;
    this->executeNext();
}

ArraySave::ArraySave(vector<int> vec)
{
    x = vec[1];
    B = vec[2];
    idx = vec[3];
}

void ArraySave::execute()
{
    Memory::mem[B + Memory::mem[idx]] = Memory::mem[x];
    this->executeNext();
}

ArrayRead::ArrayRead(vector<int> vec)
{
    B = vec[1];
    idx = vec[2];
    x = vec[3];
}

void ArrayRead::execute()
{
    Memory::mem[x] = Memory::mem[B + Memory::mem[idx]];
    this->executeNext();
}

ValuePass::ValuePass(vector<int> vec)
{
    x = vec[1];
    y = vec[2];
}

void ValuePass::execute()
{
    Memory::mem[y] = Memory::mem[x];
    this->executeNext();
}

Plus::Plus(vector<int> vec)
{
    x = vec[1];
    y = vec[2];
    z = vec[3];
}

void Plus::execute()
{
    Memory::mem[z] = Memory::mem[x] + Memory::mem[y];
    this->executeNext();
}

Minus::Minus(vector<int> vec)
{
    x = vec[1];
    y = vec[2];
    z = vec[3];
}

void Minus::execute()
{
    Memory::mem[z] = Memory::mem[x] - Memory::mem[y];
    this->executeNext();
}


Multiply::Multiply(vector<int> vec)
{
    x = vec[1];
    y = vec[2];
    z = vec[3];
}

void Multiply::execute()
{
    Memory::mem[z] = Memory::mem[x] * Memory::mem[y];
    this->executeNext();
}

Divide::Divide(vector<int> vec)
{
    x = vec[1];
    y = vec[2];
    z = vec[3];
}

void Divide::execute()
{
    Memory::mem[z] = Memory::mem[x] / Memory::mem[y];
    this->executeNext();
}

Mod::Mod(vector<int> vec)
{
    x = vec[1];
    y = vec[2];
    z = vec[3];
}

void Mod::execute()
{
    Memory::mem[z] = Memory::mem[x] % Memory::mem[y];
    this->executeNext();
}

Equal::Equal(vector<int> vec)
{
    x = vec[1];
    y = vec[2];
    z = vec[3];
}

void Equal::execute()
{
    Memory::mem[z] = (Memory::mem[x] == Memory::mem[y]);
    this->executeNext();
}

Greater::Greater(vector<int> vec)
{
    x = vec[1];
    y = vec[2];
    z = vec[3];
}

void Greater::execute()
{
    Memory::mem[z] = (Memory::mem[x] > Memory::mem[y]);
    this->executeNext();
}

Less::Less(vector<int> vec)
{
    x = vec[1];
    y = vec[2];
    z = vec[3];
}

void Less::execute()
{
    Memory::mem[z] = (Memory::mem[x] < Memory::mem[y]);
    this->executeNext();
}

And::And(vector<int> vec)
{
    x = vec[1];
    y = vec[2];
    z = vec[3];
}

void And::execute()
{
    Memory::mem[z] = (Memory::mem[x] && Memory::mem[y]);
    this->executeNext();
}

Or::Or(vector<int> vec)
{
    x = vec[1];
    y = vec[2];
    z = vec[3];
}

void Or::execute()
{
    Memory::mem[z] = (Memory::mem[x] || Memory::mem[y]);
    this->executeNext();
}

Not::Not(vector<int> vec)
{
    x = vec[1];
    z = vec[2];
}

void Not::execute()
{
    Memory::mem[z] = (!Memory::mem[x]);
    this->executeNext();
}

ConJump::ConJump(vector<int> vec)
{
    x = vec[1];
    B = vec[2];
}

void ConJump::execute()
{
    if(Memory::mem[x])
        Command::cmd[B]->execute();
    else
        this->executeNext();
}

BJump::BJump(vector<int> vec)
{
    B = vec[1];
}

void BJump::execute()
{
    Command::cmd[B]->execute();
}

MxJump::MxJump(vector<int> vec)
{
    x = vec[1];
}

void MxJump::execute()
{
    Command::cmd[Memory::mem[x]]->execute();
}

Write::Write(vector<int> vec)
{
    x = vec[1];
}

void Write::execute()
{
    printf("%d\n", Memory::mem[x]);
    this->executeNext();
}

Read::Read(vector<int> vec)
{
    x = vec[1];
}

void Read::execute()
{
    scanf("%d", &Memory::mem[x]);
    this->executeNext();
}
