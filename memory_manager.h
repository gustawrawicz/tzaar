#ifndef _MEMORY_MANAGER_H_
#define _MEMORY_MANAGER_H

#include "uct_node.h"

#define MAX_NODES 1<<15

class NotEnoughMemory{};

class MemoryManager{

	UCTNode * memory;
	int n;

public:

	MemoryManager(): n(0){
		memory = new UCTNode[MAX_NODES];
	};

	~MemoryManager(){
		delete [] memory;
	}
	
	void reset();
	UCTNode * getMemory(int size);


};

#endif