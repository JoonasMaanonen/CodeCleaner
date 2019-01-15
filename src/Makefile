IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR) -Wall -pedantic

ODIR=obj
LDIR =../lib

LIBS=

_DEPS = currTime.h parser.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = currTime.o parser.o cleaner.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

cleaner: $(OBJ)
		gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
		rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
