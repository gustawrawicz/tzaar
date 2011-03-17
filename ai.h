#ifndef _AI_H_
#define _AI_H_

#include "board.h"
#include "uct.h"

class AI{

	UCT uct;
	Board b;

public:

	MOVE_T generateMove();
	void init(Board&);

};


#endif