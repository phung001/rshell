CFlags = -Wall -Werror -ansi -pedantic

all: rshell

rshell: bin/rshell 

bin: 
	mkdir bin

bin/rshell: ./src/terminal.cpp | bin
	g++ $(CFlags) -o ./bin/rshell ./src/terminal.cpp 

clean: 
	rm -rf bin 
