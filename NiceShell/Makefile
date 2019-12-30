all: nsh
#todos os .o/gcc -o executavel *.o
nsh: main.o burst.o program.o signal.o tad.o wait.o
	gcc -o nsh main.o burst.o program.o signal.o tad.o wait.o -g -Wall
#x.o: x.c *.h gcc -o x.o -c x.c 
main.o: main.c burst.h program.h signal.h tad.h wait.h
	gcc -o main.o -c main.c -g -Wall
burst.o: burst.c burst.h tad.h
	gcc -o burst.o -c burst.c -g -Wall
program.o: program.c program.h burst.h signal.h tad.h
	gcc -o program.o -c program.c -g -Wall
signal.o: signal.c signal.h
	gcc -o signal.o -c signal.c -g -Wall
wait.o: wait.c wait.h signal.h
	gcc -o wait.o -c wait.c -g -Wall
tad.o: tad.c tad.h
	gcc -o tad.o -c tad.c -g -Wall
#limpa os .o
clear:
	rm -rf *.o
