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

void rm_file(const char* path){
	if(unlink(path) == -1){
		perror("Error with unlink()");
		exit(1);
	}


}

void rm_dir(const char* path){
	DIR *dir;
	struct dirent *dirfiles;
	if (NULL == (dir = opendir(path)))
	{
			perror("no dir");
			exit(1);
	}
	errno = 0;
	while (NULL != (dirfiles = readdir(dir)))
	{
		struct stat s;
		if(-1 == lstat(path, &s)) {
			perror("Error with stat");
			exit(1);
		}

		if(S_IFDIR & s.st_mode) { //is a directory
			/*DIR *dirp;
			if(NULL == (dirp = opendir(argv))){
				perror("Error with opendir");
				exit(1);
			}*/
			string str = path;
			str.append("/");
			str.append(dirfiles->d_name);
			if(-1 == lstat(str.c_str(), &s)) {
				perror("Error with stat");
				exit(1);
			}

			if(S_IFDIR & s.st_mode) { 
				if((0 != strcmp(dirfiles->d_name, ".")) && (0 != strcmp(dirfiles->d_name, "..")))
				{
					const char *temp = str.c_str();
					rm_dir(temp);
				}
			}
			else
			{
				const char *temp = str.c_str();
				rm_file(temp);
			}

		}
		else
		{
			rm_file(dirfiles->d_name);
		}

	}
	if (errno != 0)
		perror("readdir");
	if (-1 == (closedir(dir)))
		perror("close fail");
	if (-1 == rmdir(path))
		perror("rmdir");
	


}




int main(int argc, char **argv){
	
	bool rmdir;
	string argg = argv[1];
	if (argg.find("-r") == 0)
	{
			rmdir = true;
	}
	if (rmdir)
	{
		//check if file exits
		for(int i = 2; i < argc; i++){
		
			argg = argv[i];
			int fd = open(argg.c_str(), O_RDONLY);
			if (fd == -1){
				perror("file does not exist");
				exit(1);
			}
			if(-1 == close(fd)){
				perror("close failed");
				exit(1);
			}
			
		}
		for (int i = 2; i < argc; i++)
		{
			struct stat s;
			argg = argv[i];
			if(-1 == lstat(argg.c_str(), &s)) {
				perror("Error with stat");
				exit(1);
			}

			if(S_IFDIR & s.st_mode) { //is a directory
				/*DIR *dirp;
				if(NULL == (dirp = opendir(argv))){
					perror("Error with opendir");
					exit(1);
				}*/
				rm_dir(argv[i]);
			}
			else 
				rm_file(argv[i]);
		}
	}


	else
	{
		for(int i =1; i < argc; i++){
		
			string argg = argv[i];
			int fd = open(argg.c_str(), O_RDONLY);
			if (fd == -1){
				perror("file does not exist");
				exit(1);
			}
			if(-1 == close(fd)){
				perror("close failed");
				exit(1);
			}
			
			struct stat s;
			if(-1 == lstat(argg.c_str(), &s)) {
				perror("Error with stat");
				exit(1);
			}
			if(S_IFDIR & s.st_mode) { //is a directory
				/*DIR *dirp;
				if(NULL == (dirp = opendir(argv))){
					perror("Error with opendir");
					exit(1);
				}*/
				cerr << "file is dir cannot remove";
			}
		}

		for (int i = 1; i < argc; i++)
		{
			rm_file(argv[i]);
		}
	}

		return 0;
}
