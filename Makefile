CFlags = -Wall -Werror -ansi -pedantic

all: rshell

rshell: 
	g++ $(CFlags) -std=c++11 /src/terminal.cpp 
	
clean: 
	rm -f rshell /src/terminal.cpp
