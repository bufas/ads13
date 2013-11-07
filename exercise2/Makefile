SOURCES=Driver.cpp Tree.cpp Leaf.cpp
OBJECTS=$(SOURCES:.cpp=.o)
TEST=test
CXXFLAGS=-g -Wall

$(TEST): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

clean:
	rm -f *.o
	rm -f $(TEST)
