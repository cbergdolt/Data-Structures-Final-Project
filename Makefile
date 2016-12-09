all:		users musicMatch measure

musicMatch:	src/musicMatch.cpp
	g++ -g -gdwarf-2 -Wall -std=gnu++11 src/musicMatch.cpp -o musicMatch
users:		check-users src/playlistContents.py
	python src/playlistContents.py $(user1) $(user2)
measure:	src/measure.cpp
	g++ -g -gdwarf-2 -Wall -std=gnu++11 src/measure.cpp -o measure
test: users test-memory test-time

test-memory:	musicMatch
	@echo Testing memory...
	@[ `valgrind --leak-check=full ./musicMatch 2>&1 | grep ERROR | awk '{print $$4}'` = 0 ]
test-time:	musicMatch
	@echo Testing time...
	@./measure ./musicMatch 5 | tail -n 1 | awk '{ if ($$1 > 30.0) { print "Time limit exceeded"; exit 1} }'

clean:
	rm musicMatch data/related_artists.txt data/user1* data/user2*


check-users:
ifndef user1
	$(error usage: "make user1=user1 user2=user2")
endif
ifndef user2
	$(error usage: "make user1=user1 user2=user2")
endif

