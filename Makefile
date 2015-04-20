CFlags = -Wall -Werror -ansi -pedantic

all: rshell

rshell: 
	g++ /src/terminal.cpp $(CFlags) -std=c++11 	

clean: 
	rm -f rshell /src/terminal.cpp
