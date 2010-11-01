#ifndef _UCT_NODE_H_
#define _UCT_NODE_H_

#include <stddef.h> /*WTF*/

#include "moves.h"
#include "params.h"

class UCTNode{

	float played;
	float won;
	float ratio;
	UCTNode * firstSon;
	int sonsCount;
	MOVE_T myMove;

public:

	UCTNode():	played(Params::initPlayed),
				won(Params::initWin),
				ratio(Params::initWin/Params::initPlayed),
				firstSon(NULL),
				sonsCount(0){}

	UCTNode(MOVE_T move):	played(Params::initPlayed),
							won(Params::initWin),
							ratio(Params::initWin/Params::initPlayed),
							firstSon(NULL),
							sonsCount(0),
							myMove(move){}

	MOVE_T getMyMove();
	int getSonsCount();
	UCTNode * getFirstSon();
	float getPlayed();
	float getRatio();
	void reset();
	UCTNode * getNext(playerType);
	void update(playerType);
	void setSons(UCTNode *, int);
	void setMove(MOVE_T, int);

private:

	bool hasGreaterFormula(UCTNode *, playerType);

};

#endif