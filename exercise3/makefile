CC=clang++
CFLAGS=-std=c++11 -stdlib=libc++
DEBUG_FLAGS=-g -DDEBUG_ALL
RELEASE_FLAGS=-O3

all: release

release: rollback-rb-test part-retro-test

debug: rollback-rb-testd part-retro-testd

rollback-rb-test:
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -o rollback-rb-test rollback-rb-test.cpp

rollback-rb-testd:
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -o rollback-rb-test rollback-rb-test.cpp

part-retro-test:
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -o part-retro-test part-retro-test.cpp

part-retro-testd:
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -o part-retro-test part-retro-test.cpp

clean:
	rm -f rollback-rb-test
	rm -f part-retro-test
	rm -rf *.dSYM/
