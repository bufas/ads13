CC=clang++
CFLAGS=-O3 -std=c++11 -stdlib=libc++

all: benchmark

benchmark:
	$(CC) $(CFLAGS) -o benchmark benchmark.cpp RBTree.cpp RBTreeNode.cpp

clean:
	rm -f benchmark
