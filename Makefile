CXX = g++
CXXFLAGS = -std=c++11 -m64 -O3 -g

all:
	$(CXX) $(CXXFLAGS) -o bdd *.cpp *.h
clean:
	rm -rf *.o bdd
