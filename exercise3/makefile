CC=clang++
CFLAGS=-std=c++11 -stdlib=libc++
DEBUG_FLAGS=-g -DDEBUG_ALL
RELEASE_FLAGS=-O3

all: release

release: rollback-rb-test

debug: rollback-rb-testd

rollback-rb-test:
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -o rollback-rb-test rollback-rb-test.cpp

rollback-rb-testd:
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -o rollback-rb-test rollback-rb-test.cpp

clean:
	rm -f rollback-rb-test
	rm -rf *.dSYM/
