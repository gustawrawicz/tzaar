#ifndef _BOARD_H_
#define _BOARD_H_

#include <cassert>

#include "logic.h"
#include "fields.h"
#include "moves.h"
#include "directions.h"
#include "sizes.h"
#include "move_pointers.h"

class Board
{

public:

	Board();

	void setBeginningGameState();
	void playRandom();
	void debug();

	playerType doRandomPlayout();

private:

	FIELD_T boardAt(int i, int j);
	void boardSet(int i, int j, FIELD_T value);
	void firstFreeMoveSet(int i, int j, short value);

	void doRandomWhiteMove();
	void doRandomWhiteCapture();
	void doRandomBlackMove();
	void doRandomBlackCapture();
	MOVE_T getRandomWhiteMove();
	MOVE_T getRandomWhiteCapture();
	MOVE_T getRandomBlackMove();
	MOVE_T getRandomBlackCapture();

	void updateMoves(MOVE_T move);
	void checkIfBlackWon(unsigned opponentMovesLeft);
	void checkIfWhiteWon(unsigned opponentMovesLeft);

	void makeMove(MOVE_T move);
	void tryToNotifyMove(unsigned src, unsigned dest, direction dir);

	void notifyMove(unsigned i1, unsigned j1, unsigned i2, unsigned j2, direction dir, moveType mt);
	void notifyMove(unsigned src, unsigned dest, direction dir, moveType mt);
	void Board::deleteMove(unsigned src, direction dir);
	void deleteMove(unsigned src, direction dir, moveType mt);

	bool isDataConsistent();

	phaseType phase;

	FIELD_T board[factualSideSize*factualSideSize];

	MOVEPTR_T movePointers[factualSideSize*factualSideSize][DIR_NUMBER];
	MOVE_T allMoves[MOVE_TYPES_NUMBER][maxOneTypeMoves];

	unsigned short allMovesCount[MOVE_TYPES_NUMBER];

	unsigned short whiteTzaars;
	unsigned short blackTzaars;
	unsigned short whiteTzarras;
	unsigned short blackTzarras;
	unsigned short whiteTotts;
	unsigned short blackTotts;

};

#endif