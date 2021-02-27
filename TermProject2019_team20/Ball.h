#pragma once
#define _USE_MATH_DEFINES


#include <cmath>
#include <GL/glut.h>
#include "Control.h"
#include "Material.h"

enum Color {
	RED = 1 , GREEN, SILVER, YELLOW
};
// 3      2
//  \    /
//   \  /
//    \/
// 4-------1
enum Case {
	RIGHT = 1, RIGHT_UP, LEFT_UP, LEFT, LEFT_DOWN, RIGHT_DOWN, UP_SIDE, SIDE, PASS 
};

class Ball
{
protected:
	float pos[2];
	float vel[2];
	float properties[3];
	Material mtl;
	Color color; //11.15 승현 혼자 추가함.
public:
	Ball();
	Ball(float radius, float slices, float stacks);
	void setProperties(float r, float s, float st); //for test
	void setPos(float xpos, float ypos);
	void setVel(float xvel, float yvel);
	void move();
	float* getPos();
	float* getVel();
	Color getColor() const; // 11.15 승현 혼자 추가함.
	void setMTL(Color color);
	void draw();
	// 좌우 벽이나 위 벽에 충돌 체크
	int collisionDetection();
	void collisionHandling();

	int collisionDetection(Ball& ball);
	void collisionHandling(Ball& ball);
};

