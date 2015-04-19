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
		string userinput, inputBlock, connector;
		getline(cin, userinput);
		
		while(!userinput.empty()){
			unsigned int here = 0;
			if(userinput.find(";") != string::npos)
				here = userinput.find(";");
		
			if(userinput.find("&&") != string::npos)
				if(userinput.find("&&") < here)
					here = userinput.find("&&");
	
			if(userinput.find("||") != string::npos)
				if(userinput.find("||") < here)
					here = userinput.find("||");

			if(here > 0){
			inputBlock = userinput.substr(0, here);
			connector = userinput.substr(here, 1);
			string rest = userinput;
			userinput = rest.substr(here+1);
		
			cout << here;
			cout << "inputBlock :" << inputBlock << "1" << endl;
			cout << "connector :" << connector << "2" << endl;
			cout << "userinput :" << userinput << "3" << endl;
			}

			else{
				inputBlock = userinput;
				userinput = "";
			}

		//tokenize here	
			vector<char*> tokenlist;				
			char *token;
	
			char *uinput = new char[inputBlock.length() +1];	//turns string into c*
			strcpy(uinput, inputBlock.c_str());

			token = strtok(uinput, " ");				//tokenize and add to vector of char*
			while(token != NULL){
				tokenlist.push_back(token);
				token = strtok(NULL, " ");	
			}
		
			tokenlist.push_back('\0');

			char **argg = &tokenlist[0]; 

//		for(unsigned int j = 0; j < tokenlist.size(); ++j)	
//			cout << "tokenlist" << j << tokenlist.at(j) << endl;	

//		for(unsigned int j = 0; j < tokenlist.size(); ++j){	//execute loop
			
			if(strcmp(tokenlist.at(0), "exit") == 0)
				exit(0); 	
				
			else {
				int pid = fork();	
				if(pid == 0){
					if(execvp(argg[0], argg) == -1)
						perror("execvp");
					exit(0);
				}

				else if(pid > 0){
					waitpid(pid, NULL, 0);
				}

				else perror("fork");
			//	else if(tokenlist.at(j) == "true");
			//else if(tokenlist.at(j) == "false") exit(0);

			}
		delete[] uinput;	
		}
	}
	return 0;
}
