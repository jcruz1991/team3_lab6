CC = g++
CFLAGS  = -g -Wall

default : main

main : main.o
	$(CC) $(CFLAGS) -o main main.o

main.o : main.cpp LogList.h
	$(CC) $(CFLAGS) -c main.cpp

clean : 
	$(RM) main *.o *~