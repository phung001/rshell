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

void ls_go(char *argv, bool &a, bool &l, bool &R){
	
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
		vector<char*> list;
		vector<string> dirlist;

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

			string arg2;
			if(R){												//for -R
				arg2.append(argv);
//cout << arg2 << endl;
				arg2.append("/");
//cout << arg2 << endl;
				arg2.append(filespecs->d_name);
//cout << arg2 << endl;
				char *argg = new char[arg2.length()+1];
				strcpy(argg, arg2.c_str());
		
//	cout << '1' << arg2 << '2' << argg << endl;
				if(-1 == stat(argg, &s)) {
//cout << argg << " ";
					perror("Error with stat2");
					exit(1);
				}
//cout << filespecs->d_name << endl;
				if((S_IFDIR & s.st_mode) )  {//is a directory
//			cout << "0000" <<  argg << endl;
					if ((a) && (strcmp(filespecs->d_name, ".") != 0) && (strcmp(filespecs->d_name, "..") != 0)) {
//			cout << "2222" << filespecs->d_name << endl;
						dirlist.push_back(arg2);
					}
					else if((!a) && (filespecs->d_name[0] != '.')){
//			cout << "3333" << filespecs->d_name << endl;
						dirlist.push_back(arg2);
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

		if(R){
			for(unsigned int i = 0; i < dirlist.size(); i++){		
//		cout << "1111" << dirlist[i]<< dirlist.size() << endl;
		//		string name(dirlist[i]);
		//		name.insert(0, "/");
		//		name.insert(0, argname);
		//		strcpy(dirlist.at(i), name.c_str());
	
				cout << endl << dirlist[i] << ':' << endl;
//				cout << name << ':' << endl;
				char* name = new char[dirlist.at(i).size() +1];
				strcpy(name, dirlist.at(i).c_str());
				ls_go(name, a, l, R);
			}
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
	char here[2]= {"."};
	
	if(argc == 1){								//regular ls
	//	cout << "here" << endl;
	//	string name(here);
		ls_go(here, a, l, R);		
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
	//		string name(here);
			ls_go(here, a, l, R);
		}
		else{
			for(unsigned int i = 0; i < list.size(); i++){
	//			string name(list[i]);
				ls_go(list[i], a, l, R);
			}
		}
	}


	return 0;
}
		
		
	
