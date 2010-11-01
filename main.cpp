#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

#include "ai.h"

using namespace std;

int main(){


	srand(time(NULL));

	Board b;
	AI ai;

	b.setBeginningGameState();
	
	ai.init(b);
	cout<<ai.generateMove(white_t);

	string s;
	cin>>s;

}