#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <string.h> 
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <vector>
#include <algorithm>

using namespace std;

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

bool sort_alpha(char* a, char* b){
	return (tolower(static_cast<unsigned char>(a[0])) < tolower(static_cast<unsigned char>(b[0])));
}

void ls_go(char *argv, bool a, bool l, bool R){
	DIR *dirp;

	if(NULL == (dirp = opendir(argv))){
		perror("Error with opendir");
		exit(1);
	}
	
	struct dirent *filespecs;
	vector<char*> list;	
	errno = 0;
	while(NULL != (filespecs = readdir(dirp))){
		if(!a){
			if(filespecs->d_name[0] != '.'){
				list.push_back(filespecs->d_name);
			}
		}
		else
			list.push_back(filespecs->d_name);
	}
	sort(list.begin(), list.end(), sort_alpha);
	for(unsigned int i = 0; i < list.size(); i++){
		cout <<list.at(i) << "  ";
	}
	cout << endl;

	if(errno != 0){
		perror("Error with readdir");
		exit(1);
	}

	if(closedir(dirp) == -1){
		perror("Error with closedir");
		exit(1);
	}
}




int main(int argc, char** argv){

	char here[1] = {'.'};
	if(argc == 1){
		// do ls here
		ls_go(here, false, false ,false);		
	}
	else{
		if(strcmp(argv[1], "-a") == 0) ls_go(here, true, false, false);

		
		
		else{
			cout << "nothing" << endl;



		}
	}


	return 0;
}
		
		
	
