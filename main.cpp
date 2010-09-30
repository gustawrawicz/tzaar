#include <iostream>
#include <string>
#include <ctime>

#include "board.h"

using namespace std;

void main(){


	srand(time(NULL));

	Board b;

	int white = 0;
	int black = 0;

	int start = time(NULL);

	for(int i=0; i<1000; i++){
		b.setBeginningGameState();
		if(b.doRandomPlayout() == white_t)
			white++;
		else
			black++;
	}

	cout<<"bialy wygral "<<white<<" razy, a czarny "<<black<<" razy."<<endl;

	cout<<"trwalo to "<<time(NULL)-start<<" sekund.";

	string s;
	cin>>s;

}