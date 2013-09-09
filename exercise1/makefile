# the compiler: gcc for C program, define as g++ for C++
CC = clang

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall

# the build target executable:
TARGET = fib-heap
TARGET2 = bin-heap
TARGET3 = bin-heap-improved

all: $(TARGET) $(TARGET2) $(TARGET3)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

$(TARGET2): $(TARGET2).c
	$(CC) $(CFLAGS) -o $(TARGET2) $(TARGET2).c

$(TARGET3): $(TARGET3).c
	$(CC) $(CFLAGS) -o $(TARGET3) $(TARGET3).c

clean:
	$(RM) $(TARGET)
	$(RM) $(TARGET2)
	$(RM) $(TARGET3)
	$(RM) -r $(TARGET).dSYM
	$(RM) -r $(TARGET2).dSYM
	$(RM) -r $(TARGET3).dSYM
