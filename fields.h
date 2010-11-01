#ifndef _FIELDS_H_
#define _FIELDS_H_

#include <string>
#include <iostream>

#include "my_assert.h"
#include "logic.h"

using namespace std;

#define PLAYER_TYPE_MASK 0x800
#define OFFSET_FOR_PLAYER_TYPE 11
#define FIELD_TYPE_MASK 0xc00
#define OFFSET_FOR_FIELD_TYPE 10
#define PAWN_TYPE_MASK 0x300
#define OFFSET_FOR_PAWN_TYPE 8
#define HEIGHT_MASK 0xff
#define OFFSET_FOR_HEIGHT 0

#define FIELD_T unsigned short

class Fields{

public:

	static const FIELD_T wall = (FIELD_T)(wallField_t<<OFFSET_FOR_FIELD_TYPE);
	static const FIELD_T empty = (FIELD_T)(emptyField_t<<OFFSET_FOR_FIELD_TYPE);

	static short getOccupiedValue(pawnType pwn, playerType plr, unsigned short height){
		return	(height<<OFFSET_FOR_HEIGHT) |
				(pwn<<OFFSET_FOR_PAWN_TYPE) |
				(plr<<OFFSET_FOR_FIELD_TYPE); //because white_t == whiteField_t and black_t == blackField_t
	}

	static bool isProperPawn(FIELD_T field){
		return	!((field>>OFFSET_FOR_FIELD_TYPE)&NOT_PAWN_BIT) &&
				((field&PAWN_TYPE_MASK)>>OFFSET_FOR_PAWN_TYPE != 3) &&	//because pawnType is from {0,1,2}
				((field&HEIGHT_MASK)>>OFFSET_FOR_HEIGHT > 0);
	}

	static bool canCapture(FIELD_T attacker, FIELD_T defender){
		MY_ASSERT(isProperPawn(attacker));
		if(!isProperPawn(defender)) return false;
		return ((attacker&HEIGHT_MASK) >= (defender&HEIGHT_MASK));
	}

	static bool canReinforce(FIELD_T attacker, FIELD_T defender){
		MY_ASSERT(isProperPawn(attacker));
		if(!isProperPawn(defender)) return false;
		return ((attacker&PAWN_TYPE_MASK) == (defender&PAWN_TYPE_MASK));
	}

	static string debug(FIELD_T field);

	static playerType getPawnColor(FIELD_T field){
		MY_ASSERT(!(((field&FIELD_TYPE_MASK)>>OFFSET_FOR_FIELD_TYPE)&NOT_PAWN_BIT));
		return (playerType)((field&PLAYER_TYPE_MASK)>>OFFSET_FOR_PLAYER_TYPE);
	}

	static pawnType getPawnType(FIELD_T field){
		MY_ASSERT((field&PAWN_TYPE_MASK)>>OFFSET_FOR_PAWN_TYPE != 3);
		return (pawnType)((field&PAWN_TYPE_MASK)>>OFFSET_FOR_PAWN_TYPE);
	}

	static FIELD_T increase(FIELD_T increased, FIELD_T increaser){
		return (increased & (~HEIGHT_MASK)) | ((increased & HEIGHT_MASK) + (increaser & HEIGHT_MASK));
	}

};

#undef PLAYER_TYPE_MASK
#undef OFFSET_FOR_PLAYER_TYPE
#undef FIELD_TYPE_MASK
#undef OFFSET_FOR_FIELD_TYPE
#undef PAWN_TYPE_MASK
#undef OFFSET_FOR_PAWN_TYPE
#undef HEIGHT_MASK
#undef OFFSET_FOR_HEIGHT

#endif