#include "technical_tools.h"

int getNumeralPrefix(string s){

	int i=0, x=0;

	for(;i<s.length()&&s[i]<='9'&&s[i]>='0'; i++){
		x+=s[i]-'0';
		if(i+1<s.length()&&s[i+1]<='9'&&s[i+1]>='0')
			x*=10;
	}

	return x;

}

string cutNumeralPrefix(string s){

	int i=0, x=0;

	for(;i<s.length()&&s[i]<='9'&&s[i]>='0'; i++){
		x+=s[i]-'0';
		if(i+1<s.length()&&s[i+1]<='9'&&s[i+1]>='0')
			x*=10;
	}

	return s.substr(i);

}

string unsignedToString(unsigned n){
	string s = "";
	if(n == 0)
		s = "0";
	else
		while(n>0){
			char digit[2];
			digit[1] = '\0';
			digit[0] = n%10+'0';
			s = s+string(digit);
			n/=10;
		}
	return s;
}