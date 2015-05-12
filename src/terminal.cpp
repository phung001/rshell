#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <string.h> 
#include <sys/wait.h>
#include <sys/types.h>
#include <vector>
#include <fcntl.h>

using namespace std;

void outhostname(){
		char host[32];
		if(gethostname(host, 32) == -1){
			perror("gethostname error");
			exit(1);
		}	
		else{ 
			char* login;
			if((login = getlogin()) == NULL){
				perror("getlogin error");
				exit(1);
			}
			cout << login << '@' << host << "$ ";
		}
}

string trim_space(string const& str){
	if(str.empty()) return str;

	size_t first = str.find_first_not_of(' ');
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, last-first+1);
}

string trim_tab(string const& str){
	if(str.empty()) return str;

	size_t first = str.find_first_not_of('\t');
	size_t last = str.find_last_not_of('\t');
	return str.substr(first, last-first+1);
}

string trim(string const& str){
	string temp = trim_space(str);
	temp = trim_tab(temp);
	return temp;
}

int main(int argc, char* argv[]){
	while(1){
		outhostname();
		//get input
		string userinput, inputBlock, connector;
		getline(cin, userinput);
		unsigned int pf = 0;
		bool first = true;
		int fd[2];
		int prevflag = 0;

		while(!userinput.empty()){
			unsigned int here;
			unsigned int flag = 0;		

			unsigned int first_char = userinput.find_first_not_of(" \t");
		
			if(first_char > userinput.size()) break; 
			userinput = trim(userinput);

			if(userinput.find_first_of("#") != string::npos){
				userinput = userinput.substr(0, userinput.find_first_of("#"));
			}

			userinput.append(" ");	
			if(userinput.find_first_of(";&|") != string::npos){
				here = userinput.find_first_of(";&|");
				if(userinput.at(here) == ';'){
					flag = 3;
					inputBlock = userinput.substr(0, here);
					connector = userinput.substr(here, 1);
					string rest = userinput;
					userinput = rest.substr(here+1);
				}	
				else if( ((userinput.at(here) == '&') && (userinput.at(here+1) == '&'))
						|| ( (userinput.at(here) == '|') && (userinput.at(here+1) == '|'))){
					if(userinput.at(here) == '&')	flag = 1;
					if(userinput.at(here) == '|')	flag = 2;
					inputBlock = userinput.substr(0, here);
					connector = userinput.substr(here, 2);
					string rest = userinput;
					userinput = rest.substr(here+2);
				}
				else if(userinput.at(here) == '|'){				//piping
					flag = 4;
					inputBlock = userinput.substr(0, here);
					connector = userinput.substr(here, 1);
					string rest = userinput;
					userinput = rest.substr(here+1);
				}
			}
			else if(userinput.find_first_of("<>") != string::npos){
				here = userinput.find_first_of("<>");
				if(userinput.at(here) == '>'){
					if(userinput.at(here+1) == '>'){				//if ">>"
						flag = 6;
						inputBlock = userinput.substr(0, here);
						connector = userinput.substr(here, 2);
						string rest = userinput;
						userinput = rest.substr(here+2);
					}
					else{											//if just ">"
						flag = 5;
						inputBlock = userinput.substr(0, here);
						connector = userinput.substr(here, 1);
						string rest = userinput;
						userinput = rest.substr(here+1);
					}
				}
				else if(userinput.at(here) == '<'){					//if just "<"
					flag = 7;
					inputBlock = userinput.substr(0, here);
					connector = userinput.substr(here, 1);
					string rest = userinput;
					userinput = rest.substr(here+1);
				}
				else {
					cout << "Error with finding io carrots" << endl;
					exit(0);
				}
			}
	
		//	cout << endl << here;
			cout << "inputBlock :" << inputBlock << "1" << endl;
			cout << "connector :" << connector << "2" << endl;
			cout << "rest of userinput :" << userinput << "3" << endl << endl;


			if(flag == 0){
				inputBlock = userinput;
				userinput = "";
			}

			if(inputBlock.empty())	{
				cout << "execvp: No such file or directory" << endl;
				break;
			}

		//tokenize here	
			vector<char*> tokenlist;				
			char *token;
	
			char *uinput = new char[inputBlock.length() +1];	//turns string into c*
			strcpy(uinput, inputBlock.c_str());

			token = strtok(uinput, " \t");				//tokenize and add to vector of char*
			while(token != NULL){
				tokenlist.push_back(token);
				token = strtok(NULL, " \t");
			}
		
			tokenlist.push_back('\0');

			char **argg = &tokenlist[0]; 

			if( ((pf == 0) && (prevflag == 1)) || ((pf == 1) && (prevflag == 2)) || (prevflag == 3) || (first) ){	
				pf = 0;
				if(strcmp(tokenlist.at(0), "exit") == 0)
					exit(0); 	
				if(strcmp(tokenlist.at(0), "false") == 0){
					break;
					pf = 1;
				}
				else {
					if(-1 == pipe(fd)){
						perror("Pipe fail");
						exit(1);
					}
					int pid = fork();	
					if(pid == 0){
						if(-1 == close(fd[0])){
							perror("Close fail");
							exit(1);
						}
						int check = execvp(argg[0], argg);
						if(check == -1){	
							pf = 1;
							if(-1 == write(fd[1], &pf, sizeof(pf))){
								perror("Write fail");
								exit(1);
							}
							if(-1 == close(fd[1])){
								perror("Close fail");
								exit(1);
							}
							perror("execvp");
						}
						else{
							pf = 0;
							if(-1 == write(fd[1], &pf, sizeof(pf))){
								perror("Write fail");
								exit(1);
							}
							if(-1 == close(fd[1])){
								perror("Close fail");
								exit(1);
							}
							perror("execvp");
						}
						exit(0);
					}
					else if(pid > 0){
						if(-1 == waitpid(pid, NULL, 0)){
							perror("Waitpid fail");
							exit(1);
						}
						if(-1 == close(fd[1])){
							perror("Close fail");
							exit(1);
						}
						if(-1 == read(fd[0], &pf, sizeof(pf))){
							perror("Read fail");
							exit(1);
						}
						if(-1 == close(fd[0])){
							perror("Close fail");
							exit(1);
						}
					}
					else {
						perror("fork");
						exit(1);
					}
				}
			}
			first = false;
			prevflag = flag;
			delete[] uinput;	
		}
	}
	return 0;
}
