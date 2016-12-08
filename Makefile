all:		users musicMatch measure

musicMatch:	src/musicMatch.cpp
	g++ -g -gdwarf-2 -Wall -std=gnu++11 src/musicMatch.cpp -o musicMatch
users:		check-users
	python src/playlistContents.py $(user1) $(user2)
measure:	src/measure.cpp
	g++ -g -gdwarf-2 -Wall -std=gnu++11 src/measure.cpp -o measure

clean:
	rm musicMatch data/*

check-users:
ifndef user1
	$(error usage: "make user1=user1 user2=user2")
endif
ifndef user2
	$(error usage: "make user1=user1 user2=user2")
endif




