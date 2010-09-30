#include <cstdlib>
#include <iostream>

#include "board.h"
#include "move_pointers.h"
#include "logic.h"

using namespace std;

Board::Board()
{
	assert(sideSize%2 == 1);	/*musi byæ jedno pole na œrodku*/
	assert(2*(tzaars+tzarras+totts) == sideSize*sideSize - (sideSize/2)*((sideSize+1)/2) - 1);
								/*tzaary tzaarasy i tottsy musz¹ w sumie pokryæ ca³¹ planszê*/
}

void Board::setBeginningGameState()
{

	for(unsigned i=0; i<sideSize; i++)	/*czyszczê planszê*/
		for(unsigned j=0; j<sideSize; j++)
			boardSet(i,j,Fields::empty);

	for(int i=-(int)guardLayers; i<(int)(sideSize+guardLayers); i++)	/*ustawiam dooko³a œciany*/
		for(int j=-(int)guardLayers; j<0; j++){
			boardSet(i,j,Fields::wall);
			boardSet(j,i,Fields::wall);
			boardSet(i,sideSize+guardLayers+j,Fields::wall);
			boardSet(sideSize+guardLayers+j,i,Fields::wall);
		}

	for(unsigned i = 0, j = sideSize-1; i < sideSize/2; j--){ /*blokujê lewy dolny róg*/
		if(j <= sideSize/2 + i){
			i++;
			j=sideSize;
			continue;
		}
		boardSet(i,j,Fields::wall);
	}

	for(unsigned i = sideSize-1, j = 0; i > sideSize/2; j++){ /*prawy górny*/
		if(j >= i - sideSize/2){
			i--;
			j = -1;
			continue;
		}
		boardSet(i,j,Fields::wall);
	}

	boardSet(sideSize/2,sideSize/2,Fields::wall); /*blokujê œrodek*/

	unsigned	notSetWhiteTzaars = tzaars, notSetBlackTzaars = tzaars, \
				notSetWhiteTzarras = tzarras, notSetBlackTzarras = tzarras, \
				notSetWhiteTotts = totts, notSetBlackTotts = totts, \
				allNotSet = 2*(tzaars + tzarras + totts), allNotSetWhite = allNotSet/2;			

	for(unsigned i=0; i<sideSize; i++)	/*zape³niam planszê pionkami*/
		for(unsigned j=0; j<sideSize; j++){
			if(boardAt(i,j) == Fields::empty){
				unsigned r = rand() % allNotSet;
				allNotSet--;
				if(r < allNotSetWhite){
					if(r < notSetWhiteTotts){
						boardSet(i,j,Fields::getOccupiedValue(tott_t, white_t, 1));
						notSetWhiteTotts--;
						allNotSetWhite--;
					} else if(r < notSetWhiteTzaars + notSetWhiteTotts) {
						boardSet(i,j,Fields::getOccupiedValue(tzaar_t, white_t, 1));
						notSetWhiteTzaars--;
						allNotSetWhite--;
					} else {
						boardSet(i,j,Fields::getOccupiedValue(tzarras_t, white_t, 1));
						notSetWhiteTzarras--;
						allNotSetWhite--;
					}
				} else {
					if(r < notSetBlackTotts + allNotSetWhite){
						boardSet(i,j,Fields::getOccupiedValue(tott_t, black_t, 1));
						notSetBlackTotts--;
					} else if(r < notSetBlackTzaars + notSetBlackTotts + allNotSetWhite){
						boardSet(i,j,Fields::getOccupiedValue(tzaar_t, black_t, 1));
						notSetBlackTzaars--;
					} else {
						boardSet(i,j,Fields::getOccupiedValue(tzarras_t, black_t, 1));
						notSetBlackTzarras--;
					}
				}
			}
		}

			assert(notSetBlackTotts == 0 && notSetBlackTzaars == 0 && notSetBlackTzarras == 0 \
				&& notSetWhiteTotts == 0 && notSetWhiteTzaars == 0 && notSetWhiteTzarras == 0);


	whiteTzaars = blackTzaars = tzaars;
	whiteTzarras = blackTzarras = tzarras;
	whiteTotts = blackTotts = totts;

	allMovesCount[whiteCapturingMove_t] = allMovesCount[blackCapturingMove_t] = \
	allMovesCount[whiteReinforcingMove_t] = allMovesCount[blackReinforcingMove_t] = 0;

	for(unsigned i=0; i<factualSideSize*factualSideSize; i++)
		ITERATE_DIRS(dir){
			movePointers[i][dir]=MovePointers::initialMovePtr;
		}

	for(int i=0; i<factualSideSize*factualSideSize; i++){
		if(board[i] != Fields::wall){
			assert(Fields::isProperPawn(board[i]));
			ITERATE_DIRS(dir){
				tryToNotifyMove(i,i+dirOffsets[dir],(direction)dir);
			}
		}
	}

	phase = beginning_t;

	movesDone = 0;

}

playerType Board::doRandomPlayout(){

	try{
		switch(phase){
			case beginning_t:
				doRandomWhiteCapture();
				movesDone++;
				break;
			case blackCapture_t:
				break;
			case blackChoice_t:
				doRandomBlackMove();
				movesDone++;
			case whiteCapture_t:
				doRandomWhiteCapture();
				movesDone++;
			case whiteChoice_t:
				doRandomWhiteMove();
				movesDone++;
			default:
				assert(false);
				break;
		}

		while(true){
			doRandomBlackCapture();
			movesDone++;
			doRandomBlackMove();
			movesDone++;
			doRandomWhiteCapture();
			movesDone++;
			doRandomWhiteMove();
			movesDone++;
		}
	}catch(WinningException we){
		return we.winner;
	}

}

void Board::doRandomWhiteCapture(){

	MOVE_T move = getRandomWhiteCapture();
	makeMove(move);
	updateMoves(move);

}

void Board::doRandomWhiteMove(){

	MOVE_T move = getRandomWhiteMove();
	makeMove(move);
	updateMoves(move);

}

void Board::doRandomBlackCapture(){

	MOVE_T move = getRandomBlackCapture();
	makeMove(move);
	updateMoves(move);

}

void Board::doRandomBlackMove(){

	MOVE_T move = getRandomBlackMove();
	makeMove(move);
	updateMoves(move);

}

MOVE_T Board::getRandomWhiteMove(){

	checkIfBlackWon(allMovesCount[whiteCapturingMove_t]+allMovesCount[whiteReinforcingMove_t]);
	int r=rand()%(allMovesCount[whiteCapturingMove_t]+allMovesCount[whiteReinforcingMove_t]);
	if(r<allMovesCount[whiteCapturingMove_t])
		return allMoves[whiteCapturingMove_t][r];
	else
		return allMoves[whiteReinforcingMove_t][r-allMovesCount[whiteCapturingMove_t]];

}

MOVE_T Board::getRandomBlackMove(){
	
	int r;
	checkIfWhiteWon(allMovesCount[blackCapturingMove_t]+allMovesCount[blackReinforcingMove_t]);
	r=rand()%(allMovesCount[blackCapturingMove_t]+allMovesCount[blackReinforcingMove_t]);
	if(r<allMovesCount[blackCapturingMove_t])
		return allMoves[blackCapturingMove_t][r];
	else
		return allMoves[blackReinforcingMove_t][r-allMovesCount[blackCapturingMove_t]];

}

MOVE_T Board::getRandomWhiteCapture(){

	checkIfBlackWon(allMovesCount[whiteCapturingMove_t]);
	int r = rand()%allMovesCount[whiteCapturingMove_t];
	return allMoves[whiteCapturingMove_t][r];

}

MOVE_T Board::getRandomBlackCapture(){

	checkIfBlackWon(allMovesCount[blackCapturingMove_t]);
	int r = rand()%allMovesCount[blackCapturingMove_t];
	return allMoves[blackCapturingMove_t][r];

}

void Board::checkIfBlackWon(unsigned opponentMovesLeft){
	if(opponentMovesLeft == 0 || whiteTotts == 0 || whiteTzaars == 0 || whiteTzarras == 0){
		WinningException we;
		we.winner = black_t;
		throw we;
	}
}

void Board::checkIfWhiteWon(unsigned opponentMovesLeft){
	if(opponentMovesLeft == 0 || blackTotts == 0 || blackTzaars == 0 || blackTzarras == 0){
		WinningException we;
		we.winner = white_t;
		throw we;
	}
}

void Board::makeMove(MOVE_T move){

	int src = Moves::getSource(move), dest = Moves::getDestination(move);
	moveType mt = Moves::getMoveType(move);
	pawnType pwnt = Fields::getPawnType(board[dest]);
	playerType plrt = Fields::getPawnColor(board[dest]); 

	switch(pwnt){
		case tzaar_t:
			if(plrt == white_t)
				whiteTzaars--;
			else
				blackTzaars--;
			break;
		case tzarras_t:
			if(plrt == white_t)
				whiteTzarras--;
			else
				blackTzarras--;
			break;
		case tott_t:
			if(plrt == white_t)
				whiteTotts--;
			else
				blackTotts--;
		default:
			break;
	}

	if(mt == whiteCapturingMove_t || mt == blackCapturingMove_t)
		board[dest] = board[src];
	else
		board[dest] = Fields::increase(board[dest],board[src]);

	board[src] = Fields::empty;

}

void Board::updateMoves(MOVE_T move){

	unsigned source = Moves::getSource(move);
	unsigned dest = Moves::getDestination(move);

	ITERATE_LINES(dir){
		unsigned side1 = MovePointers::getDestination(movePointers[source][dir]);
		unsigned side2 = MovePointers::getDestination(movePointers[source][OPPOSITE_DIR(dir)]);
		if(movePointers[source][dir] == MovePointers::initialMovePtr){
			if(movePointers[source][OPPOSITE_DIR(dir)] != MovePointers::initialMovePtr){
				deleteMove(side2,(direction)dir);
			}
		} else if(movePointers[source][OPPOSITE_DIR(dir)] == MovePointers::initialMovePtr){
			deleteMove(side1,(direction)OPPOSITE_DIR(dir));
		} else {
			deleteMove(side1,(direction)OPPOSITE_DIR(dir));
			deleteMove(side2,(direction)dir);
			tryToNotifyMove(side1,side2,(direction)OPPOSITE_DIR(dir));
			tryToNotifyMove(side2,side1,(direction)dir);
		}
	}

	ITERATE_DIRS(dir){
		if(movePointers[source][dir] != MovePointers::initialMovePtr)
			deleteMove(source,(direction)dir);
	}

	ITERATE_DIRS(dir){
		if(movePointers[dest][dir] != MovePointers::initialMovePtr){
			unsigned otherSide = MovePointers::getDestination(movePointers[dest][dir]);
			deleteMove(dest,(direction)dir);
			deleteMove(otherSide,(direction)OPPOSITE_DIR(dir));
			tryToNotifyMove(dest,otherSide,(direction)dir);
			tryToNotifyMove(otherSide,dest,(direction)OPPOSITE_DIR(dir));
		}
	}

	checkEverything();

}

void Board::tryToNotifyMove(unsigned src, unsigned dest, direction dir){

	if(Fields::getPawnColor(board[src]) == white_t){
		if(Fields::canCapture(board[src],board[dest])){
			notifyMove(src,dest,dir,whiteCapturingMove_t);
			return;
		} else if(Fields::canReinforce(board[src],board[dest])){
			notifyMove(src,dest,dir,whiteReinforcingMove_t);
			return;
		}
	} else if(Fields::getPawnColor(board[src]) == black_t){
		if(Fields::canCapture(board[src],board[dest])){
			notifyMove(src,dest,dir,blackCapturingMove_t);
			return;
		} else if(Fields::canReinforce(board[src],board[dest])){
			notifyMove(src,dest,dir,blackReinforcingMove_t);
			return;
		}
	}

	if(Fields::isProperPawn(board[src]) && Fields::isProperPawn(board[dest])){
		notifyInactiveMove(src,dest,dir);
	}

}


FIELD_T Board::boardAt(int i, int j){
	return board[factualSideSize*(i+guardLayers)+j+guardLayers];
}

void Board::boardSet(int i, int j, FIELD_T value){
	board[factualSideSize*(i+guardLayers)+j+guardLayers] = value;
}

void Board::notifyMove(unsigned i1, unsigned j1, unsigned i2, unsigned j2, direction dir, moveType mt){
	assert(allMovesCount[mt] < maxOneTypeMoves);
	unsigned x=factualSideSize*(i1+guardLayers)+j1+guardLayers;
	unsigned y=factualSideSize*(i2+guardLayers)+j2+guardLayers;
	movePointers[x][dir] = MovePointers::getMovePtr(allMovesCount[mt],y,mt);
	allMoves[mt][allMovesCount[mt]++] = Moves::getMove(x,y,mt,dir);
}

void Board::notifyMove(unsigned src, unsigned dest, direction dir, moveType mt){
	assert(allMovesCount[mt] < maxOneTypeMoves);
	movePointers[src][dir] = MovePointers::getMovePtr(allMovesCount[mt],dest,mt);
	allMoves[mt][allMovesCount[mt]++] = Moves::getMove(src,dest,mt,dir);
}

void Board::notifyInactiveMove(unsigned src, unsigned dest, direction dir){
	movePointers[src][dir] = MovePointers::getInactiveMovePtr(dest);
}

void Board::deleteMove(unsigned src, direction dir){
	if(MovePointers::isActive(movePointers[src][dir]))
		deleteMove(src,dir,MovePointers::getMoveType(movePointers[src][dir]));
	else
		movePointers[src][dir] = MovePointers::initialMovePtr;
}

void Board::deleteMove(unsigned src, direction dir, moveType mt){
	unsigned index = MovePointers::getIndex(movePointers[src][dir]);
	assert(index < allMovesCount[mt]);
	assert(Moves::getSource(allMoves[mt][index]) == src);
	assert(movePointers[src][dir] != MovePointers::initialMovePtr);
	allMoves[mt][index] = allMoves[mt][--allMovesCount[mt]];
	movePointers[src][dir] = MovePointers::initialMovePtr;
	if(index < allMovesCount[mt]){
		unsigned switched = Moves::getSource(allMoves[mt][index]);
		direction switchedDir = Moves::getDirection(allMoves[mt][index]);
		movePointers[switched][switchedDir] = MovePointers::setIndex(movePointers[switched][switchedDir], index);
	}
}

bool Board::isDataConsistent(){

	for(int i=0; i<factualSideSize*factualSideSize; i++){
		ITERATE_DIRS(dir){
			if((movePointers[i][dir] != MovePointers::initialMovePtr) && MovePointers::isActive(movePointers[i][dir])){
				unsigned index = MovePointers::getIndex(movePointers[i][dir]);
				moveType mt = MovePointers::getMoveType(movePointers[i][dir]);
				if(		(Moves::getSource(allMoves[mt][index]) != i) || 
						(Moves::getDestination(allMoves[mt][index]) !=
						 MovePointers::getDestination(movePointers[i][dir]))){
					DataInconsistency di;
					di.dir = (direction) dir;
					di.field = i;
					throw di;
				}
			}
		}
	}

	for(int mt=0; mt<4; mt++)
		for(int i=0; i<allMovesCount[mt]; i++){
			if(		(board[Moves::getSource(allMoves[mt][i])] == Fields::empty) ||
					(board[Moves::getDestination(allMoves[mt][i])] == Fields::empty)){
				BrokenMove bm;
				bm.dest = Moves::getDestination(allMoves[mt][i]);
				bm.src = Moves::getSource(allMoves[mt][i]);
				throw bm;
			}

		}

	return true;

}

void Board::checkEverything(){

	try{
		isDataConsistent();
	}catch(DataInconsistency di){
		cout<<"dane niespojne! Z pola "<<di.field<<" w kierunku "<<di.dir<<endl;
		debug();
	}catch(BrokenMove bm){
		cout<<"zepsuty ruch! Z pola "<<bm.src<<" do pola "<<bm.dest<<endl;
		debug();
	}

}