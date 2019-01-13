#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

#include <string>
using std::string;

#include "area.h"

class Framework : public Area {
    private:
	void do_exec(string rom);

    public:
	Framework();
	virtual ~Framework();

	virtual Area& operator=(const Area&);

	virtual EventList handleEvent(const Event& ev);
	virtual void draw();
	virtual void init();
	virtual int focus();
	virtual void unfocus();
	virtual string getName();
};

#endif
