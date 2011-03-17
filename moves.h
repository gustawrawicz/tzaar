#ifndef _MOVES_H_ 
#define _MOVES_H_

#include <string>

#include "logic.h"
#include "directions.h"
#include "my_assert.h"

#define SOURCE_MASK 0x0fff
#define OFFSET_FOR_SOURCE 0
#define DIRECTION_MASK 0xf000
#define OFFSET_FOR_DIRECTION 12
#define DESTINATION_MASK 0xfff0000
#define OFFSET_FOR_DESTINATION 16
#define MOVE_TYPE_MASK 0x30000000
#define OFFSET_FOR_MOVE_TYPE 28
#define PLAYER_TYPE_MASK 0x20000000
#define OFFSET_FOR_PLAYER_TYPE 29

#define MOVE_T unsigned

class Moves{

public:

	static const MOVE_T initialMove = 0;

	static MOVE_T getMove(unsigned source, unsigned destination, moveType move, direction dir){
		return	(source<<OFFSET_FOR_SOURCE) |
				(destination<<OFFSET_FOR_DESTINATION) | 
				((int)move<<OFFSET_FOR_MOVE_TYPE) | 
				(dir<<OFFSET_FOR_DIRECTION);
	}

	static unsigned getSource(MOVE_T move){
		return (move & SOURCE_MASK) >> OFFSET_FOR_SOURCE;
	}

	static unsigned getDestination(MOVE_T move){
		return (move & DESTINATION_MASK) >> OFFSET_FOR_DESTINATION;
	}

	static direction getDirection(MOVE_T move){
		return (direction)((move & DIRECTION_MASK) >> OFFSET_FOR_DIRECTION);
	}

	static moveType getMoveType(MOVE_T move){
		return (moveType)((move & MOVE_TYPE_MASK) >> OFFSET_FOR_MOVE_TYPE);
	}

	static playerType getPlayerType(MOVE_T move){
		return (playerType)((move & PLAYER_TYPE_MASK) >> OFFSET_FOR_PLAYER_TYPE);
	}

};

#undef SOURCE_MASK
#undef OFFSET_FOR_SOURCE
#undef DESTINATION_MASK
#undef OFFSET_FOR_DESTINATION
#undef MOVE_TYPE_MASK
#undef OFFSET_FOR_MOVE_TYPE
#undef PLAYER_TYPE_MASK
#undef OFFSET_FOR_PLAYER_TYPE
#undef DIRECTION_MASK
#undef OFFSET_FOR_DIRECTION

#endif