#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

#include "ai.h"
#include "my_exception.h"

using namespace std;

int main(){

	srand(time(NULL));

	Board b;
	AI ai;

	while(true){

		string s;
		cin>>s;

		try{
			if(s=="set_game"){
				cin>>s;
				while(s != "*"){
					b.tryToSetPawnAt(s);
					cin>>s;
				}
				cout<<"=\n";
			}
			if(s=="make_move"){
				string s2;
				cin>>s>>s2;
				b.tryToMakeMove(s,s2);
				cout<<"=\n";	
			}
			if(s=="gen_move"){
				MOVE_T m = ai.generateMove();
				cout<<Board::getMoveString(m)<<"\n=\n";
			}
			if(s=="quit"){
				cout<<"=\n";
				return 0;
			}
		} catch(MyException me){
			cout<<me.getMessage();
		}

	}

}