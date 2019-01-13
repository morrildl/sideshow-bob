#include <signal.h>
#include <curses.h>
#include <dlfcn.h>
#include <errno.h>

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
using std::map;
using std::string;
using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::ofstream;
using std::ios_base;

#include "area.h"
#include "framework.h"

// Bindings from mnemonic string ID to factory function, for creating Areas
map<string, area_factory_t> area_registry;

// Single instance of the core Framework class
Framework* framework = new Framework();

#ifdef DEBUG
ofstream logger;
#endif

/*
 * Curses-Related Functions
 */
void init_curses() {
    initscr();
    start_color();
    cbreak();
    leaveok(stdscr, FALSE);
    keypad(stdscr, TRUE);
    nonl();
    noecho();
    refresh();
    init_pair(1, COLOR_CYAN, COLOR_BLUE);
    init_pair(2, COLOR_YELLOW, COLOR_BLUE);
    init_pair(3, COLOR_GREEN, COLOR_BLUE);
    init_pair(4, COLOR_WHITE, COLOR_BLUE);
    init_pair(5, COLOR_CYAN, COLOR_RED);
    init_pair(6, COLOR_YELLOW, COLOR_RED);
    init_pair(7, COLOR_GREEN, COLOR_RED);
    init_pair(8, COLOR_WHITE, COLOR_RED);
}

void handle_winch(int signum) {
    // TODO: Fix the refresh when a window is resized.
    // At the moment, it looks like it updates to the size
    // one AFTER the change you've made. for example, if you size
    // it to 12 lines, it won't do anything. Then you size it to 
    // 8 lines, and it becomes 12 lines. It's really annoying.
    endwin();
    init_curses();
    refresh();
}


/*
 * Library-Related Functions
 */
int init_libraries(const list<string>& libs) {
    facmap_data* dat = NULL;
    void* sohndl = NULL;
    int i = 0;
    list<string>::const_iterator it;

    if (libs.empty()) {
	cerr << "init_libraries: ERROR: no libraries specified" << endl;
	return -1;
    }
    
    // for everything in the list of libraries...
    for (it = libs.begin(); it != libs.end(); ++it) {
	// first open the desired .so library
	sohndl = dlopen((*it).c_str(), RTLD_LAZY);
	if (sohndl == NULL) {
	    cerr << "init_libraries: ERROR:" << endl << dlerror() << endl;
	    return 1;
	}

	// next, fetch the standard symbol (i.e. array) from the .so
	dat = (facmap_data*)(dlsym(sohndl, AREA_LIBDATA_STRING));
	if (!dat) {
	    cerr << "init_libraries: ERROR: no symbol '" <<
		    AREA_LIBDATA_STRING << "' in '" << *it << "'." << endl;
	    //return 2;
	    continue;
	}

	// finally, add all the function pointers in the array to the registry
	i = 0;
	while (dat[i].funptr != NULL) {
	    if (area_registry[dat[i].name] != NULL) {
		cerr << "init_libraries: WARNING: '" << dat[i].name << 
			"' already mapped." << endl;
		++i;
		continue;
	    }
	    area_registry[dat[i].name] = dat[i].funptr;
	    ++i;
	}
    }

    return 0;
}


/*
 * It's main().  Dig it.
 */
int main(int argc, char** argv) {
    int c = 0;
    unsigned int i = 0;
    int rc = 0;
    list<string> libs;
    Event* ev;
    EventList evlist;
    EventList events;
    vector<Area*> areas;
    int cur_win = 0;
    string* s = NULL;

    // tell the system what to do when the user resizes the window
    signal(SIGWINCH, handle_winch);

    // delete loop below once parse_config above works & populates libs
    for (c = 1; c < argc; ++c) {
	libs.push_back(argv[c]);
    }
    // delete above 

    #ifdef DEBUG
    logger.open("./arse.out", ios_base::app);
    #endif

    // Get curses set up for use
    init_curses();

    rc = init_libraries(libs); // initialize libraries
    if (rc != 0) {
	cerr << argv[0] << ": Error during startup. Aborting." << endl;
	return rc;
    }
    
    // un-hardcode below w/ data from config file
    areas.push_back((*(area_registry["arsestd_input_area"]))());
    areas.push_back((*(area_registry["arsestd_list_area"]))());
    areas.push_back(framework);
    
    // Initialize all of the areas through their init function
    for(i=0; i<areas.size(); i++) {
	areas[i]->init();
    }

    // Give the default current window initial focus
    areas[cur_win]->focus();

    // Let the areas draw themselves for the first time
    for(i=0; i<areas.size(); i++) {
	areas[i]->draw();
    }

    // main loop - where the rubber meets the road
    for(;;) {
        c = getch();

	// Default key handlers
	if ((c == 'u') || (c == 'r')) {
	    // Cycle through all of the windows
	    areas[cur_win]->unfocus();
	    do { // certain windows aren't allowed to have focus.
	    	cur_win = ((cur_win + 1) % areas.size());
	    } while(areas[cur_win]->focus() == 0);
	    for  (i = 0; i < areas.size(); ++i) {
		areas[i]->draw();
	    }
	}

	if ((c == '\n') || (c == '\r'))
	    continue;
	
	if ((c == '1') || (c == '3')) {
	    break;
	}

	// make the keypress into an event
	events.clear();
	ev = new Event(framework, KEYPRESS,
		       ((*(s = new string())) += (char)c));
	events.push_back(*ev);
	delete ev;
	delete s;

	// keep processing the event list until it goes empty (which means
	// Areas stop adding events to it)
	while (!events.empty()) {
	    ev = &events.front();
	    for (i = 0; i < areas.size(); ++i) {
	        #ifdef DEBUG
	        logger << "Sending " << ev->type << "/ '" << ev->data <<
			  "' to '" << areas[i]->getName() << "'." << endl;
	        #endif
		if (ev->source != areas[i]) {
		    evlist = areas[i]->handleEvent(*ev);
		    #ifdef DEBUG
		    logger << "Adding " << evlist.size() << " events from '" <<
			      areas[i]->getName() << "'" << endl;
	    	    #endif
		    events.splice(events.end(), evlist);
		} 
	    }
	    events.pop_front();
	}
    }

    for (i = 0; i < areas.size(); ++i) {
	delete areas[i];
    }
    endwin();
    return 0;
}
