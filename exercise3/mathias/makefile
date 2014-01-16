CC=clang++
CFLAGS=-std=c++11 -stdlib=libc++
DEBUG_FLAGS=-g -Wall

all: test

test:
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -o test test.cpp


clean:
	rm -f test
	rm -rf *.dSYM/
