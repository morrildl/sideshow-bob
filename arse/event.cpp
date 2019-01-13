#include "event.h"

Event::Event(const void* source_, EventType type_, const string& data_)
//: type(type_), data(data_), source(source_)
{
    type = type_;
    data = data_;
    source = source_;
}

Event::~Event() { }

Event::Event(const Event& ev)
    : type(ev.type), data(ev.data), source(ev.source)
{ }

/*
Event& Event::operator=(const Event& ev) {
    
}
*/
