IDIR = include
CC = g++
CFLAGS = -I$(IDIR)

ODIR = obj
LDIR =../lib

LIBS=-lm -lncurses

_DEPS=MazeCell.h Maze.h defs.h Player.h
DEPS= $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ=MazeCell.o main.o Maze.o Player.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~

run: all
	./all
