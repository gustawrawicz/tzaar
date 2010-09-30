#ifndef _FIELDS_H_
#define _FIELDS_H_

#include <string>
#include <cassert>
#include <iostream>

#include "logic.h"

using namespace std;

#define EMPTY 0
#define WALL 1<<14
#define WHITE 1<<13
#define BLACK 1<<12
#define TZAAR 1<<11
#define TZARRAS 1<<10
#define TOTT 1<<9
#define HEIGHT_MASK 0xff
#define ALL_EXCEPT_HEIGHT_MASK 0xff00

#define FIELD_T unsigned short

class Fields{

public:
	static const FIELD_T wall = WALL;
	static const FIELD_T empty = EMPTY;
	static short getOccupiedValue(pawnType pwn, playerType plr, unsigned short height){
		return getPawnValue(pwn) | getPlayerValue(plr) | height;
	}

	static bool isProperPawn(FIELD_T field){
		return	((field&WHITE) ^ (field&BLACK)) && 
				((field&TZAAR) ^ (field&TZARRAS) ^ (field&TOTT)) && 
				((field&(0xff)) > 0) &&
				(!(field&WALL));
	}

	static bool canCapture(FIELD_T attacker, FIELD_T defender){
		assert(isProperPawn(attacker));
		if(!isProperPawn(defender)) return false;
		return (!(!!(attacker&WHITE) ^ !!(defender&BLACK))) && ((attacker&HEIGHT_MASK) >= (defender&HEIGHT_MASK));
	}

	static bool canReinforce(FIELD_T attacker, FIELD_T defender){
		assert(isProperPawn(attacker));
		if(!isProperPawn(defender)) return false;
		return ((attacker&WHITE) == (defender&WHITE));
	}

	static string debug(FIELD_T field);

	static playerType getPawnColor(FIELD_T field){
		assert((field&WHITE) ^ (field&BLACK));
		if(field&WHITE) return white_t;
		return black_t;
	}

	static pawnType getPawnType(FIELD_T field){
		assert((field&TZAAR) ^ (field&TZARRAS) ^ (field&TOTT));
		if(field&TOTT) return tott_t;
		if(field&TZARRAS) return tzarras_t;
		return tzaar_t;
	}

	static FIELD_T increase(FIELD_T increased, FIELD_T increaser){
		return (increased & ALL_EXCEPT_HEIGHT_MASK) | ((increased & HEIGHT_MASK) + (increaser & HEIGHT_MASK));
	}


private:

	static FIELD_T getPawnValue(pawnType pwn){
		switch(pwn){
			case tzaar_t: return TZAAR;
			case tzarras_t: return TZARRAS;
			case tott_t: return TOTT;
			default: assert(false); return 0;
		}
	}
	static FIELD_T getPlayerValue(playerType plr){
		switch(plr){
			case white_t: return WHITE;
			case black_t: return BLACK;
			default: assert(false); return 0;
		}
	}

};

#endif