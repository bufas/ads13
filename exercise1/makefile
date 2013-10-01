CC=clang
CFLAGS=-g -Wall

DIJ-BIN=dijkstra-bin
DIJ-FIB=dijkstra-fib

TEST-BIN=bin-test
TEST-FIB=fib-test

all: $(DIJ-FIB) $(DIJ-BIN) $(TEST-BIN) $(TEST-FIB)

$(DIJ-BIN): dijkstra.c bin-heap.c
	$(CC) $(CLFAGS) -o $@ $^

$(DIJ-FIB): dijkstra.c fib-heap.c
	$(CC) $(CLFAGS) -D FIB -o $@ $^

$(TEST-BIN): $(TEST-BIN).c bin-heap.c
	$(CC) $(CLFAGS) -o $@ $^

$(TEST-FIB): $(TEST-FIB).c fib-heap.c
	$(CC) $(CLFAGS) -o $@ $^

clean:
	$(RM) *.o
	$(RM) -r *.dSYM
	$(RM) *.dot
	$(RM) $(TEST-BIN) $(TEST-FIB) $(DIJ-BIN) $(DIJ-FIB)
