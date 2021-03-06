#ifndef _MOVE_POINTERS_H_
#define _MOVE_POINTERS_H_

#include "logic.h"

#define DESTINATION_MASK 0xffff
#define OFFSET_FOR_DESTINATION_MASK 0
#define INDEX_MASK 0xfff0000
#define OFFSET_FOR_INDEX 16
#define ACTION_TYPE_MASK 0x30000000
#define OFFSET_FOR_ACTION_TYPE 28
#define ACTIVATION_BIT 0x40000000

#define MOVEPTR_T unsigned

class MovePointers{

public:

	static const MOVEPTR_T initialMovePtr = 0;

	static MOVEPTR_T getMovePtr(unsigned indexInMovesTable, unsigned destination, moveType move){
		return	(indexInMovesTable<<OFFSET_FOR_INDEX) | 
				(destination<<OFFSET_FOR_DESTINATION_MASK) |
				(move<<OFFSET_FOR_ACTION_TYPE) |
				ACTIVATION_BIT;
	}

	static MOVEPTR_T getInactiveMovePtr(unsigned destination){
		return destination;
	}

	static unsigned getIndex(MOVEPTR_T ptr){
		MY_ASSERT(ptr & ACTIVATION_BIT);
		return (ptr & INDEX_MASK) >> OFFSET_FOR_INDEX;
	}

	static unsigned getDestination(MOVEPTR_T ptr){
		return ptr & DESTINATION_MASK;
	}

	static moveType getMoveType(MOVEPTR_T ptr){
		MY_ASSERT(ptr & ACTIVATION_BIT);
		return (moveType) ((ptr&ACTION_TYPE_MASK)>>OFFSET_FOR_ACTION_TYPE);
	}

	static MOVEPTR_T setIndex(MOVEPTR_T ptr, unsigned index){
		MY_ASSERT(ptr & ACTIVATION_BIT);
		return (ptr & ~INDEX_MASK) | (index << OFFSET_FOR_INDEX);
	}

	static bool isActive(MOVEPTR_T ptr){
		return !!(ptr & ACTIVATION_BIT);
	}

	static void debug(MOVEPTR_T mv);
};

#undef DESTINATION_MASK
#undef OFFSET_FOR_DESTINATION_MASK
#undef INDEX_MASK
#undef OFFSET_FOR_INDEX
#undef ACTION_TYPE_MASK
#undef OFFSET_FOR_ACTION_TYPE
#undef ACTIVATION_BIT

#endif