#ifndef _UCT_NODE_H_
#define _UCT_NODE_H_

#include <stddef.h>

#include "moves.h"
#include "params.h"

class UCTNode{

	float n;
	float won;
	float ratio;
	UCTNode * son;
	int sonsCount;
	MOVE_T myMove;

public:

	UCTNode():	n(Params::initPlayed),
				won(Params::initWin),
				ratio(Params::initWin/Params::initPlayed),
				son(NULL),
				sonsCount(0){}

	void expand();

};

#endif