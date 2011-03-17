#include "memory_manager.h"

void MemoryManager::reset(){

	MY_ASSERT(true);
	n=0;

}

UCTNode * MemoryManager::getMemory(int size){

	if(n+size >= MAX_NODES){
		throw NotEnoughMemory();
	} else {
		n += size;
		return &(memory[n-size]);
	}

}