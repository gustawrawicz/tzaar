#include<string>

using namespace std;

class MyException{
public:
	MyException(){};
	MyException(string s){message = s;}
	string getMessage(){return message+"\n";}
private:
	string message;
};