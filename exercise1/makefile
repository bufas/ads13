CC=clang
CFLAGS=-g -Wall
ifdef OPT
	CFLAGS+=-O$(OPT)
endif

DIJ-BIN=dijkstra-bin
DIJ-FIB=dijkstra-fib

TEST-BIN=bin-test
TEST-FIB=fib-test

BENCH-BIN=bench-bin
BENCH-FIB=bench-fib

GRAPHGEN=graphgen

all: $(DIJ-FIB) $(DIJ-BIN) $(TEST-BIN) $(TEST-FIB) $(BENCH-FIB) $(BENCH-BIN) $(GRAPHGEN)

$(DIJ-BIN): dijkstra.c bin-heap.c
	$(CC) $(CFLAGS) -o $@ $^

$(DIJ-FIB): dijkstra.c fib-heap.c
	$(CC) $(CFLAGS) -D FIB -o $@ $^

$(TEST-BIN): $(TEST-BIN).c bin-heap.c
	$(CC) $(CFLAGS) -o $@ $^

$(TEST-FIB): $(TEST-FIB).c fib-heap.c
	$(CC) $(CFLAGS) -o $@ $^

$(BENCH-BIN): bench.c bin-heap.c
	$(CC) $(CFLAGS) -o $@ $^

$(BENCH-FIB): bench.c fib-heap.c
	$(CC) $(CFLAGS) -D FIB -o $@ $^

$(GRAPHGEN): graphgen.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) *.o
	$(RM) -r *.dSYM
	$(RM) *.dot
	$(RM) $(TEST-BIN) $(TEST-FIB) $(DIJ-BIN) $(DIJ-FIB) $(BENCH-FIB) $(BENCH-BIN) $(GRAPHGEN) 
