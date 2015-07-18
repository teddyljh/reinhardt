CC=g++
#OPTIONS=-c -O2
OPTIONS=-c -g

all: srp2

srp2: srp2.o rein.o
	$(CC) -o srp2 srp2.o rein.o

srp2.o: srp2.cpp rein.h
	$(CC) $(OPTIONS) srp2.cpp

rein.o: rein.cpp rein.h
	$(CC) $(OPTIONS) rein.cpp

clean:
	rm -f srp2 srp2.o rein.o
