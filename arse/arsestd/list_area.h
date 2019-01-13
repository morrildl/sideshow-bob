#ifndef LIST_AREA_H
#define LIST_AREA_H

#include <ncurses.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "area.h"
#include "rom.h"
#include "arsestd.h"

class ListArea : public Area {
  public:
    ListArea();
    static Area* getInstance();

    // abstract methods from Area that need implementing
    virtual ~ListArea();
    virtual void draw();
    virtual void init();
    virtual int focus();
    virtual void unfocus();
    virtual string getName();
    virtual EventList handleEvent(const Event &);

  protected:
    int _xpos, _ypos;
    int _top, _length, _selected;
    bool _focus;
    WINDOW *_win;
    RomList _master_rom_list;
    RomList _select_list;
};

int parse_master_gamelist(const string& path, RomList& pList);
void select_roms(const RomList& pick_list, RomList& found_list,
		 const string& key);
#endif
