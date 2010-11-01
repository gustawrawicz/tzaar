#include "ai.h"

MOVE_T AI::generateMove(playerType pt){
	uct.reset();
	uct.buildTree(b);
	return uct.getBestMoveFromTree();
}

void AI::init(Board& b_){
	b.load(b_);
}