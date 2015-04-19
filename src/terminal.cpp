#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <string.h> 
#include <sys/wait.h>
#include <vector>

using namespace std;

void outhostname(){
		char host[32];
		int error;

	//output login info
		error = gethostname(host, 32);
		if(error == -1)
			perror("gethostname");
		else{ 
			cout << getlogin() << '@' << host << "$ ";
		}
}

int main(int argc, char* argv[]){
	while(1){
		outhostname();
		//get input
		string userinput;
		getline(cin, userinput);
		char *token;

		//tokenize here	
		vector<char*> tokenlist;				
	
//find ; && || and add space if needed
		
		char *uinput = new char[userinput.length() +1];	//turns string into c*
		strcpy(uinput, userinput.c_str());

		token = strtok(uinput, " ");				//tokenize and add to vector of char*
		while(token != NULL){
			tokenlist.push_back(token);
			token = strtok(NULL, " ");	
		}

		tokenlist.push_back('\0');

		char **argg = &tokenlist[0]; 

//		for(unsigned int j = 0; j < tokenlist.size(); ++j)	
//			cout << j << tokenlist.at(j) << endl;	

		for(unsigned int j = 0; j < tokenlist.size(); ++j){	//execute loop
			
			if(strcmp(tokenlist.at(j), "exit") == 0)
				exit(0); 	
				
			else if(execvp(argg[0], argg) == -1){
				perror("execvp");
			}
		//	else if(tokenlist.at(j) == "true");

			//else if(tokenlist.at(j) == "false") exit(0);

		//	else if(tokenlist.at(j) == "&" && tokenlist.at(j+1) == "&");
				
		//	else if(tokenlist.at(j) == "|" && tokenlist.at(j+1) == "|");

			else {
				perror("not a valid command");
			}
		}
		delete [] uinput;
		
	}
	return 0;
}
