CFLAGS=-Wall -Wextra -Werror
BLDFLAGS=-O2
DBGFLAGS=-g
LIBS= #-lpthread #-latomic
CC=clang++

ODIR=./build
IDIR=./include
SRC=./src

TARGET=fft
HEAD=

_DEPS=threadpool.cpp
DEPS=$(patsubst %,$(SRC)/%,$(_DEPS))
OBJ=$(patsubst %,$(ODIR)/%,${_DEPS:.cpp=.o})
HEAD=$(patsubst %,$(IDIR)/%,${_DEPS:.cpp=.h})

.PHONY: all clean dbg $(TARGET)

all: $(TARGET) $(OBJ)


$(ODIR)/%.o: $(SRC)/%.cpp $(DEPS)
	$(CC) -o $@ -c $< $(CFLAGS) -I $(IDIR)

$(TARGET): $(OBJ)
	$(CC) -o $@ main.cpp $(OBJ) $(LIBS) $(BLDFLAGS) -I $(IDIR)

dbg: $(OBJ)
	$(CC) -o $(TARGET) main.cpp $(OBJ) $(LIBS) $(BLDFLAGS) $(DBGFLAGS) -I $(IDIR)

clean:
	rm -f $(ODIR)/*.o *~ *.core $(OBJ) $(TARGET)
