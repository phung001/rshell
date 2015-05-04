CFlags = -Wall -Werror -ansi -pedantic

all: rshell ls mv rm

rshell: bin/rshell 

ls: bin/ls

mv: bin/mv

rm: bin/rm 

bin: 
	mkdir bin

bin/rshell: ./src/terminal.cpp | bin
	g++ $(CFlags) -o ./bin/rshell ./src/terminal.cpp 

bin/ls: ./src/ls.cpp | bin
	g++ $(CFlags) -std=c++11 -o ./bin/ls ./src/ls.cpp 

bin/mv: ./src/mv.cpp | bin
	g++ $(CFLAGS) -o ./bin/mv ./src/mv.cpp

bin/rm: ./src/rm.cpp | bin
	g++ $(CFLAGS) -o ./bin/rm ./src/rm.cpp

clean: 
	rm -rf /bin

