all : collecteur 

collecteur : list.o data.o main.o init.o worker.o
	gcc -o collecteur list.o data.o init.o worker.o main.o -g  -Wall -lpthread

data.o : data.c data.h
	gcc -o data.o -c data.c

worker.o : worker.c worker.h
	gcc -o worker.o -c worker.c

init.o : init.c init.h
	gcc -o init.o  -c init.c

list.o : list.c list.h
	gcc -o list.o -c list.c

main.o : main.c defs.h
	gcc -o main.o -c main.c

clean :
	rm -rf data.o list.o main.o init.o worker.o collecteur 

