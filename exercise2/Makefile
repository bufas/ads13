SOURCES=Driver.cpp ITree.cpp Tree.cpp Leaf.cpp
OBJECTS=$(SOURCES:.cpp=.o)
TEST=test
CXX=clang++
CXXFLAGS=-g -Wall -std=c++11 -stdlib=libc++

$(TEST): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

clean:
	rm -f *.o
	rm -f $(TEST)
