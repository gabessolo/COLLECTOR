all : collecteur weblogi pppx

collecteur : list.o data.o main_collector.o init.o worker.o
	gcc -o collecteur list.o data.o init.o worker.o main_collector.o -g  -Wall -lpthread

weblogi : weblogi.o 
	gcc -o weblogi weblogi.o  -g  -Wall -lpthread

pppx : pppx.o main_pppx.o list.o data.o init.o
	gcc -o pppx pppx.o main_pppx.o list.o data.o init.o  -g  -Wall -lpthread

data.o : data.c data.h
	gcc -o data.o -c data.c

worker.o : worker.c worker.h
	gcc -o worker.o -c worker.c

init.o : init.c init.h
	gcc -o init.o  -c init.c

list.o : list.c list.h
	gcc -o list.o -c list.c

main_pppx.o : main_pppx.c defs.h
	gcc -o main_pppx.o -c main_pppx.c

main_collector.o : main_collector.c defs.h
	gcc -o main_collector.o -c main_collector.c

weblogi.o : weblogi.c 
	gcc -o weblogi.o -c weblogi.c

pppx.o : pppx.c 
	gcc -o pppx.o -c pppx.c
clean :
	rm -rf data.o list.o main.o init.o worker.o collecteur weblogi pppx

