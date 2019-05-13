CC = gcc
CFLAGS= -g

program: ma sv cv ag compact global 

ma:
	$(CC) $(CFLAGS) ma.c  -o ma global.o

sv:
	$(CC) $(CFLAGS) sv.c -o sv global.o

cv:
	$(CC) $(CFLAGS) cv.c -o cv global.o

compact:
	$(CC) $(CFLAGS) compact.c -o comapact global.o

ag:
	$(CC) $(CFLAGS) ag.c -o ag global.o


global:
	$(CC) $(CFLAGS) -c global.c
