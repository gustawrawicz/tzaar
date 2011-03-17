#include "uct.h"


void UCT::reset(){
	mm.reset();
	root.reset();
}

void UCT::buildTree(Board& b){

	expand(&root,b);
	
	playerType movingPlayer = b.getMovingPlayer();
	for(int i=0; i<Params::playoutsNo; i++){
		notifyPlayout(b, movingPlayer);
	}

}

void UCT::notifyPlayout(Board& b, playerType movingPlayer){

	Board tmpBoard;
	tmpBoard.load(b);

	UCTNode * active = &root;

	while(active->getFirstSon() != NULL){
		active = active->getNext(movingPlayer);
		tmpBoard.doSingleMove(active->getMyMove());
	}

	if(active->getPlayed() >= Params::playoutsToExpand)
		expand(active, tmpBoard);

	if(active->getSonsCount() > 0){
		active = active->getFirstSon();
		tmpBoard.doSingleMove(active->getMyMove());
	}

	playerType winner = tmpBoard.doRandomPlayout();

	active = &root;
	do{
		active = active->getNext(movingPlayer);
		active->update(winner);
	}while(active->getFirstSon() != NULL);

}

MOVE_T UCT::getBestMoveFromTree(){

	UCTNode * bestMove = root.getFirstSon();
	for(int i=1; i<root.getSonsCount(); i++){
		if(bestMove->getRatio() < root.getFirstSon()[i].getRatio())
			bestMove = &(root.getFirstSon()[i]);
	}
	return bestMove->getMyMove();

}

void UCT::expand(UCTNode * node, Board& b){

	MY_ASSERT((node->getSonsCount())==0);
	MY_ASSERT((node->getFirstSon())==NULL);

	int n = b.getMovesCount();

	node->setSons(mm.getMemory(n), n);

	int i=0;
	for(b.resetMovesIterator(); !(b.iteratorAtEnd()); b.increaseIterator(), i++){
		node->setMove(b.getIteratorValue(),i);				
	}

}