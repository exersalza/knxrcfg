CXX = g++
CXXFLAGS = -std=c++2a

OBJS = config.o

.PHONY: clean

all: main

main: main.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

config.o: config.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) main