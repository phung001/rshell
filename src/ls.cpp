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

bool sort_alpha(char* a, char* b){
	return (tolower(static_cast<unsigned char>(a[0])) < tolower(static_cast<unsigned char>(b[0])));
}

void ls_go(char *argv, bool &a, bool &l, bool &R, string argname){
	
	struct stat s;
	if(-1 == stat(argv, &s)) {
		cout << "stat1" << argv  << endl;
		perror("Error with stat");
		exit(1);
	}
	if(S_IFDIR & s.st_mode) {							//is a directory
		
		DIR *dirp;
		if(NULL == (dirp = opendir(argv))){
			perror("Error with opendir");
			exit(1);
		}
	
		struct dirent *filespecs;
		vector<char*> list, dirlist;

		errno = 0;
		while(NULL != (filespecs = readdir(dirp))){
			if(!a){												//for regular ls
				if(filespecs->d_name[0] != '.'){
					list.push_back(filespecs->d_name);
				}
			}
			else												//for -a
				list.push_back(filespecs->d_name);
	
			if(errno != 0){
				perror("Error with readdir");
				exit(1);
			}

			if(R){												//for -R
				string arg2;
				arg2.append(argname);
				arg2.append("/");
				arg2.append(filespecs->d_name);
//
				char *argg = new char[arg2.length()+1];
				strcpy(argg, arg2.c_str());
				arg2 = filespecs->d_name;
//	cout << '1' << arg2 << '2' << argg << endl;
				if(-1 == stat(argg, &s)) {
//cout << filespecs->d_name << " ";
					perror("Error with stat");
					exit(1);
				}
//cout << filespecs->d_name << endl;
				if((S_IFDIR & s.st_mode) )  {//is a directory
					if ((a) && (strcmp(filespecs->d_name, ".") != 0) && (strcmp(filespecs->d_name, "..") != 0)) {
							dirlist.push_back(filespecs->d_name);
					}
					else if((!a) && (filespecs->d_name[0] != '.')){
							dirlist.push_back(filespecs->d_name);
					}
				}
				delete[] argg;
			}
		}
		sort(list.begin(), list.end(), sort_alpha);
		if(!list.empty()){
			for(unsigned int i = 0; i < list.size(); i++){
				cout <<list.at(i) << "  ";
			}
			cout << endl;
		}

		if(closedir(dirp) == -1){
			perror("Error with closedir");
			exit(1);
		}

		for(unsigned int i = 0; i < dirlist.size(); i++){	
//cout <<"R OKOK: " << i << '/' << dirlist.size() << " " << dirlist[i]<< endl;
			string name(dirlist[i]);
			name.insert(0, "/");
			name.insert(0, argname);
			strcpy(dirlist[i], name.c_str());
			
			cout << endl << dirlist[i] << ':' << endl; 
			ls_go(dirlist[i], a, l, R, name);
		}

	}

	else {
		cout << argv << endl;
	}

}

int main(int argc, char** argv){
	
	bool a = false;
	bool l = false;
	bool R = false;
	char here[2] = {"."};
	
	if(argc == 1){								//regular ls
	//	cout << "here" << endl;
		string name(here);
		ls_go(here, a, l, R, name);		
	}
	else{
		vector<char*> list;
		string flags;

		for(signed int i = 1; i < argc; i++){
			list.push_back(argv[i]);
		}
		
		for(unsigned int i = 0; i < list.size(); i++){
			if(list[i][0] == '-'){
				flags.append(list[i]);
				list.erase(list.begin()+i);
			}
	
		}
	//	cout << flags << endl;
		
		if(flags.find('a') != string::npos) a = true;
		if(flags.find('l') != string::npos) l = true;
		if(flags.find('R') != string::npos) R = true;

		if(list.empty()) { 
			string name(here);
			ls_go(here, a, l, R, name);
		}
		else{
			for(unsigned int i = 0; i < list.size(); i++){
				string name(list[i]);
				ls_go(list[i], a, l, R, name);
			}
		}
		
	
		



	}


	return 0;
}
		
		
	
