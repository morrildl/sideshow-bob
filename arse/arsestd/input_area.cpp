#include "arsestd.h"

InputArea::~InputArea() { }

InputArea::InputArea() {
    _ypos = 2;
    _xpos = 2;
    _idx = 0;
    _top_limit = 0;
    _bottom_limit = 2;
    _focus = false;
    _choice.clear();
}

void InputArea::init() {
    _win = newwin(4, COLS, 0, 0);
    wbkgdset(_win, COLOR_PAIR(1));
}

int InputArea::focus() {
    _focus = true;
    wbkgdset(_win, COLOR_PAIR(5)); // set bgnd color for this window
    wbkgd(_win, COLOR_PAIR(5)); // make new color take effect now
    return(1);
}

void InputArea::unfocus() {
    _focus = false;
    wbkgdset(_win, COLOR_PAIR(1));
    wbkgd(_win, COLOR_PAIR(1));
}

void InputArea::draw() {
    int cpairbase = 0;

    // pair sets 1 - 4 and 5 - 8 are the same colors, but w/ diff backgrounds
    cpairbase = (_focus ? 4 : 0); // choose which color pair "set" to use

    werase(_win);

    wattron(_win, A_BOLD | COLOR_PAIR(cpairbase + 1));
    mvwprintw(_win, 0, 2, "A B C D E F G H I");
    mvwprintw(_win, 1, 2, "J K L M N O P Q R");
    mvwprintw(_win, 2, 2, "S T U V W X Y Z  ");
    wattron(_win, A_BOLD | COLOR_PAIR(cpairbase + 2));
    mvwprintw(_win, 0, 19, " | ");
    mvwprintw(_win, 1, 19, " | ");
    mvwprintw(_win, 2, 19, " | ");
    wattron(_win, A_BOLD | COLOR_PAIR(cpairbase + 3));
    mvwprintw(_win, 0, 22, "1 2 3 4 5 ");
    mvwprintw(_win, 1, 22, "6 7 8 9 0 ");
    mvwprintw(_win, 2, 22, "< _ !     ");
    wattron(_win, A_BOLD | COLOR_PAIR(cpairbase + 1));
    mvwprintw(_win, 2, 50, "              ");
    mvwprintw(_win, 0, 32, ":%s", _choice.c_str());
    /*
    if(selected_rom != NULL)
        mvwprintw(_win, 1, 32, "ROM: %s", selected_rom->rom_name);
    */

    if (_focus)
	wmove(_win, _ypos, _xpos);
    wrefresh(_win);
}

EventList InputArea::handleEvent(const Event& ev) {
    char in_ch;
    EventList evs;
    Event* e = NULL;

    if ((ev.type != KEYPRESS) || !_focus)
	return evs;

    switch (ev.data[0]) {
	case P1_LEFT:
	case P2_LEFT:
	    _xpos -= 2;
	    if (_xpos < 2)
		_xpos = 2;
	    break;

	case P1_RIGHT:
	case P2_RIGHT:
	    _xpos += 2;
	    break;

	case P1_UP:
	case P2_UP:
	    _ypos--;
	    if (_ypos < _top_limit)
		_ypos = _bottom_limit;
	    break;

	case P1_DOWN:
	case P2_DOWN:
	    _ypos++;
	    if (_ypos > _bottom_limit)
		_ypos = _top_limit;
	    break;

	case P1_SELECT:
	case P2_SELECT:
	    in_ch = mvwinch(_win, _ypos, _xpos);
	    switch (in_ch) {
		case '|':
		    break;
		case '<':
		    if (_choice.size() > 0)
			_choice.resize(_choice.size() - 1);
		    break;
		case '_':
		    _choice += ' ';
		    break;
		case '!':
		    _choice.clear();
		    break;
		default:
		    _choice += in_ch;
		    break;
	    }

	    evs.push_back(*(e = new Event(this, USER_INPUT, _choice)));
	    delete e;
	    break;

	default:
	    break;
    }

    //wrefresh(_win);

    evs.push_back(*(e = new Event(this, CONTENT_CHANGE, _choice)));
    delete e;
    return evs;
}

static const string _input_area_name = "InputArea";
string InputArea::getName() {
    return _input_area_name;
}

Area* InputArea::getInstance() {
    return new InputArea();
}
