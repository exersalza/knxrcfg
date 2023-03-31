CXX = g++
CXX_FLAGS = -std=c++2a

main: config.o clean
	${CXX} ${CXX_FLAGS} -o a.out main.cpp config.cpp

config.o:
	${CXX} ${CXX_FLAGS} -nostartfiles config.cpp -o config.o


run: main
	./a.out

clean:
	@rm -rf *.out *.o