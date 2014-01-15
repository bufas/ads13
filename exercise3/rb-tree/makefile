CC=clang++
CFLAGS=-std=c++11 -stdlib=libc++
DEBUG_FLAGS=-g -DDEBUG_ALL
RELEASE_FLAGS=-O3

all: release

release: test benchmark

debug: testd benchmarkd

test:
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -o test test.cpp

benchmark:
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -o benchmark benchmark.cpp

testd:
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -o test test.cpp

benchmarkd:
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -o benchmark benchmark.cpp

clean:
	rm -f test
	rm -f benchmark
	rm -rf *.dSYM/
