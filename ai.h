#ifndef _AI_H_
#define _AI_H_

#include "board.h"
#include "uct.h"

class AI{

	UCT uct;

public:

	MOVE_T generateMove(Board& b, playerType pt);

};


#endif