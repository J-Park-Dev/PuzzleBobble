#pragma once
#include <GL/glut.h>
#include "Material.h"

#define CYLINDER_HEIGHT 70
#define CONE_HEIGHT 30

class Arrow
{
private:
	int angle;
	Material mtl;
public:
	Arrow();
	int getAngle();
	void move_right(int angle);
	void move_left(int angle);
	void angleReset();
	void setMTL();
	void draw();
};

