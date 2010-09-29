#ifndef _LOGIC_H_
#define _LOGIC_H_

#include "directions.h"

enum pawnType {tzaar_t, tzarras_t, tott_t, dummy_pawn_t};
enum playerType {white_t, black_t, dummy_player_t};
enum phaseType {beginning_t, blackCapture_t, blackChoice_t, whiteCapture_t, whiteChoice_t};
enum moveType {blackCapturingMove_t=0, blackReinforcingMove_t, whiteCapturingMove_t, whiteReinforcingMove_t};

#define MOVE_TYPES_NUMBER 4

class WinningException{
public:
	playerType winner;
};

struct DataInconsistency{
	unsigned field;
	direction dir;
};

#endif