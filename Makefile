CXX = g++
CXXFLAGS = -std=c++11 -m64 -O3 -g

all:
	$(CXX) $(CXXFLAGS) -o bdd *.cpp *.h
clean:
	rm -rf *.o bdd
nqueens:
	$(CXX) $(CXXFLAGS) -o nqueens memo_table.cpp unique_table.cpp bdd.cpp *.h examples/nqueens.*
