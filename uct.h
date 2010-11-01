#ifndef _UCT_H_
#define _UCT_H_

#include "uct_node.h"
#include "memory_manager.h"
#include "board.h"

class UCT{

	UCTNode root;
	MemoryManager mm;

public:

	void reset();
	void buildTree(Board&);
	MOVE_T getBestMoveFromTree();

private:

	void notifyPlayout(Board&, playerType);
	MOVE_T * getNextMove(MOVE_T *);
	void expand(UCTNode *, Board&);

};

#endif