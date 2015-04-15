#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>

using namespace std;


int main(int argc, char* argv[]){
	
	while(1){
	char host[64];

	if(gethostname(host, 64) == -1) perror("gethostname");
	else{
		cout << getlogin() << '@' << host << "$ ";

	}

	string userinput;
	cin >> userinput;
	
	if(userinput == "exit"){ 
		exit(0); 
	}	
	


	
	}
	return 0;
}
