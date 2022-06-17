CFLAGS=-Wall -Wextra -Werror
BLDFLAGS=-O2
LIBS= -lpthread #-latomic
CC=clang++ --std=gnu++17
#CC=eg++

ODIR=./build
IDIR=./include
SRC=./src

TARGET=fft
HEAD=

_DEPS= main.cpp fft.cpp
DEPS=$(patsubst %,$(SRC)/%,$(_DEPS))
OBJ=$(patsubst %,$(ODIR)/%,${_DEPS:.cpp=.o})
HEAD=$(patsubst %,$(IDIR)/%,${_DEPS:.cpp=.hpp})

.PHONY: all clean $(TARGET)

all: $(TARGET) $(OBJ)


$(ODIR)/%.o: $(SRC)/%.cpp
	$(CC) -o $@ -c $< $(CFLAGS) -I $(IDIR) -g 

$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LIBS) $(BLDFLAGS) -I $(IDIR) -g

clean:
	rm -f $(ODIR)/*.o *~ *.core $(OBJ) $(TARGET)
