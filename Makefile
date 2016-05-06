CC := g++
SRCDIR := src
BUILDDIR := build
TARGET := bin/bdd

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -O2 -g -Wall -Wextra -Wpointer-arith -Wconversion -std=c++11 -march=native
LIB := -L lib
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

.PHONY: clean
