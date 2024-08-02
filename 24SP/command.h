#ifndef COMMAND_H
#define COMMAND_H

#include <cstdio>
#include <memory.h>
#include <map>
#include <vector>
#include <stack>
using namespace std;

class Command{

public:
    virtual void execute() = 0;
    void getCmd();
    int ord;
    static map<int, Command*> cmd;
    static int tot, cmdIdx;
    void executeNext();
    static vector<vector<int> > todoList;
    static void addTodo(int op, int x = 0, int y = 0, int z = 0);
};

class ConstSave : public Command{

    int i, addr;

public:
    ConstSave(vector<int> vec);
    void execute() override;
};

class ArraySave : public Command{

    int x, B, idx;

public:
    ArraySave(vector<int> vec);
    void execute() override;
};

class ArrayRead : public Command{

    int B, idx, x;

public:
    ArrayRead(vector<int> vec);
    void execute() override;
};

class ValuePass : public Command{

    int x, y;

public:
    ValuePass(vector<int> vec);
    void execute() override;
};

class Plus : public Command{

    int x, y, z;

public:
    Plus(vector<int> vec);
    void execute() override;
};

class Minus : public Command{

    int x, y, z;

public:
    Minus(vector<int> vec);
    void execute() override;
};

class Multiply : public Command{

    int x, y, z;

public:
    Multiply(vector<int> vec);
    void execute() override;
};

class Divide : public Command{

    int x, y, z;

public:
    Divide(vector<int> vec);
    void execute() override;
};

class Mod : public Command{

    int x, y, z;

public:
    Mod(vector<int> vec);
    void execute() override;
};

class Equal : public Command{

    int x, y, z;

public:
    Equal(vector<int> vec);
    void execute() override;
};

class Greater : public Command{

    int x, y, z;

public:
    Greater(vector<int> vec);
    void execute() override;
};

class Less : public Command{

    int x, y, z;

public:
    Less(vector<int> vec);
    void execute() override;
};

class And : public Command{

    int x, y, z;

public:
    And(vector<int> vec);
    void execute() override;
};

class Or : public Command{

    int x, y, z;

public:
    Or(vector<int> vec);
    void execute() override;
};

class Not : public Command{

    int x, z;

public:
    Not(vector<int> vec);
    void execute() override;
};

class ConJump : public Command{

    int x, B;

public:
    ConJump(vector<int> vec);
    void execute() override;
};

class BJump : public Command{

    int B;

public:
    BJump(vector<int> vec);
    void execute() override;
};

class MxJump : public Command{

    int x;

public:
    MxJump(vector<int> vec);
    void execute() override;
};

class Write : public Command{

    int x;

public:
    Write(vector<int> vec);
    void execute() override;
};

class Read : public Command{

    int x;

public:
    Read(vector<int> vec);
    void execute() override;
};

#endif // COMMAND_H
