CFlags = -Wall -Werror -ansi -pedantic

all: rshell

rshell: 
	g++ /src/terminal.cpp $(CFlags) 
clean: 
	rm -f rshell /src/terminal.cpp
