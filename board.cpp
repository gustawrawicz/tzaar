#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cmath>

#include "board.h"
#include "move_pointers.h"
#include "logic.h"
#include "my_exception.h"
#include "technical_tools.h"

using namespace std;

Board::Board()
{
	MY_ASSERT(sideSize%2 == 1);	/*there has to be one field exactly in the middle of the board*/
	MY_ASSERT(2*(tzaars+tzarras+totts) == sideSize*sideSize - (sideSize/2)*((sideSize+1)/2) - 1);
								/*tzaars, tzarrases and totts have to cover all board*/
}

void Board::resetMovesIterator(){
	moveIterator.moveIndex = 0;
	moveIterator.mt = MOVE_TYPE_FROM_PHASE(phase);
	moveIterator.needToJumpForward = CHOICE_PHASE(phase);
	MY_ASSERT(!(phase == whiteChoice_t) || (moveIterator.mt == whiteCapturingMove_t));
	MY_ASSERT(!(phase == blackChoice_t) || (moveIterator.mt == blackCapturingMove_t));
}

MOVE_T Board::getIteratorValue(){
	return allMoves[moveIterator.mt][moveIterator.moveIndex];
}

void Board::increaseIterator(){
	moveIterator.moveIndex++;
	if(iteratorAtEnd() && moveIterator.needToJumpForward){
		MY_ASSERT((moveIterator.mt == blackCapturingMove_t) || (moveIterator.mt == whiteCapturingMove_t));
		moveIterator.moveIndex = 0;
		moveIterator.mt = NEXT_MOVE(moveIterator.mt);
		moveIterator.needToJumpForward = false;
	}
}

bool Board::iteratorAtEnd(){
	return moveIterator.moveIndex >= allMovesCount[moveIterator.mt];
}

playerType Board::getMovingPlayer(){
	if(phase == blackCapture_t || phase == blackChoice_t)
		return black_t;
	else
		return white_t;
}

phaseType Board::getPhase(){
	return phase;
}

int Board::getMovesCount(){
	if(CHOICE_PHASE(phase)){
		if(ACTIVE_PLAYER(phase) == white_t)
			return allMovesCount[whiteCapturingMove_t]+allMovesCount[whiteReinforcingMove_t];
		else
			return allMovesCount[blackCapturingMove_t]+allMovesCount[blackReinforcingMove_t];
	} else {
		if(ACTIVE_PLAYER(phase) == white_t)
			return allMovesCount[whiteCapturingMove_t];
		else
			return allMovesCount[blackCapturingMove_t];
	}
}

bool Board::blackWon(){
	switch(phase){
		case beginning_t:
			return blackWon(allMovesCount[whiteCapturingMove_t]);
		case whiteCapture_t:
			return blackWon(allMovesCount[whiteCapturingMove_t]);
		case whiteChoice_t:
			return blackWon(allMovesCount[whiteCapturingMove_t]+allMovesCount[whiteReinforcingMove_t]);
		default:
			return false;
	}
}

bool Board::whiteWon(){
	switch(phase){
		case blackCapture_t:
			return whiteWon(allMovesCount[blackCapturingMove_t]);
		case blackChoice_t:
			return whiteWon(allMovesCount[blackCapturingMove_t]+allMovesCount[blackReinforcingMove_t]);
		default:
			return false;
	}
}

void Board::setBeginningGameState(){

	for(unsigned i=0; i<sideSize; i++)
		for(unsigned j=0; j<sideSize; j++)
			boardSet(i,j,Fields::empty);

	for(int i=-(int)guardLayers; i<(int)(sideSize+guardLayers); i++)	/*guards at the ends of the board*/
		for(int j=-(int)guardLayers; j<0; j++){
			boardSet(i,j,Fields::wall);
			boardSet(j,i,Fields::wall);
			boardSet(i,sideSize+guardLayers+j,Fields::wall);
			boardSet(sideSize+guardLayers+j,i,Fields::wall);
		}

		for(unsigned i=0; i<sideSize/2; i++){				/*filling left-upper corner*/
			for(unsigned j=sideSize/2-i+1; j>0; j--){
				boardSet(i,j, Fields::wall);
			}
		}
		
		for(unsigned i=sideSize; i>sideSize/2; i--){			/*filling right-lower corner*/
			for(unsigned j=sideSize; j>3*sideSize/2-i; j--){
				boardSet(i,j, Fields::wall);
			}
		}

	boardSet(sideSize/2,sideSize/2,Fields::wall); /*middle of the board*/

	unsigned	notSetWhiteTzaars = tzaars, notSetBlackTzaars = tzaars, \
				notSetWhiteTzarras = tzarras, notSetBlackTzarras = tzarras, \
				notSetWhiteTotts = totts, notSetBlackTotts = totts, \
				allNotSet = 2*(tzaars + tzarras + totts), allNotSetWhite = allNotSet/2;			

	for(unsigned i=0; i<sideSize; i++)	/*setting pawns: each at random place of the board*/
		for(unsigned j=0; j<sideSize; j++){
			if(boardAt(i,j) == Fields::empty){
				unsigned r = rand() % allNotSet;
				allNotSet--;
				if(r < allNotSetWhite){
					allNotSetWhite--;
					if(r < notSetWhiteTotts){
						boardSet(i,j,Fields::getOccupiedValue(tott_t, white_t, 1));
						notSetWhiteTotts--;
					} else if(r < notSetWhiteTzaars + notSetWhiteTotts) {
						boardSet(i,j,Fields::getOccupiedValue(tzaar_t, white_t, 1));
						notSetWhiteTzaars--;
					} else {
						boardSet(i,j,Fields::getOccupiedValue(tzarras_t, white_t, 1));
						notSetWhiteTzarras--;
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

			MY_ASSERT(notSetBlackTotts == 0 && notSetBlackTzaars == 0 && notSetBlackTzarras == 0
				&& notSetWhiteTotts == 0 && notSetWhiteTzaars == 0 && notSetWhiteTzarras == 0);


	allPawnsCount[white_t][tzaar_t] = allPawnsCount[black_t][tzaar_t] = tzaars;
	allPawnsCount[white_t][tzarras_t] = allPawnsCount[black_t][tzarras_t] = tzarras;
	allPawnsCount[white_t][tott_t] = allPawnsCount[black_t][tott_t] = totts;

	allMovesCount[whiteCapturingMove_t] = allMovesCount[blackCapturingMove_t]
		= allMovesCount[whiteReinforcingMove_t] = allMovesCount[blackReinforcingMove_t] = 0;

	for(unsigned i=0; i<factualSideSize*factualSideSize; i++)
		ITERATE_DIRS(dir){
			movePointers[i][dir]=MovePointers::initialMovePtr;
		}

	for(int i=0; i<factualSideSize*factualSideSize; i++){
		if(board[i] != Fields::wall){
			MY_ASSERT(Fields::isProperPawn(board[i]));
			ITERATE_DIRS(dir){
				tryToNotifyMove(i,i+dirOffsets[dir],(direction)dir);
			}
		}
	}

	phase = beginning_t;

}

void Board::load(Board& b){
	memcpy(this, &b, sizeof(*this));
}

void Board::doSingleMove(MOVE_T move){
	MY_ASSERT(movePointers[Moves::getSource(move)][Moves::getDirection(move)] != MovePointers::initialMovePtr);
	MY_ASSERT(move != Moves::initialMove);
	makeMove(move);
	updateMoves(move);
	phase = NEXT_PHASE(phase);
}

playerType Board::doRandomPlayout(){

/*for optimization omits attribute 'phase' updating*/

	switch(phase){
		case beginning_t:
			if(blackWon(allMovesCount[whiteCapturingMove_t]))
				return black_t;
			doRandomWhiteCapture();
			break;
		case blackCapture_t:
			break;
		case blackChoice_t:
			if(whiteWon(allMovesCount[blackCapturingMove_t]+allMovesCount[blackReinforcingMove_t]))
				return white_t;
			doRandomBlackMove();
		case whiteCapture_t:
			if(blackWon(allMovesCount[whiteCapturingMove_t]))
				return black_t;
			doRandomWhiteCapture();
		case whiteChoice_t:
			if(blackWon(allMovesCount[whiteCapturingMove_t]+allMovesCount[whiteReinforcingMove_t]))
				return black_t;
			doRandomWhiteMove();
		default:
			break;
	}

	while(true){
		if(whiteWon(allMovesCount[blackCapturingMove_t]))
			return white_t;
		doRandomBlackCapture();
		if(whiteWon(allMovesCount[blackCapturingMove_t]+allMovesCount[blackReinforcingMove_t]))
			return white_t;
		doRandomBlackMove();
		if(blackWon(allMovesCount[whiteCapturingMove_t]))
			return black_t;
		doRandomWhiteCapture();
		if(blackWon(allMovesCount[whiteCapturingMove_t]+allMovesCount[whiteReinforcingMove_t]))
			return black_t;
		doRandomWhiteMove();
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

	int r=rand()%(allMovesCount[whiteCapturingMove_t]+allMovesCount[whiteReinforcingMove_t]);
	if(r<allMovesCount[whiteCapturingMove_t])
		return allMoves[whiteCapturingMove_t][r];
	else
		return allMoves[whiteReinforcingMove_t][r-allMovesCount[whiteCapturingMove_t]];

}

MOVE_T Board::getRandomBlackMove(){
	
	int r=rand()%(allMovesCount[blackCapturingMove_t]+allMovesCount[blackReinforcingMove_t]);
	if(r<allMovesCount[blackCapturingMove_t])
		return allMoves[blackCapturingMove_t][r];
	else
		return allMoves[blackReinforcingMove_t][r-allMovesCount[blackCapturingMove_t]];

}

MOVE_T Board::getRandomWhiteCapture(){

	int r = rand()%allMovesCount[whiteCapturingMove_t];
	return allMoves[whiteCapturingMove_t][r];

}

MOVE_T Board::getRandomBlackCapture(){

	int r = rand()%allMovesCount[blackCapturingMove_t];
	return allMoves[blackCapturingMove_t][r];

}

bool Board::blackWon(unsigned opponentMovesLeft){
	if(opponentMovesLeft == 0 || allPawnsCount[white_t][tott_t] == 0 ||
		allPawnsCount[white_t][tzarras_t] == 0 || allPawnsCount[white_t][tzaar_t] == 0){
		return true;
	}
	return false;
}

bool Board::whiteWon(unsigned opponentMovesLeft){
	if(opponentMovesLeft == 0 || allPawnsCount[black_t][tott_t] == 0 ||
		allPawnsCount[black_t][tzarras_t] == 0 || allPawnsCount[black_t][tzaar_t] == 0){
		return true;
	}
	return false;
}

void Board::makeMove(MOVE_T move){

	int src = Moves::getSource(move), dest = Moves::getDestination(move);
	moveType mt = Moves::getMoveType(move);
	pawnType pwnt = Fields::getPawnType(board[dest]);
	playerType plrt = Fields::getPawnColor(board[dest]); 

	allPawnsCount[plrt][pwnt]--;

	if(mt == whiteCapturingMove_t || mt == blackCapturingMove_t){
		board[dest] = board[src];
		board[src] = Fields::empty;
	}else{
		board[dest] = Fields::increase(board[dest],board[src]);
		board[src] = Fields::empty;
	}

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

	if(Fields::isProperPawn(board[src]) && Fields::isProperPawn(board[dest]))
		notifyInactiveMove(src,dest,dir);

}

bool Board::validMove(unsigned x1, unsigned y1, unsigned x2, unsigned y2){

	FIELD_T f1, f2;
	f1 = boardAt(x1,y1);
	f2 = boardAt(x2,y2);

	switch(phase){
		case beginning_t:
			if(Fields::getPawnColor(f1) != white_t)
				return false;
			if(Fields::getPawnColor(f2) != black_t)
				return false;
			break;
		case blackCapture_t:
			if(Fields::getPawnColor(f1) != black_t)
				return false;
			if(Fields::getPawnColor(f2) != white_t)
				return false;
			break;
		case blackChoice_t:
			if(Fields::getPawnColor(f1) != white_t)
				return false;
			break;
		case whiteCapture_t:
			if(Fields::getPawnColor(f1) != white_t)
				return false;
			if(Fields::getPawnColor(f2) != black_t)
				return false;
			break;
		case whiteChoice_t:
			if(Fields::getPawnColor(f1) != white_t)
				return false;
			break;
		}
	if(Fields::getPawnHeight(f1)<Fields::getPawnHeight(f2))
		return false;
	if(x1==x2 && y1==y2)
		return false;
	if(x1==x2){
		unsigned i = min(y1,y2)+1;
		while(i<max(y1,y2)){
			if(Fields::getPawnType(boardAt(x1,i)) != emptyField_t)
				return false;
			i++;
		}
		return true;
	}
	if(y1 == y2){
		unsigned i = min(x1,x2)+1;
		while(i<max(x1,x2)){
			if(Fields::getPawnType(boardAt(i,y1)) != emptyField_t)
				return false;
			i++;
		}
		return true;
	}
	if(x1+y1 == x2+y2){
		unsigned i = 1;
		int x = min(x1, x2);
		int y = max(y1, y2);
		while(i < ((x1-x2>0)?(x1-x2):(x2-x1))){
			if(Fields::getPawnType(boardAt(x+i,y-i)) != emptyField_t)
				return false;
			i++;
		}
		return true;
	}
	return false;

}

moveType Board::getMoveType(unsigned x1, unsigned y1, unsigned x2, unsigned y2){
	moveType mt;
	if(Fields::getPawnColor(boardAt(x1,y1)) == white_t){
		if(Fields::canCapture(boardAt(x1,y1),boardAt(x2,y2))){
			mt = whiteCapturingMove_t;
		} else if(Fields::canReinforce(boardAt(x1,y1),boardAt(x2,y2))){
			mt = whiteReinforcingMove_t;
		}
	} else if(Fields::getPawnColor(boardAt(x1,y1)) == black_t){
		if(Fields::canCapture(boardAt(x1,y1),boardAt(x2,y2))){
			mt = blackCapturingMove_t;
		} else if(Fields::canReinforce(boardAt(x1,y1),boardAt(x2,y2))){
			mt = blackReinforcingMove_t;
		}
	}
	return mt;
}

direction Board::getDirection(unsigned x1, unsigned y1, unsigned x2, unsigned y2){

	if(x1 == x2){
		if(y1<y2)
			return down_t;
		else if(y1>y2)
			return up_t;
	} else if(y1 == y2){
		if(x1<x2)
			return right_t;
		else if(x1>x2)
			return left_t;
	} else if(x1+y1 == x2+y2)
		if(x2>x1)
			return rightup_t;
		else if(x2<x1)
			return leftdown_t;

	MY_ASSERT(false);

	return down_t;	

}

MOVE_T Board::getMove(unsigned x1, unsigned y1, unsigned x2, unsigned y2){
	moveType mt;
	mt = getMoveType(x1,y1,x2,y2);
	direction d = getDirection(x1,y1,x2,y2);
	return Moves::getMove(coordsToIndex(x1,y1),coordsToIndex(x2,y2),mt,d);
}

FIELD_T Board::boardAt(int i, int j){
	return board[factualSideSize*(i+guardLayers)+j+guardLayers];
}

void Board::boardSet(int i, int j, FIELD_T value){
	board[factualSideSize*(i+guardLayers)+j+guardLayers] = value;
}

int Board::coordsToIndex(int i, int j){
	return factualSideSize*(i+guardLayers)+j+guardLayers;
}

void Board::notifyMove(unsigned i1, unsigned j1, unsigned i2, unsigned j2, direction dir, moveType mt){
	MY_ASSERT(allMovesCount[mt] < maxOneTypeMoves);
	unsigned x=factualSideSize*(i1+guardLayers)+j1+guardLayers;
	unsigned y=factualSideSize*(i2+guardLayers)+j2+guardLayers;
	movePointers[x][dir] = MovePointers::getMovePtr(allMovesCount[mt],y,mt);
	allMoves[mt][allMovesCount[mt]++] = Moves::getMove(x,y,mt,dir);
}

void Board::notifyMove(unsigned src, unsigned dest, direction dir, moveType mt){
	MY_ASSERT(allMovesCount[mt] < maxOneTypeMoves);
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
	MY_ASSERT(index < allMovesCount[mt]);
	MY_ASSERT(Moves::getSource(allMoves[mt][index]) == src);
	MY_ASSERT(movePointers[src][dir] != MovePointers::initialMovePtr);
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

int Board::getField(char a, int b){
	return boardAt(a - 'a' + 1, b);
}

direction Board::getDir(char a1, int a2, char b1, int b2){
	return getDir(a1-'a'+1,a2, b1-'a'+1,b2);
}

direction Board::getDir(int a1, int a2, int b1, int b2){

	if(a1 == a2)
		if(b1 > b2)
			return down_t;
		else
			return up_t;
	
	if(b1 == b2)
		if(a1 > a2)
			return left_t;
		else
			return right_t;

	if(a1+a2 == b1+b2)
		if(a2 > a1)
			return rightup_t;
		else
			return leftdown_t;

	MY_ASSERT(false);
	return up_t;

}

moveType Board::getMoveType(int a, int b){

	if(Fields::getPawnColor(board[a]) == white_t)
		if(Fields::getPawnColor(board[b]) == white_t)
			return whiteReinforcingMove_t;
		else
			return whiteCapturingMove_t;
	else
		if(Fields::getPawnColor(board[b]) == white_t)
			return blackCapturingMove_t;
		else
			return blackReinforcingMove_t;

}

string Board::getMoveString(MOVE_T m){

	int src = Moves::getSource(m);
	int dest = Moves::getDestination(m);

	int x1,x2,y1,y2;

	x1 = src%factualSideSize - guardLayers + 1;
	y1 = src/factualSideSize - guardLayers + 1;
	x2 = dest%factualSideSize - guardLayers + 1;
	y2 = dest/factualSideSize - guardLayers + 1;

	x1 += 'A' - 1;
	x2 += 'A' - 1;

	char x1Str[2] = {x1,'\0'};
	char x2Str[2] = {x2,'\0'};

	return string(x1Str) + unsignedToString(y1) + " " + string(x2Str) + unsignedToString(y2);

}

void Board::tryToSetPawnAt(string s){

	int i=0;
	int x=0, y;
	int h=0;
	playerType playert;
	pawnType pawnt;

	if(s.length() == 0) throw MyException("too short string");

	x=s[0]-'A'+1;

	if(x<=0) throw MyException("bad coordinate");

	s = s.substr(1);

	y = getNumeralPrefix(s);

	if(y<=0) throw MyException("bad coordinate");

	s = cutNumeralPrefix(s);

	if(s.length() < 3)
		throw MyException("too short string");

	if(s[1]=='w')
		playert = white_t;
	else if(s[1]=='b')
		playert = black_t;
	else throw MyException("bad pawn color");

	if(s[2] != 't') throw MyException("bad pawn type");

	if(s[3]=='r')
		pawnt = tzaar_t;
	else if(s[3]=='s')
		pawnt = tzarras_t;
	else if(s[3]=='t')
		pawnt = tott_t;
	else throw MyException("bad pawn type");

	s=s.substr(3);

	h = getNumeralPrefix(s);

	if(h<=0) throw MyException("bad pawn height");

	boardSet(x-1,y-1,Fields::getOccupiedValue(pawnt,playert,h));
	
}

void Board::tryToMakeMove(string s1, string s2){

	int x1 = getNumeralPrefix(s1);
	int x2 = getNumeralPrefix(s2);

	s1 = cutNumeralPrefix(s1);
	s2 = cutNumeralPrefix(s2);

	if(s1.length() == 0 || s2.length() == 0)
		throw MyException("too short string");

	int y1 = s1[0]-'A'+1;
	int y2 = s2[0]-'A'+1;

	if(!validMove(x1,y1,x2,y2))
		throw MyException("invalid move");

	MOVE_T m = getMove(x1,y1,x2,y2);

	makeMove(m);

}