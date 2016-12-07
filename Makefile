all:		musicMatch

musicMatch:	musicMatch.cpp match.h
	g++ -g -gdwarf-2 -Wall -std=gnu++11 musicMatch.cpp -o musicMatch

clean:
	rm musicMatch




