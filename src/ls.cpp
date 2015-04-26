
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <string.h> 
#include <sys/wait.h>
#include <sys/types.h>
#include <vector>

using namespace std;

tring trim_space(string const& str){
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

void outhostname(){
	char host[32];
	int error;

	error = gethostname(host, 32);
	if(error == -1)	perror("gethostname");
	else cout << getlogin() << '@' << host << "$ ";
}

int main(){
	while(){
		outhostname();
		string userinput;
		getline(cin, userinput);

		while(!userinput.empty()){
	
			if(first_char > userinput.size()) break; 
				userinput = trim(userinput);
				

		}
	

	}	

	return 0;
}
		
		
		
		
