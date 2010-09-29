#include <iostream>
#include <string>
#include <ctime>

#include "board.h"

using namespace std;

void main(){


	srand(1);

	Board b;
	b.setBeginningGameState();
	b.debug();

	if(b.doRandomPlayout() == white_t)
		cout<<"losowy playout wygral gracz bialy";
	else
		cout<<"losowy playout wygral gracz czarny";

	string s;
	cin>>s;

	b.debug();

	cin>>s;

}