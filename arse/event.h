#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <list>
using std::string;
using std::list;

enum EventType {
    KEYPRESS,
    USER_INPUT,
    CONTENT_CHANGE,
    EXECUTE_ROM
};

class Event {
    public:
	Event(const void* source_, EventType type_, const string& data_);
	virtual ~Event();
	Event(const Event& ev);
	EventType type;
	string data;
	const void* source;

    protected:
	Event();
};

typedef list<Event> EventList;

#endif
