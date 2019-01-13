TARGET=libarsestd.so
OFILES=\
	input_area.o \
	rom.o \
	rom_sort.o  \
	arsestd.o \
	list_area.o \

CPPFILES=\
	input_area.cpp \
	rom.cpp \
	rom_sort.cpp \
	arsestd.cpp \
	list_area.cpp \

VOLATILES=$(OFILES) $(TARGET)
DEPFILE=deps.mk
DEPFLAGS=-MM
LIBS=-lncurses
INCLUDE=-I.. -I.

CPPFLAGS=-g -Wall $(INCLUDE)
CC=g++

$(TARGET): deps $(OFILES)
	$(CC) -shared $(LIBS) -o $(TARGET) $(OFILES)
clean:
	rm -f $(VOLATILES)
deps:
	$(CC) $(CPPFLAGS) $(DEPFLAGS) $(CPPFILES) > $(DEPFILE)
include $(DEPFILE)
