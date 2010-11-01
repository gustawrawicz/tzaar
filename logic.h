#ifndef _LOGIC_H_
#define _LOGIC_H_

#include "directions.h"

/*those enums are extremaly important: most tables in Board are indexed by their values*/
/*also numerical values of those enums are encoded in bit arrays (FIELD_T, MOVE_T and similar)*/
/*changing anything (for example order) will take very bad effects*/

enum pawnType {tzaar_t=0, tzarras_t, tott_t};
enum playerType {black_t=0, white_t};
enum phaseType {beginning_t=0, blackCapture_t, blackChoice_t, whiteCapture_t, whiteChoice_t};
enum moveType {blackCapturingMove_t=0, blackReinforcingMove_t, whiteCapturingMove_t, whiteReinforcingMove_t};
enum fieldType {blackField_t=0, whiteField_t, wallField_t, emptyField_t};

#define NOT_PAWN_BIT 2	//bit which is lit in fieldType only when this field is not a pawn
#define NEXT_PHASE(phase) ((phaseType)(((int)(phase))%4 + 1))
#define MOVE_TYPE_FROM_PHASE(phase) ((((phase)==beginning_t)?whiteCapturingMove_t:((moveType)(((((int)(phase))+1)/2)&(~1)))))
						//upper macro returns only capturing moves!
#define CHOICE_PHASE(phase) ((((phase)==blackChoice_t)||(phase)==whiteChoice_t)?true:false)
#define NEXT_MOVE(move) ((moveType)(((int)(move)+1)%4))
#define ACTIVE_PLAYER(phase) (((phase)==beginning_t)?white_t:((playerType)((((int)(phase))+1)/4)))

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