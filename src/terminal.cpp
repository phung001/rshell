#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <string.h> 
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
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

void exec_go(char** path){
	int pid = fork();
	if(pid == 0){
		cout << "exec_go child ok" << endl;
		if(-1 == execvp(path[0], path)) perror("Error with exec_go execvp");
		exit(1);
	}
	else if(pid > 0){
		if(-1 == wait(0)){
			perror("Error with exec_go wait");
			exit(1);
		}
	}
	else {
		perror("Error with exec_go fork");
		exit(1);
	}
}

void redirect_out(int flag, char** outputLeft, char* inputRight){
	int orig = dup(1);
	if(-1 == close(1)) {
		perror("Error with redirect_out close1");
		exit(1);
	}
	if(flag == 5) {
		if (open(inputRight, O_WRONLY|O_CREAT|O_TRUNC) == -1) perror("Error with redirect_out open");
		else{
			//output to inputRight
			exec_go(outputLeft);		
		}
	}
	else if(flag == 6) {
		if(open(inputRight, O_WRONLY|O_CREAT|O_APPEND) == -1) perror("Error with redirect_out open");
		else{
			//output to inputRight
			exec_go(outputLeft);		
		}
	}
	if(-1 == close(1)) {
		perror("Error with redirect_out close2");
		exit(1);
	}
	if(-1 == dup2(orig, 1)){
		perror("Error with redirect_out dup2");
		exit(1);
	}
	if(-1 == close(orig)) {
		perror("Error with redirect_out close1");
		exit(1);
	}
	return;
}

void redirect_in(char** outputLeft, char* inputRight){
	int orig = dup(0);
	if(-1 == close(0)){
		perror("Error with redirect_in close");
		exit(1);
	}
	if(open(inputRight, O_RDONLY)  == -1) perror("Error with redirect_in open");
	else{
		exec_go(outputLeft);
	}
	close(0);
	dup2(orig, 0);
	close(orig);
	return;
}


int main(int argc, char* argv[]){
	while(1){
		outhostname();
		//get input
		string userinput, inputBlock, connector, inputRight;
		getline(cin, userinput);
		unsigned int pf = 0;
		bool first = true;
		int fd[2];
		int prevflag = 0;

		while(!userinput.empty()){
			unsigned int here;
			unsigned int flag = 0;		

cout << "userinput: " << userinput << endl << endl; 

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
						if(userinput.find_first_of("<>|", here+2) != string::npos){
							int here2 = userinput.find_first_of("<>|", here+1);
							inputRight = userinput.substr(here+1, here2-here-1);
							string rest = userinput;
							userinput = rest.substr(here2);
						}
						else{
							inputRight = userinput.substr(here+2);
							userinput = "";
						}
					}
					else{											//if just ">"
						flag = 5;
						inputBlock = userinput.substr(0, here);
						connector = userinput.substr(here, 1);
						if(userinput.find_first_of("<>|", here+1) != string::npos){
							int here2 = userinput.find_first_of("<>|", here+1);
							inputRight = userinput.substr(here+1, here2-here-1);
							string rest = userinput;
							userinput = rest.substr(here2);
						}
						else{
						inputRight = userinput.substr(here+1);
						userinput = "";
						}
					}
				}
				else if(userinput.at(here) == '<'){					//if just "<"
					flag = 7;
					inputBlock = userinput.substr(0, here);
					connector = userinput.substr(here, 1);
					if(userinput.find_first_of("<>|", here+1) != string::npos){
						int here2 = userinput.find_first_of("<>|", here+1);
						inputRight = userinput.substr(here+1, here2-here-1);
						string rest = userinput;
						userinput = rest.substr(here2);
					}
					else{
						inputRight = userinput.substr(here+1);
						userinput = "";
					}
				}
				else {
					cout << "Error with finding io carrots" << endl;
					exit(0);
				}
			}
	
		//	cout << endl << here;
			cout << "inputBlock :" << inputBlock << "1" << endl;
			cout << "connector :" << connector << "2" << endl;
			cout << "rest of userinput :" << userinput << "3" << endl;
			cout << "inputRight :" << inputRight << '4' << endl << endl;

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
			
	cout << "flag begin: " << flag << "  prevflag: " << prevflag << endl << endl;

			if( ((pf == 0) && (prevflag == 1)) || ((pf == 1) && (prevflag == 2)) || (prevflag == 3) || (first && !((flag == 4) ||  (flag == 5) || (flag == 6) || (flag == 7)) ) ){	
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
			
			else if(flag == 5 || flag == 6){
				inputRight = trim(inputRight);
				char *uinputt = new char[inputRight.length() +1];	//turns string into c*
				strcpy(uinputt, inputRight.c_str());
				redirect_out(flag, argg, uinputt); 
				delete[] uinputt;
				inputRight = "";
			}

			else if(flag == 7){
				inputRight = trim(inputRight);
				char *uinputt = new char[inputRight.length() +1];	//turns string into c*
				strcpy(uinputt, inputRight.c_str());
				redirect_in(argg, uinputt); 
				delete[] uinputt;
				inputRight = "";
			}
cout << "flag end: " << flag << endl;
			first = false;
			prevflag = flag;
			delete[] uinput;	
		}
	}
	return 0;
}
