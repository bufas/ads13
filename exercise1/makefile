# the compiler: gcc for C program, define as g++ for C++
CC = clang

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall

# the build target executable:
TARGET1 = fib-heap
TARGET2 = bin-heap
TARGET3 = fib-test

all: $(TARGET3)

$(TARGET3): $(TARGET3).o $(TARGET1).o
	$(CC) $(CFLAGS) -o $(TARGET3) $(TARGET3).o $(TARGET1).o

clean:
	$(RM) *.o
	$(RM) *.dot
	$(RM) -r *.dSYM
	$(RM) $(TARGET1) $(TARGET2) $(TARGET3)
