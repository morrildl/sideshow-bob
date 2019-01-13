input_area.o: input_area.cpp arsestd.h ../area.h ../event.h input_area.h \
  list_area.h rom.h
rom.o: rom.cpp rom.h
rom_sort.o: rom_sort.cpp rom.h
arsestd.o: arsestd.cpp arsestd.h ../area.h ../event.h input_area.h \
  list_area.h rom.h
list_area.o: list_area.cpp list_area.h ../area.h ../event.h rom.h \
  arsestd.h input_area.h
