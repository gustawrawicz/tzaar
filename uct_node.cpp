#include "uct_node.h"
#include "fast_computations.h"

void UCTNode::reset(){
	played=Params::initPlayed;
	won=Params::initWin;
	ratio=Params::initWin/Params::initPlayed;
	firstSon=NULL;
	sonsCount=0;
}


MOVE_T UCTNode::getMyMove(){

	return myMove;

}

int UCTNode::getSonsCount(){

	return sonsCount;

}

UCTNode * UCTNode::getFirstSon(){

	return firstSon;

}

float UCTNode::getPlayed(){

	return played;

}

float UCTNode::getRatio(){

	return ratio;

}

UCTNode * UCTNode::getNext(playerType movingPlayer){

	UCTNode * max = firstSon;
	for(int i=0; i<sonsCount; i++){
		if(firstSon[i].hasGreaterFormula(max,movingPlayer)){
			max = &(firstSon[i]);
		}
	}

	return max;

}

void UCTNode::update(playerType winner){

	if(winner == Moves::getPlayerType(myMove)){
		won += 1.0;
	}
	played += 1.0;
	ratio = won/played;

}

void UCTNode::setSons(UCTNode * sons, int size){
	firstSon = sons;
	sonsCount = size;
}

void UCTNode::setMove(MOVE_T move, int place){
	MY_ASSERT(place<sonsCount);
	firstSon[place].myMove = move;
}

bool UCTNode::hasGreaterFormula(UCTNode * node, playerType movingPlayer){
	
	MY_ASSERT(Moves::getPlayerType(myMove) == Moves::getPlayerType(node->myMove));
	if(movingPlayer == Moves::getPlayerType(myMove)){
		return	ratio + (Params::alpha * FastComputations::invSqrt(played)) >
				node->ratio + (Params::alpha * FastComputations::invSqrt(node->played));
	} else {
		return	-ratio + (Params::alpha * FastComputations::invSqrt(played)) >
				-node->ratio + (Params::alpha * FastComputations::invSqrt(node->played));
	}

}