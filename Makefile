CXX = g++
CXXFLAGS = -m64 -O3 -g 

all:
	$(CXX) $(CXXFLAGS) -o bdd *.cpp *.h
clean:
	rm -rf *.o bdd
