#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <boost/tokenizer.hpp> 

using namespace std;
using namespace boost;

int main(int argc, char* argv[]){
	
	while(1){
		char host[64];

	//output login info
		if(gethostname(host, 64) == -1) perror("gethostname");
		else{
			cout << getlogin() << '@' << host << "$ ";
		}

	//get input
		string userinput;
		cin >> userinput;
	
	//tokenize here
		char_separator<char> delim(",.<>?/:[]{}()-\\_\"=+!@#$%^*`~\t\n ","|&;"); 	
		tokenizer<char_separator<char>> mytok(userinput, delim);

		for(auto i = mytok.begin(); i != mytok.end(); ++i){
	
			if(*i  == "exit") 
				exit(0); 	
	
			//else if(execvp(userinput, 0) == -1) perror(execvp);

			else perror("not a valid command");
		
		}
	}
	return 0;
}
