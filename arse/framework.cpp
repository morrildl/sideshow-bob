#include <string>
#include <iostream>
using std::string;
using std::cout;
using std::endl;

#include "framework.h"

Framework::Framework() {
}

Framework::~Framework() {
}

void Framework::do_exec(string rom) {
    int status;
    pid_t pid;
   
    pid = fork();
    if (pid == 0) {
	execlp("./arse_exec.sh", "./arse_exec.sh", rom.c_str(), 0);
    } else {
	waitpid(pid, &status, 0);
    }
}

static const string name = "Framework";

string Framework::getName() {
    return name;
}

EventList Framework::handleEvent(const Event& ev) {
    EventList list;

    if (ev.type == EXECUTE_ROM)
	do_exec(ev.data);

    if (ev.type == CONTENT_CHANGE) {
	((Area*)(ev.source))->draw();
    }

    return list;
}

Area& Framework::operator=(const Area&) {
    return *this;
}

void Framework::draw() { }

void Framework::init() { }

int Framework::focus() { return(0); }

void Framework::unfocus() { }
