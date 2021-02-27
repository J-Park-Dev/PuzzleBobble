#pragma once
#include <GL/glut.h>
#include "Control.h"

class TimeBar
{
private:
	int remainTime;
public:
	TimeBar();
	void decrease();
	void reset();
	int getRemaintime();
	void draw();
};

