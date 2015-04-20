CFlags = -Wall -Werror -ansi -pedantic

all: rshell

rshell: bin/rshell 

bin: 
	mkdir bin

bin/rshell: ./src/terminal.cpp | bin
	g++ ./src/terminal.cpp $(CFlags) -std=c++11 -o ./bin/rshell 	

clean: 
	rm -rf bin 
