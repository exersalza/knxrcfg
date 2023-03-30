CXX = g++
CXX_FLAGS = -std=c++2a

main: config.o
	${CXX} ${CXX_FLAGS} -o a.out main.cpp config.cpp

config.o:
	${CXX} ${CXX_FLAGS} -nostartfiles config.cpp -o config.o


clean:
	rm -rf *.out *.o