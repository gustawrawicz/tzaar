#ifndef _LOGIC_H_
#define _LOGIC_H_

#include "directions.h"

/*those enums are extremaly important: most tables in Board are indexed by their values*/

enum pawnType {tzaar_t, tzarras_t, tott_t};
enum playerType {white_t, black_t};
enum phaseType {beginning_t, blackCapture_t, blackChoice_t, whiteCapture_t, whiteChoice_t};
enum moveType {blackCapturingMove_t=0, blackReinforcingMove_t, whiteCapturingMove_t, whiteReinforcingMove_t};

#define MOVE_TYPES_NUMBER 4
#define PLAYER_TYPES_NUMBER 2
#define PAWN_TYPES_NUMBER 3

struct DataInconsistency{
	unsigned field;
	direction dir;
};

struct BrokenMove{
	unsigned src;
	unsigned dest;
};

#endif