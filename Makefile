CC := g++
SRCDIR := src
BUILDDIR := build
TARGET := bin/bdd

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
WFLAGS := -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wnoexcept -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-null-sentinel -Wswitch-default -Wundef -Wno-unused -Wpointer-arith -Wconversion -Wpointer-arith -Wconversion 
CFLAGS := -O2 -g -std=c++11 -march=native $(WFLAGS)
LIB := -L lib -lpthread
INC := -I include
PBDD := build/pbdd/*.o

$(TARGET): $(OBJECTS)
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

nqueens:
	make
	$(CC) $(CFLAGS) examples/nqueens.cpp $(PBDD) $(INC) $(LIB) -o bin/nqueens

simple:
	make
	$(CC) $(CFLAGS) examples/simple.cpp $(PBDD) $(INC) $(LIB) -o bin/simple

tester:
	make
	$(CC) $(CFLAGS) test/test_all.cpp $(PBDD) $(INC) $(LIB) -o bin/test_all

nodemanager_tests:
	make
	$(CC) $(CFLAGS) test/nodemanager_tests.cpp $(PBDD) $(INC) $(LIB) -o bin/nodemanager_tests

c432:
	make
	$(CC) $(CFLAGS) examples/c432.cpp $(PBDD) $(INC) $(LIB) -o bin/c432

c1908:
	make
	$(CC) $(CFLAGS) examples/c1908.cpp $(PBDD) $(INC) $(LIB) -o bin/c1908

.PHONY: clean
