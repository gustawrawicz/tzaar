#ifndef _UCT_H_
#define _UCT_H_

#include "uct_node.h"

class UCT{

	UCTNode root;

public:

	void reset();
	void buildTree();
	MOVE_T getBestMoveFromTree();

private:

	void notifyPlayout();

};

#endif