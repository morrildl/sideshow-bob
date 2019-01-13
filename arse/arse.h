#ifndef ARSE_INCLUDE
#define ARSE_INCLUDE

#include <curses.h>

#define AREAS		2
#define ARSE_SMALL 	1
#define ARSE_NORMAL	2

extern int arse_style;
extern void redraw_areas();
extern void notify_rom_select();
char * get_option(char *);
#endif
