#include <iostream>
#include <string>

#include "my_assert.h"
#include "board.h"
#include "fields.h"
#include "logic.h"

using namespace std;

//#define FULL_DEBUG

void Board::debug()
{
	/*for(int y=0; y<(int)factualSideSize; y++){
		for(int x=0; x<(int)factualSideSize; x++){
			cout<<Fields::debug(board[y*factualSideSize+x])<<" ";
		}
		cout<<"\n";
	}*/
	cout<<"\nwhite captures: ";
	for(int i=0; i<allMovesCount[whiteCapturingMove_t]; i++)
		cout<<"("<<Moves::getSource(allMoves[whiteCapturingMove_t][i])/factualSideSize - guardLayers<<","
			<<Moves::getSource(allMoves[whiteCapturingMove_t][i])%factualSideSize - guardLayers<<") -> "
			<<"("<<Moves::getDestination(allMoves[whiteCapturingMove_t][i])/factualSideSize - guardLayers<<","
			<<Moves::getDestination(allMoves[whiteCapturingMove_t][i])%factualSideSize - guardLayers<<") ";
	cout<<"\nblack captures: ";
	for(int i=0; i<allMovesCount[blackCapturingMove_t]; i++)
		cout<<"("<<Moves::getSource(allMoves[blackCapturingMove_t][i])/factualSideSize - guardLayers<<","
			<<Moves::getSource(allMoves[blackCapturingMove_t][i])%factualSideSize - guardLayers<<") -> "
			<<"("<<Moves::getDestination(allMoves[blackCapturingMove_t][i])/factualSideSize - guardLayers<<","
			<<Moves::getDestination(allMoves[blackCapturingMove_t][i])%factualSideSize - guardLayers<<") ";
	cout<<"\nwhite reinforcements: ";
	for(int i=0; i<allMovesCount[whiteReinforcingMove_t]; i++)
		cout<<"("<<Moves::getSource(allMoves[whiteReinforcingMove_t][i])/factualSideSize - guardLayers<<","
			<<Moves::getSource(allMoves[whiteReinforcingMove_t][i])%factualSideSize - guardLayers<<") -> "
			<<"("<<Moves::getDestination(allMoves[whiteReinforcingMove_t][i])/factualSideSize - guardLayers<<","
			<<Moves::getDestination(allMoves[whiteReinforcingMove_t][i])%factualSideSize - guardLayers<<") ";
	cout<<"\nblack reinforcements: ";
	for(int i=0; i<allMovesCount[blackReinforcingMove_t]; i++)
		cout<<"("<<Moves::getSource(allMoves[blackReinforcingMove_t][i])/factualSideSize - guardLayers<<","
			<<Moves::getSource(allMoves[blackReinforcingMove_t][i])%factualSideSize - guardLayers<<") -> "
			<<"("<<Moves::getDestination(allMoves[blackReinforcingMove_t][i])/factualSideSize - guardLayers<<","
			<<Moves::getDestination(allMoves[blackReinforcingMove_t][i])%factualSideSize - guardLayers<<") ";
	cout<<"\n";
	cout<<allMovesCount[whiteCapturingMove_t]<<" "<<allMovesCount[blackCapturingMove_t]<<" "<<allMovesCount[whiteReinforcingMove_t]<<" "<<allMovesCount[blackReinforcingMove_t]<<endl;

#ifdef FULL_DEBUG

		cout<<"\nwhite captures: ";
	for(int i=0; i<allMovesCount[whiteCapturingMove_t]; i++)
		cout<<"i="<<i<<","<<Moves::getSource(allMoves[whiteCapturingMove_t][i])<<"->"
			<<Moves::getDestination(allMoves[whiteCapturingMove_t][i])<<" | ";
	cout<<"\nblack captures: ";
	for(int i=0; i<allMovesCount[blackCapturingMove_t]; i++)
		cout<<"i="<<i<<","<<Moves::getSource(allMoves[blackCapturingMove_t][i])<<"->"
			<<Moves::getDestination(allMoves[blackCapturingMove_t][i])<<" | ";
	cout<<"\nwhite reinforcements: ";
	for(int i=0; i<allMovesCount[whiteReinforcingMove_t]; i++)
		cout<<"i="<<i<<","<<Moves::getSource(allMoves[whiteReinforcingMove_t][i])<<"->"
			<<Moves::getDestination(allMoves[whiteReinforcingMove_t][i])<<" | ";
	cout<<"\nblack reinforcements: ";
	for(int i=0; i<allMovesCount[blackReinforcingMove_t]; i++)
		cout<<"i="<<i<<","<<Moves::getSource(allMoves[blackReinforcingMove_t][i])<<"->"
			<<Moves::getDestination(allMoves[blackReinforcingMove_t][i])<<" | ";
	cout<<"\n";

	for(unsigned y=0; y<factualSideSize; y++){
		for(unsigned x=0; x<factualSideSize; x++){
			for(unsigned i=0; i<DIR_NUMBER; i++){
				MovePointers::debug(movePointers[y*factualSideSize+x][i]);
				cout<<"  ";
			}
			cout<<"\n"; 
		}
		cout<<"\n\n";
	}

#endif

}
/*
string Fields::debug(FIELD_T field){
	MY_ASSERT(((field&WHITE) ^ (field&BLACK)) || field == WALL || field == EMPTY);
	MY_ASSERT(((field&TZAAR) ^ (field&TZARRAS) ^ (field&TOTT)) || field == WALL || field == EMPTY);
	MY_ASSERT(((field&(0xff)) > 0) || field == WALL || field == EMPTY);
	switch(field){
		case EMPTY: return string("     ");
		case WALL: return string("#####");
		default:
			if(field & WHITE){
				if(field & TZAAR) return (string("<<") + (char)((field & HEIGHT_MASK) + '0') + string(">>"));
				if(field & TZARRAS) return (string(" <") + (char)((field & HEIGHT_MASK) + '0') + string("> "));
				if(field & TOTT) return (string(" -") + (char)((field & HEIGHT_MASK) + '0') + string("- "));
			} else {
				if(field & TZAAR) return (string("[[") + (char)((field & HEIGHT_MASK) + '0') + string("]]"));
				if(field & TZARRAS) return (string(" [") + (char)((field & HEIGHT_MASK) + '0') + string("] "));
				if(field & TOTT) return (string(" ~") + (char)((field & HEIGHT_MASK) + '0') + string("~ "));
			}
	}
	MY_ASSERT(false);
	return "~~~~~";
}
*/

void MovePointers::debug(MOVEPTR_T mv){
	if(MovePointers::initialMovePtr == mv){
		cout<<" -0- ";
		return;
	}
	if(MovePointers::isActive(mv))
		cout<<"i: "<<getIndex(mv)<<" dst: "<<getDestination(mv)<<" mt: "<<getMoveType(mv);
	else
		cout<<"INACTIVE dst: "<<getDestination(mv);
}