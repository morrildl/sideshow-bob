TARGET=arse
OFILES=\
	event.o \
	arse.o
CPPFILES=\
	event.cpp \
	arse.cpp
LIB_CPPFILES=\
	framework.cpp
LIB_OFILES=\
	framework.o
LIB_TARGET=libarsecore.so

VOLATILES=$(OFILES) $(TARGET) $(LIB_TARGET) $(LIB_OFILES)
DEPFILE=deps.mk
LIB_DEPFILE=lib_deps.mk
DEPFLAGS=-MM
LIBS=-lncurses -ldl

CPPFLAGS=-g -Wall -DDEBUG
CC=g++

all: std $(LIB_TARGET) $(TARGET)
$(TARGET): deps $(OFILES)
	$(CC) -rdynamic -fPIC -L. -larsecore $(LIBS) -o $(TARGET) $(OFILES)
$(LIB_TARGET): deps $(LIB_OFILES)
	$(CC) -shared -rdynamic -fPIC $(LIBS) -o $(LIB_TARGET) $(LIB_OFILES)
clean:
	rm -f $(VOLATILES)
deps:
	$(CC) $(DEPFLAGS) $(CPPFILES) > $(DEPFILE)
	$(CC) $(DEPFLAGS) $(LIB_CPPFILES) > $(LIB_DEPFILE)
include $(DEPFILE)
include $(LIB_DEPFILE)
std:
	$(MAKE) -C arsestd 
