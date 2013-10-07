CC=clang
CFLAGS=-g -Wall

DIJ-BIN=dijkstra-bin
DIJ-FIB=dijkstra-fib

TEST-BIN=bin-test
TEST-FIB=fib-test

BENCH-BIN=bench-bin
BENCH-FIB=bench-fib

all: $(DIJ-FIB) $(DIJ-BIN) $(TEST-BIN) $(TEST-FIB) $(BENCH-FIB) $(BENCH-BIN) 

$(DIJ-BIN): dijkstra.c bin-heap.c
	$(CC) $(CLFAGS) -o $@ $^

$(DIJ-FIB): dijkstra.c fib-heap.c
	$(CC) $(CLFAGS) -D FIB -o $@ $^

$(TEST-BIN): $(TEST-BIN).c bin-heap.c
	$(CC) $(CLFAGS) -o $@ $^

$(TEST-FIB): $(TEST-FIB).c fib-heap.c
	$(CC) $(CLFAGS) -o $@ $^

$(BENCH-BIN): bench.c bin-heap.c
	$(CC) $(CLFAGS) -o $@ $^

$(BENCH-FIB): bench.c fib-heap.c
	$(CC) $(CLFAGS) -D FIB -o $@ $^

clean:
	$(RM) *.o
	$(RM) -r *.dSYM
	$(RM) *.dot
	$(RM) $(TEST-BIN) $(TEST-FIB) $(DIJ-BIN) $(DIJ-FIB) $(BENCH-FIB) $(BENCH-BIN) 
