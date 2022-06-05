CFLAGS=-Wall -Wextra -Werror
BLDFLAGS=-O2
DBGFLAGS=-g
LIBS=#-lphread -latomic
CC=clang++

ODIR=./build
IDIR=./include
SRC=./src

TARGET=fft
HEAD=

_DEPS=
DEPS=$(patsubst %,$(SRC)/%,$(_DEPS))
OBJ=$(patsubst %,$(ODIR)/%,${_DEPS:.c=.o})

.PHONY: all clean dbg $(TARGET)

all: $(TARGET)

$(OBJ): $(HEAD)

$(ODIR)/%.o: $(SRC)/%.c $(DEPS)
	$(CC) -o $@ -c $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ main.cpp $(OBJ) $(LIBS) $(BLDFLAGS)

dbg: $(OBJ)
	$(CC) -o $(TARGET) main.cpp $(OBJ) $(LIBS) $(BLDFLAGS) $(DBGFLAGS)

clean:
	rm -f $(ODIR)/*.o *~ *.core $(OBJ) $(TARGET)
