#ifndef AREA_H
#define AREA_H

#include <string>
using std::string;

#include "event.h"


class Area {
    public:
	virtual ~Area() { }
	//virtual Area& operator=(const Area&) = 0;
	virtual void draw() = 0;
	virtual void init() = 0;
	virtual int focus() = 0;
	virtual void unfocus() = 0;
	virtual string getName() = 0;
	virtual EventList handleEvent(const Event&) = 0;
};

typedef Area* (*area_factory_t)();

struct facmap_data {
    string name;
    area_factory_t funptr;
};

#define AREA_LIBDATA_SYMBOL lib_data
#define AREA_LIBDATA_STRING "lib_data"

#endif
