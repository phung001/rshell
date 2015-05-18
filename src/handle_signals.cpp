#include <iostream>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

using namespace std;

bool flag = false;
bool flag1 = false;
bool flag2 = false;
int count = 0;
int count1 = 0;
int count2 = 0;

void handle(int x){
	flag = true;
	count++;
}
void handle1(int x){
	flag1 = true;
	count1++;
}
void handle2(int x){
	flag2 = true;
	count2++;
}

int main(){

	struct sigaction oldact, newact;

	newact.sa_handler = handle;
	sigemptyset (&newact.sa_mask);
	newact.sa_flags = SA_SIGINFO;

	if(-1 == sigaction (SIGINT, &newact, NULL)){
		perror("Error with SIGINT");
		exit(1);
	}
	newact.sa_handler = handle1;
	if(-1 == sigaction (SIGQUIT, &newact, NULL)){
		perror("Error with SIGQUIT");
		exit(1);
	}
	newact.sa_handler = handle2;
	if(-1 == sigaction (SIGTSTP, &newact, NULL)){
		perror("Error with SIGTSTP");
		exit(1);
	}

	while(1){
		if(flag2 && (count2 == 3)){
			cout << endl << "^C : " << count << "\t^\\ : " << count1 << "\t^Z : " << count2 << endl;
			exit(0);
		}
		else if(flag){
			cout << "\b\b" << 'd'<< flush;
			flag = false;
		}
		else if(flag1){
			cout << "\b\b" << '\\' <<flush;
			flag1 = false;
		}
		else if(flag2){
			cout << "\b\b" << 's' << flush;
			flag2 = false;
			raise(SIGSTOP);
		}
		else cout << 'x' << flush;
		sleep(1);
	}

	return 0;
}

