#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <string>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <string>
#include <dirent.h>
#include <stdlib.h>

using namespace std;



int main(int argc, char **argv){
	
	bool isdir = false;
	bool noexit = false;

	string argg = argv[1];
	string argg2 = argv[2];
	int fd = open(argg.c_str(), O_RDONLY);
	if (fd == -1){
		perror("file does not exist");
		exit(1);
	}
	if(-1 == close(fd)){
		perror("close failed");
		exit(1);
	}
	fd = open(argg2.c_str(), O_RDONLY);
	if (fd != -1){
			struct stat s;
			if(-1 == lstat(argg2.c_str(), &s)) {
				perror("Error with stat");
				exit(1);
			}
		
			if(S_IFDIR & s.st_mode) { //is a directory
				isdir = true;
			}
			else{
				perror("file exits");
				exit(1);
			}
	}
	else
	{
		noexit = true;

	}

	if (noexit)
	{
		if (-1 == (link(argg.c_str(), argg2.c_str())))
			perror("link");
		if (-1 == (unlink(argg.c_str())))
			perror("unlink");
	}
	if (isdir)
	{
		string tmp = argg2 + "/" + argg;
		if (-1 == (link(argg.c_str(), tmp.c_str())))
			perror("link");
		if (-1 == (unlink(argg.c_str())))
			perror("unlink");
	}
	return 0;
}
