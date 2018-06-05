shuffle:
	$(MAKE) -C cpp
	mv cpp/shuffle shuffle


all: shuffle

results.txt: shuffle
	./shuffle > results.txt


benchmark: results.txt

plot: results.txt
	gnuplot gnuplot/shuffle.gnuplot

clean:
	rm -f results.txt shuffle shuffle32precompopenbsd.pdf shuffleratio.pdf shuffle64.pdf floatshuffle32.pdf floatshuffle64.pdf shuffle32.pdf               shuffle64precompopenbsd.pdf
