#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

#include "board.h"

using namespace std;

int main(){


	srand(time(NULL));

	Board b;
	b.setBeginningGameState();

	int white = 0;
	int black = 0;

	int start = time(NULL);

	for(int i=0; i<1000; i++){
		Board b2;
		b2.load(b);
		if(b2.doRandomPlayout() == white_t)
			white++;
		else
			black++;
	}

	cout<<"bialy wygral "<<white<<" razy, a czarny "<<black<<" razy."<<endl;

	cout<<"trwalo to "<<time(NULL)-start<<" sekund.";

	string s;
	cin>>s;

}