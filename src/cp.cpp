#include "timer.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

void systemcall(char *filein, char *fileout, int size)
{
	int in=open(filein, O_RDONLY);
	int out=open(fileout, O_WRONLY | O_CREAT);
	char buffer[BUFSIZ];
	if(in == -1){
		perror("in buffer");
		exit(1);
	}
	if(out == -1){
		perror("out buffer");
		exit(1);
	}
	int good;
	while(good=read(in,buffer,size)){
		if(good==-1){
			perror("read error");
			exit(1);
		}

		if(write(out,buffer,size)==-1){
			perror("write error");
			exit(1);
		}
	}	
	close(in);
	close(out);

void checkexist(char *name)
{
	ifstream file(name);
	if(file.good()){
		cout << "file exists" << endl;
		exit(1);
	}
}

void starttime(Timer &d, const string hi)
{
	double w,u,s;
	d.elapsedTime(w,u,s);
	cout << endl << hi << endl;
	cout << "Usertime: " << u << endl;
	cout << "Systemtime: " << s << endl;
	cout << "Wallclock: " << w << endl;
	d.start();
}

int main(int argc, char* argv[]){
	bool time=false;
	bool first=true;
	char *in;
	char *out;
	if(argc < 3){
		cout << "cp <SOURCE> <DEST>" << endl;
		exit(1);
	}
	else if(argc > 4){
		cout << "Only 4 arguments" << endl;
		exit(1);
	}
	if(argc == 3)
	{

		checkexist(out);
		systemcall(in,out,BUFSIZ);
	}
	else
	{
		for(int i=1; i<argc; i++)
		{
			if(argv[i][0]=='-' && !time)
			{
				time=true;
			}
			else if(first)
			{
				first=false;
				in=argv[i];
			}
			else{
				out=argv[i];
			}
		}
			if(!time){
				cout << "No Flag!" << endl;
				exit(0);
			}
			checkexist(out);
			Timer t;
			t.start();
			systemcall(in,out,1);
			starttime(t,"read BUF=1");
			ifstream fin(in);
			ofstream fout(out);

			while(fin.good()){
				char c=fin.get();
				if(fin.good())
					fout.put(c);
			}
			fout.close();
			fin.close();
			starttime(t,"fstream");
			systemcall(in,out,BUFSIZ);
			starttime(t,"read BUF=BUFSIZ");
	}
	

	return 0;
}


