#include "memory.h"

int Memory::mem[10000], Memory::varNum = 100, Memory::expNum = -1;
const int Memory::zero = 9999;
map<string, int> Memory::varToMem;

Memory::Memory()
{

}

int Memory::newForVar()
{
    Memory::varNum++;
    return Memory::varNum;
}

int Memory::newForExp()
{
    Memory::expNum++;
    return Memory::expNum;
}

void Memory::clearExp()
{
    Memory::expNum = -1;
}

/*
0:	60	0
1:	0	0	1
2:	10	0	1	2
3:	20	2		5
4:	5	1	0	0
5:	50	0
*/
