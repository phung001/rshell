#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <iostream>

using namespace std;


int main(int argc, char* argv[]){
	
	char host[32];

	if(gethostname(host, 32) == -1) perror("gethostname");
	else{
		cout << getlogin() << '@' << host << ':' << '$';

	}
	
	return 0;
}
