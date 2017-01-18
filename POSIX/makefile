########################################
#
#  IUPUI CSCI 503 -- Operating Systems
#
########################################
## Compiler, tools and options
CC      = gcc
LINK    = gcc
OPT     = -lrt

CCFLAGS = $(OPT)
LDFLAGS = $(OPT)

## Libraries
LIBS = 
INC  = -I.

## FILES
OBJECTS = posix.o
TARGET  = posix


## Implicit rules
.SUFFIXES: .c
.c.o:
	$(CC) -c $(CCFLAGS) $(INC) $(OPT) $<

## Build rules
all: $(TARGET) 

$(TARGET): $(OBJECTS)
	$(LINK) -o $@ $(OBJECTS) $(LDFLAGS) $(LIBS)

clean:
	rm -f $(OBJECTS) $(TARGET) 
	rm -f *~ core
