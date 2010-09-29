#ifndef _MOVES_H_ 
#define _MOVES_H_

#include "logic.h"
#include "directions.h"

#define MOVE_T unsigned
#define OFFSET_FOR_DESTINATION 16
#define SOURCE_MASK 0x0fff
#define DESTINATION_MASK 0xfff0000
#define BLACK_MOVE 0x10000000
#define WHITE_MOVE 0
#define CAPTURE_MOVE 0x20000000
#define REINFORCEMENT_MOVE 0
#define MOVE_TYPE_FLAG 0x30000000
#define DIRECTION_MASK 0xf000
#define OFFSET_FOR_DIRECTION 12

class Moves{

public:

	static const MOVE_T initialMove = 0;

	static MOVE_T getMove(unsigned source, unsigned destination, moveType move, direction dir){
		MOVE_T m = (destination<<OFFSET_FOR_DESTINATION) | source | (dir<<OFFSET_FOR_DIRECTION);
		switch(move){
			case blackCapturingMove_t:
				m |= BLACK_MOVE | CAPTURE_MOVE;
				break;
			case whiteCapturingMove_t:
				m |= WHITE_MOVE | CAPTURE_MOVE;
				break;
			case blackReinforcingMove_t:
				m |= BLACK_MOVE | REINFORCEMENT_MOVE;
				break;
			case whiteReinforcingMove_t:
				m |= WHITE_MOVE | REINFORCEMENT_MOVE;
				break;
			default:
				assert(false);
				break;
		}
		return m;
	}

	static unsigned getSource(MOVE_T move){
		return move & SOURCE_MASK;
	}

	static unsigned getDestination(MOVE_T move){
		return (move & DESTINATION_MASK) >> OFFSET_FOR_DESTINATION;
	}

	static direction getDirection(MOVE_T move){
		return (direction)((move & DIRECTION_MASK) >> OFFSET_FOR_DIRECTION);
	}

	static moveType getMoveType(MOVE_T move){
		switch(move&MOVE_TYPE_FLAG){
			case WHITE_MOVE | CAPTURE_MOVE:
				return whiteCapturingMove_t;
			case BLACK_MOVE | CAPTURE_MOVE:
				return blackCapturingMove_t;
			case WHITE_MOVE | REINFORCEMENT_MOVE:
				return whiteReinforcingMove_t;
			case BLACK_MOVE | REINFORCEMENT_MOVE:
				return blackReinforcingMove_t;
			default:
				assert(false);
				return (moveType)0;

		}
	}


};

#undef DESTINATION_MASK

#endif