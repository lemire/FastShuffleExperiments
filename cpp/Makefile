all:
	 $(CXX) -march=native -std=c++11 -O3 -o shuffle shuffle.cpp -Wall -Wextra
	 $(CXX) -march=native -std=c++11 -ggdb -O3 -o stattests stattests.cpp -Wall -Wextra

test:
	./stattests
	./shuffle
