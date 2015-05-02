#include <stdio.h>
#include <time.h>
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
#include <pwd.h>
#include <grp.h>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

bool sort_alpha(char* a, char* b){
	return (tolower(static_cast<unsigned char>(a[0])) < tolower(static_cast<unsigned char>(b[0])));
}

int longest_str(vector<string> list){
	if(list.empty())	return 0;
	unsigned int longest = 0;
	for(unsigned int i = 0; i < list.size(); i++){
		if(list[i].length() > longest){
			longest = list[i].length();
		}
	}
	return longest;

}
int longest_str(vector<char*> list){
	if(list.empty())	return 0;
	unsigned int longest = 0;
	for(unsigned int i = 0; i < list.size(); i++){
		if(strlen(list.at(i)) > longest){
			longest = strlen(list.at(i));
		}
	}
	return longest;

}

void ls_go(char *argv, bool &a, bool &l, bool &R){
	
	struct stat s;
	if(-1 == lstat(argv, &s)) {
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
		int btotal = 0;
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
				arg2.append("/");
				arg2.append(filespecs->d_name);
				char *argg = new char[arg2.length()+1];
				strcpy(argg, arg2.c_str());
		
				if(-1 == stat(argg, &s)) {
					perror("Error with stat2");
					exit(1);
				}
				if((S_IFDIR & s.st_mode) )  {//is a directory
					if ((a) && (strcmp(filespecs->d_name, ".") != 0) && (strcmp(filespecs->d_name, "..") != 0)) {
						dirlist.push_back(arg2);
					}
					else if((!a) && (filespecs->d_name[0] != '.')){
						dirlist.push_back(arg2);
					}
				}
				delete[] argg;
			}

		}

		sort(list.begin(), list.end(), sort_alpha);


		if(R)	cout << argv << ":" << endl;
		int currlen = 0;
		if(!list.empty() && !l){
			for(unsigned int i = 0; i < list.size(); i++){
				currlen += strlen(list.at(i)) + 1;
				if(currlen >= 64){
					cout << endl;	
					currlen = strlen(list.at(i)) +1;
				}
			//struct stat s;
				string color = argv;
				color.append("/");
				color.append(list.at(i));
				if(-1 == lstat(color.c_str(), &s)) {
					cout << "stat color" << color  << endl;
					perror("Error with stat");
					exit(1);
				}
				if(S_IFDIR & s.st_mode)								//is a directory
					cout << "\x1b[34m";	
				else if(s.st_mode & S_IXUSR)
					cout << "\x1b[32m";
				if(list.at(i)[0] == '.')
					cout << "\x1b[47m";
				cout << list.at(i) << "\x1b[0m" << "  " ;
			}
			cout  <<  endl;
		}
		else if(l){
			vector<char*> usr_len;
			vector<char*> grp_len;
			vector<string> size_len;
			vector<string> links_len;
			struct group *grp;
			struct passwd *pwd;
			grp = getgrgid(s.st_gid);
			if(NULL == grp){
				perror("Error with getgrpid");
				exit(1);
			}
			pwd = getpwuid(s.st_uid);
			if(NULL == pwd){
				perror("Error with getgrpid");
				exit(1);
			}

			for(unsigned int i = 0; i < list.size(); i++){
				string argl = "";
				argl.append(argv);	
				argl.append("/");
				argl.append(list[i]);
				char *argg = new char[argl.length()+1];
				strcpy(argg, argl.c_str());

				struct stat r;
					if(-1 == lstat(argg, &r)) {
						cout << "stat1" << argg  << endl;
						perror("Error with stat");
						exit(1);
					}
				btotal += r.st_blocks;
			
				usr_len.push_back(pwd->pw_name);
			
				grp_len.push_back(grp->gr_name);
				
				long sizee = r.st_size;
				string p = to_string(sizee);
				size_len.push_back(p);
			
				sizee = r.st_nlink;
				p = to_string(sizee);
				links_len.push_back(p);
				delete [] argg;
			}
			cout << "size = " << btotal/2 << endl;	
//			cout << longest_str(links_len) << endl;
//			cout << longest_str(usr_len) << endl;
//			cout << longest_str(grp_len) << endl;
//			cout << longest_str(size_len) << endl;
			for(unsigned int i = 0; i < list.size(); i++){
				string argl = "";
				argl.append(argv);	
				argl.append("/");
				argl.append(list[i]);
				char *argg = new char[argl.length()+1];
				strcpy(argg, argl.c_str());

				struct stat w;
					if(-1 == lstat(argg, &w)) {
						cout << "stat2" << argg  << endl;
						perror("Error with stat");
						exit(1);
					}
				struct tm *clk;
				clk = gmtime(&(w.st_mtime));
				if(clk == NULL){
					perror("Error with gmtime()");
					exit(1);
				}

				string last_time = asctime(clk); 
				last_time = last_time.erase(0, 4); 
				last_time = last_time.erase(12, 9); 

				if((S_IFDIR & w.st_mode)){ 
					cout << "d";
				}
				else if((S_IFREG & w.st_mode)){
					cout << "-";
				}
				else{ 
					cout << "l";
				}
				cout << ((S_IRUSR & w.st_mode)?"r":"-")
					<< ((S_IWUSR & w.st_mode)?"w":"-")
					<< ((S_IXUSR & w.st_mode)?"x":"-")
					<< ((S_IRGRP & w.st_mode)?"r":"-")
					<< ((S_IWGRP & w.st_mode)?"w":"-")
					<< ((S_IXGRP & w.st_mode)?"x":"-")
				    << ((S_IROTH & w.st_mode)?"r":"-")
					<< ((S_IWOTH & w.st_mode)?"w":"-")
					<< ((S_IXOTH & w.st_mode)?"x":"-") << ' '; 
				cout << w.st_nlink;	
				for(int i = to_string(w.st_nlink).length(); i <= longest_str(links_len); i++)
					cout << ' ';
				cout << pwd->pw_name;
				for(int i = strlen(pwd->pw_name); i <= longest_str(usr_len); i++)
					cout << ' ';
				cout << grp->gr_name;
				for(int i = strlen(grp->gr_name); i <= longest_str(grp_len); i++)
					cout << ' ';
				cout << w.st_size;	
				for(int i = to_string(w.st_size).length(); i <= longest_str(size_len); i++)
					cout << ' ';
				cout << last_time << ' '
					<< list[i] << endl;
	
				delete [] argg;
			}
		}
		if(R) cout << endl;

		if(closedir(dirp) == -1){
			perror("Error with closedir");
			exit(1);
		}

		if(R){
			for(unsigned int i = 0; i < dirlist.size(); i++){		
//				cout << name << ':' << endl;
				char* name = new char[dirlist.at(i).size() +1];
				strcpy(name, dirlist.at(i).c_str());
				ls_go(name, a, l, R);
				delete [] name;
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
		
		if(flags.find('a') != string::npos) a = true;
		if(flags.find('l') != string::npos) l = true;
		if(flags.find('R') != string::npos) R = true;

		if(list.empty()) { 
			ls_go(here, a, l, R);
		}
		else{
			for(unsigned int i = 0; i < list.size(); i++){
				ls_go(list[i], a, l, R);
			}
		}
	}


	return 0;
}
		
		
	
