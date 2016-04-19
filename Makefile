CC := g++
SRCDIR := src
BUILDDIR := build
TARGET := bin/bdd
 
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -O3 -g -Wall -std=c++11
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
	make  # need to make the rest first
	$(CC) $(CFLAGS) examples/nqueens.cpp $(PBDD) $(INC) $(LIB) -o bin/nqueens

.PHONY: clean
