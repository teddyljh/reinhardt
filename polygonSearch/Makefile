all: pps4

pps4: pps4.o polygonSearch.o
	g++ -o pps4 pps4.o polygonSearch.o/Users/teddyliu/Dropbox/research/Makefile

pps4.o: pps4.cpp polygonSearch.h
	g++ -fast -c pps4.cpp

polygonSearch.o: polygonSearch.cpp polygonSearch.h
	g++ -fast -c polygonSearch.cpp
#	g++ -Ddebug -c polygonSearch.cpp

clean:
	rm -f pps4 pps4.o polygonSearch.o
