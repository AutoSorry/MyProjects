#ifndef MEMORY_H
#define MEMORY_H

#include <map>
using namespace std;

class Memory
{
    static int varNum;
    static int expNum;
public:
    Memory();
    static int newForVar();
    static int newForExp();
    static void clearExp();
    static int mem[10000];
    static map<string, int> varToMem;
    static const int zero;
};

#endif // MEMORY_H
