all : collecteur weblogi pppx

collecteur : list.o data.o main_collecteur.o init.o worker.o sleepMs.o  sock.o defs.o
	gcc -o collecteur list.o data.o init.o worker.o main_collecteur.o sleepMs.o sock.o defs.o  -g  -Wall -lpthread

weblogi : weblogi.o sock.o defs.o init.o
	gcc -o weblogi weblogi.o  sock.o defs.o init.o -g  -Wall -lpthread

pppx : pppx.o main_pppx.o list.o data.o init.o sock.o defs.o
	gcc -o pppx pppx.o main_pppx.o list.o data.o init.o sock.o  defs.o  -g  -Wall -lpthread

data.o : data.c data.h
	gcc -o data.o -c data.c

worker.o : worker.c worker.h
	gcc -o worker.o -c worker.c

init.o : init.c init.h
	gcc -o init.o  -c init.c

list.o : list.c list.h
	gcc -o list.o -c list.c

main_pppx.o : main_pppx.c 
	gcc -o main_pppx.o -c main_pppx.c

main_collecteur.o : main_collecteur.c 
	gcc -o main_collecteur.o -c main_collecteur.c

weblogi.o : weblogi.c 
	gcc -o weblogi.o -c weblogi.c

pppx.o : pppx.c  
	gcc -o pppx.o -c pppx.c

defs.o : defs.c  defs.h
	gcc -o defs.o -c defs.c

sock.o : sock.c  sock.h
	gcc -o sock.o -c sock.c

clean :
	rm -rf data.o list.o  init.o worker.o collecteur weblogi pppx

