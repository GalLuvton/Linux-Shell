

all: ./bin/shell


./bin/shell: ./bin/shell.o ./bin/List.o ./bin/pipeHelper.o ./bin/cmdLog.o ./bin/LineParser.o
	gcc -m32 -g -Wall -o ./bin/shell ./bin/LineParser.o ./bin/cmdLog.o ./bin/pipeHelper.o ./bin/List.o ./bin/shell.o

 
./bin/LineParser.o: LineParser.c LineParser.h
	gcc -g -Wall -m32 -ansi -c -o ./bin/LineParser.o LineParser.c 

./bin/List.o: List.c
	gcc -g -Wall -m32 -ansi -c -o ./bin/List.o List.c

./bin/pipeHelper.o: pipeHelper.c pipeHelper.h
	gcc -g -Wall -m32 -ansi -c -o ./bin/pipeHelper.o pipeHelper.c

./bin/cmdLog.o: cmdLog.c
	gcc -g -Wall -m32 -ansi -c -o ./bin/cmdLog.o cmdLog.c

./bin/shell.o: shell.c
	gcc -g -Wall -m32 -ansi -c -o ./bin/shell.o shell.c

.PHONY: clean


clean: 
	rm -f ./bin/*.o ./bin/shell
