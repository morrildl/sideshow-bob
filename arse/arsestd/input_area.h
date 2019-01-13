#ifndef INPUT_AREA_H
#define INPUT_AREA_H

#define CHOICE_LENGTH 25

class InputArea : public Area {
    public:
	InputArea();

	// Area abstract methods that need implementations
	virtual ~InputArea();  
	virtual void draw();
	virtual void init();
	virtual int focus();
	virtual void unfocus();
	virtual string getName();
	virtual EventList handleEvent(const Event&);

	static Area* getInstance();

    protected:
	int _idx;
	int _top_limit, _bottom_limit;
	int _xpos, _ypos;
	WINDOW* _win;
	bool _focus;
	string _choice;
	int _bgcolor;
};

#endif
