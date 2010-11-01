#ifndef _MEMORY_MANAGER_H_
#define _MEMORY_MANAGER_H

#include "uct_node.h"

#define MAX_NODES 1<<7

class NotEnoughMemory{};

class MemoryManager{

	UCTNode memory[MAX_NODES];
	int n;

public:

	MemoryManager(): n(0){};
	
	void reset();
	UCTNode * getMemory(int size);


};

#endif