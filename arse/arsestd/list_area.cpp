#include "list_area.h"

extern char *get_option(char *);

ListArea::~ListArea() { }

ListArea::ListArea() {
    _xpos = 1;
    _ypos = 1;
    _top = 0;
    _selected = 0;
}

void ListArea::init() {
    _win = newwin(LINES - 4, COLS, 4, 0);
    _length = LINES - 4 - _top;
    wbkgdset(_win, COLOR_PAIR(1));

    // unhack this: replace with config-derived file
    parse_master_gamelist("test_list3.txt", _master_rom_list);

    // Make a copy of the master rom list into the select list
    // All work is done in the select list, so this is necessary
    // to start with it populated from the master.
    _select_list = _master_rom_list;    
}

void ListArea::draw() {
    Rom* pRom = NULL;
    int i;
    int cpairbase = 0;

    // pair sets 1 - 4 and 5 - 8 are the same colors, but w/ diff backgrounds
    cpairbase = (_focus ? 4 : 0); // choose which color pair "set" to use

    wclear(_win);

    wattron(_win, A_BOLD | COLOR_PAIR(cpairbase + 1));
    // _top is the top of the "viewport" into _select_list. start there,
    // and display _length entries; start index == _top, end == _top + _length
    for (i = _top; i < _top + _length; i++) {
	if (i >= (int) _select_list.size())
	    break;
	if (i % 2 == 0) { // alternate text color by rows
	    wattroff(_win, A_BOLD | COLOR_PAIR(cpairbase + 1));
	    wattron(_win, A_BOLD | COLOR_PAIR(cpairbase + 2));
	} else {
	    wattroff(_win, A_BOLD | COLOR_PAIR(cpairbase + 2));
	    wattron(_win, A_BOLD | COLOR_PAIR(cpairbase + 1));
	}
	pRom = &_select_list[i];
	mvwprintw(_win, i - _top, 2, pRom->name.c_str());
    }
    mvwprintw(_win, _selected - _top, 1, ">");
    wrefresh(_win);
}

EventList ListArea::handleEvent(const Event & ev) {
    EventList evs;
    Event *e = NULL;

    // we only handle these two event types
    if ((ev.type != KEYPRESS) && (ev.type != USER_INPUT))
	return evs;
    // beyond that, we only handle KEYPRESS if we have focus
    if ((ev.type == KEYPRESS) && !_focus)
	return evs;

    // handle user input: downselect game list, and request a draw()
    if (ev.type == USER_INPUT) {
	if (ev.data == "") {
	    _select_list.clear();
	    _select_list = _master_rom_list;
	} else {
	    select_roms(_master_rom_list, _select_list, ev.data);
	}
	evs.push_back(*(e=new Event(this, CONTENT_CHANGE, ev.data)));
	delete e;
	return evs;
    }

    // implicitly, this means ev.type == KEYPRESS
    switch (ev.data[0]) {
	case P1_UP:
	case P2_UP:
	    _selected--;
	    if (_selected < 0)
		_selected = 0;
	    if (_selected < _top) {
		_top--;
		if (_top < 0)
		    _top = 0;
	    }
	    break;

	case P1_DOWN:
	case P2_DOWN:
	    _selected++;
	    if (_selected >= (int) _select_list.size())
		_selected = _select_list.size() - 1;
	    if (_selected >= _top + _length) {
		_top++;
		_selected = _top + _length - 1;
	    }
	    break;

	case P1_SELECT:
	case P2_SELECT:
	    evs.push_back(*(e = new Event(this, EXECUTE_ROM,
					  _select_list[_selected].rom_name)));
	    delete e;
	    break;

	default:
	    break;
    }

    // we always need a redraw after every input character
    evs.push_back(*(e=new Event(this, CONTENT_CHANGE, ev.data)));
    delete e;

    return evs;
}

int ListArea::focus() {
    _focus = true;
    wbkgdset(_win, COLOR_PAIR(5)); // set bgnd color for this window
    wbkgd(_win, COLOR_PAIR(5)); // make new color take effect now
    return(1);
}

void ListArea::unfocus() {
    _focus = false;
    wbkgdset(_win, COLOR_PAIR(1));
    wbkgd(_win, COLOR_PAIR(1));
}

static const string _list_area_name = "ListArea";
string ListArea::getName() {
    return _list_area_name;
}

Area* ListArea::getInstance() {
    return new ListArea();
}
