#ifndef _MOVE_POINTERS_H_
#define _MOVE_POINTERS_H_

#include "logic.h"

#define DESTINATION_MASK 0xffff
#define INDEX_MASK 0xfff0000
#define OFFSET_FOR_INDEX 16
#define ACTION_TYPE_MASK 0x30000000
#define OFFSET_FOR_ACTION_TYPE 28

#define MOVEPTR_T unsigned

class MovePointers{
	public:

	static const MOVEPTR_T initialMovePtr = 0;

	static MOVEPTR_T getMovePtr(unsigned indexInMovesTable, unsigned destination, moveType move){
		return (indexInMovesTable<<OFFSET_FOR_INDEX) | destination | (move<<OFFSET_FOR_ACTION_TYPE);
	}

	static unsigned getIndex(MOVEPTR_T ptr){
		return (ptr & INDEX_MASK) >> OFFSET_FOR_INDEX;
	}

	static unsigned getDestination(MOVEPTR_T ptr){
		return ptr & DESTINATION_MASK;
	}

	static moveType getMoveType(MOVEPTR_T ptr){
		return (moveType) ((ptr&ACTION_TYPE_MASK)>>OFFSET_FOR_ACTION_TYPE);
	}

	static MOVEPTR_T setIndex(MOVEPTR_T ptr, unsigned index){
		return (ptr & ~INDEX_MASK) | (index << OFFSET_FOR_INDEX);
	}

	static void debug(MOVEPTR_T mv);
};

#endif