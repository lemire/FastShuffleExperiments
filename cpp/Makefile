all:
	 $(CXX) -std=c++11 -O3 -o justone justone.cpp -Wall -Wextra
	 $(CXX) -std=c++11 -O3 -o shuffle shuffle.cpp -Wall -Wextra
	 $(CXX) -std=c++11 -O3 -o sillyreservoir sillyreservoir.cpp -Wall -Wextra
	 $(CXX) -std=c++11 -ggdb -O3 -o stattests stattests.cpp -Wall -Wextra

clean: 
	rm -f justone shuffle sillyreservoir stattests
test:
	./shuffle
