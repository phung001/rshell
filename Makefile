CFlags = -Wall -Werror -ansi -pedantic

all: rshell ls

rshell: bin/rshell 

ls: bin/ls

bin: 
	mkdir bin

bin/rshell: ./src/terminal.cpp | bin
	g++ $(CFlags) -o ./bin/rshell ./src/terminal.cpp 

bin/ls: ./src/ls.cpp | bin
	g++ $(CFlags) -std=c++11 -o ./bin/ls ./src/ls.cpp 

clean: 
	rm -rf bin

