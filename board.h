#ifndef _BOARD_H_
#define _BOARD_H_

#include <string>

#include "my_assert.h"
#include "logic.h"
#include "fields.h"
#include "moves.h"
#include "directions.h"
#include "sizes.h"
#include "move_pointers.h"

struct MoveIterator{
	int moveIndex;
	moveType mt;
	bool needToJumpForward;
};

class Board{

	phaseType phase;

	FIELD_T board[factualSideSize*factualSideSize];

	MOVEPTR_T movePointers[factualSideSize*factualSideSize][DIR_NUMBER];
	MOVE_T allMoves[MOVE_TYPES_NUMBER][maxOneTypeMoves];

	unsigned short allMovesCount[MOVE_TYPES_NUMBER];
	unsigned short allPawnsCount[PLAYER_TYPES_NUMBER][PAWN_TYPES_NUMBER];

	MoveIterator moveIterator;

public:

	Board();

	void setBeginningGameState();
	void load(Board& b);
	void playRandom();
	void debug();

	void doSingleMove(MOVE_T);
	playerType doRandomPlayout();

	playerType getMovingPlayer();
	phaseType getPhase();
	int getMovesCount();

	void resetMovesIterator();
	MOVE_T getIteratorValue();
	void increaseIterator();
	bool iteratorAtEnd();

	bool blackWon();
	bool whiteWon();

	int getField(char, int);
	direction getDir(int, int, int, int);
	direction getDir(char, int, char, int);
	moveType getMoveType(int, int);

	void tryToSetPawnAt(std::string);
	void tryToMakeMove(std::string,std::string);
	static string getMoveString(MOVE_T);

private:

	FIELD_T boardAt(int, int);
	void boardSet(int, int, FIELD_T);
	int coordsToIndex(int, int);

	void doRandomWhiteMove();
	void doRandomWhiteCapture();
	void doRandomBlackMove();
	void doRandomBlackCapture();
	MOVE_T getRandomWhiteMove();
	MOVE_T getRandomWhiteCapture();
	MOVE_T getRandomBlackMove();
	MOVE_T getRandomBlackCapture();

	void updateMoves(MOVE_T move);
	bool blackWon(unsigned opponentMovesLeft);
	bool whiteWon(unsigned opponentMovesLeft);

	void makeMove(MOVE_T move);
	void tryToNotifyMove(unsigned src, unsigned dest, direction dir);
	bool validMove(unsigned, unsigned, unsigned, unsigned);
	MOVE_T getMove(unsigned, unsigned, unsigned, unsigned);
	direction getDirection(unsigned, unsigned, unsigned, unsigned);
	moveType getMoveType(unsigned, unsigned, unsigned, unsigned);

	void notifyMove(unsigned i1, unsigned j1, unsigned i2, unsigned j2, direction dir, moveType mt);
	void notifyMove(unsigned src, unsigned dest, direction dir, moveType mt);
	void notifyInactiveMove(unsigned src, unsigned dest, direction dir);
	void deleteMove(unsigned src, direction dir);
	void deleteMove(unsigned src, direction dir, moveType mt);

	bool isDataConsistent();
	void checkEverything();

};

#endif