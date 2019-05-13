CC = gcc
CFLAGS= -g

program: ma sv cv ag compact global 

ma:global
	$(CC) $(CFLAGS) ma.c -o ma global.o

sv:global
	$(CC) $(CFLAGS) sv.c -o sv global.o

cv:global
	$(CC) $(CFLAGS) cv.c -o cv global.o

compact:global
	$(CC) $(CFLAGS) compact.c -o comapact global.o

ag:global
	$(CC) $(CFLAGS) ag.c -o ag global.o


global:
	$(CC) $(CFLAGS) -c global.c
