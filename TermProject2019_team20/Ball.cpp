#include "Ball.h"


Ball::Ball() {}

Ball::Ball(float radius, float slices, float stacks) {
	properties[0] = radius;
	properties[1] = slices;
	properties[2] = stacks;
	
}

void Ball::setProperties(float r, float s, float st) {
	properties[0] = r;
	properties[1] = s;
	properties[2] = st;
}
void Ball::setPos(float xpos, float ypos) {
	pos[0] = xpos;
	pos[1] = ypos;
}
void Ball::setVel(float xvel, float yvel) {
	vel[0] = xvel;
	vel[1] = yvel;
}

void Ball::move() {
	pos[0] += vel[0];
	pos[1] += vel[1];
}
float* Ball::getPos() {
	return pos;
}
float* Ball::getVel() {
	return vel;
}

void Ball::setMTL(Color color) {
	// emission�� �����̹Ƿ� �տ� ���� ����
	this->color = color; // 11.15 ���� �߰�
	mtl.setEmission(0.1, 0.1, 0.1, 1);
	
	switch (color) {
	case RED:
		mtl.setAmbient(0.0, 0.0, 0.0, 1.0);
		mtl.setDiffuse(0.5, 0.0, 0.0, 1.0);
		mtl.setSpecular(0.7, 0.6, 0.6, 1.0);
		mtl.setShininess(32);
		break;
	case GREEN:
		mtl.setAmbient(0.0215, 0.1745, 0.0215, 1.0);
		mtl.setDiffuse(0.07568, 0.61424, 0.07568, 1.0);
		mtl.setSpecular(0.633, 0.727811, 0.633, 1.0);
		mtl.setShininess(76.8);
		break;
	case YELLOW:
		mtl.setAmbient(0.0, 0.0, 0.0, 1.0);
		mtl.setDiffuse(0.5, 0.5, 0.0, 1.0);
		mtl.setSpecular(0.6, 0.6, 0.5, 1.0);
		mtl.setShininess(32);
		break;
	case SILVER:
		mtl.setAmbient(0.19225, 0.19225, 0.19225, 1.0);
		mtl.setDiffuse(0.50754, 0.50754, 0.50754, 1.0);
		mtl.setSpecular(0.508273, 0.508273, 0.508273, 1.0);
		mtl.setShininess(51.2);
		break;
	default:
		break;
	}
}
void Ball::draw() {
	// Material ����
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_EMISSION, mtl.getEmission());
	glMaterialfv(GL_FRONT, GL_AMBIENT, mtl.getAmbient());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mtl.getDiffuse());
	glMaterialfv(GL_FRONT, GL_SPECULAR, mtl.getSpecular());
	glMaterialfv(GL_FRONT, GL_SHININESS, mtl.getShininess());

	glPushMatrix();
		glTranslatef(pos[0], pos[1], -100);
		glutSolidSphere(properties[0], properties[1], properties[2]);
	glPopMatrix();
}
// �� ���̳� �� ���� ����� ��츦 üũ�ϴ� �Լ�
int Ball::collisionDetection() {
	//�� �� �浹�ϴ� ������ ���� ������ 4�� ������.
	if (this->getPos()[0] + RADIUS >= boundaryX-4 || this->getPos()[0] - RADIUS <= -boundaryX+4) {
		return SIDE;
	}
	if (this->getPos()[1] + RADIUS >= boundaryY-100) {
		return UP_SIDE;
	}
	else return -1;
}

// ������ collision�� �˻��ϴ� �Լ�
int Ball::collisionDetection(Ball& ball) {

	float dx = this->getPos()[0] - ball.getPos()[0];
	float dy = this->getPos()[1] - ball.getPos()[1];
	float r = RADIUS * 2;

	if (dx * dx + dy * dy <= r*r) {
		// �߻�� ���� ��ġ�� ���⿡ ���� ������ ������ ����. (header file �ּ� ����)
		float tan = dy / dx;
		if (dx >= 0) {
			if (tan >= 1 / sqrt(3)) return RIGHT_UP;
			else if (tan > -1 / sqrt(3)) return RIGHT;
			else return RIGHT_DOWN;
		}
		else {
			if (tan >= 1 / sqrt(3)) return LEFT_DOWN;
			else if (tan > -1 / sqrt(3)) return LEFT;
			else return LEFT_UP;
		}
	}
	// �ƹ� ������ ������ PASS
	return PASS;
}

// ���� �浹�Ͽ��� �� collision handling
void Ball::collisionHandling() {
	int case_value = this->collisionDetection();
	if (case_value == SIDE)
		this->setVel(-this->getVel()[0], this->getVel()[1]);
	if (case_value == UP_SIDE) {
		this->setVel(0, 0);
		this->setPos(((int)(this->getPos()[0]+200) / 40) * 40 -180, 180);
		Control::control.launch = false;
	}
}

// ���� �浹 �Ͽ��� �� collision handling
void Ball::collisionHandling(Ball& ball) {
	int case_value = this->collisionDetection(ball);
	switch (case_value) {
	case RIGHT:
		this->setVel(0, 0);
		this->setPos(ball.getPos()[0] + RADIUS*2, ball.getPos()[1]);
		Control::control.launch = false;
		break;
	case RIGHT_UP:
		this->setVel(0, 0);
		this->setPos(ball.getPos()[0] + RADIUS, ball.getPos()[1] + RADIUS * sqrt(3));
		Control::control.launch = false;
		break;
	case LEFT_UP:
		this->setVel(0, 0);
		this->setPos(ball.getPos()[0] - RADIUS, ball.getPos()[1] + RADIUS * sqrt(3));
		Control::control.launch = false;
		break;
	case LEFT:
		this->setVel(0, 0);
		this->setPos(ball.getPos()[0] - RADIUS*2, ball.getPos()[1]);
		Control::control.launch = false;
		break;
	case LEFT_DOWN:
		this->setVel(0, 0);
		if (ball.getPos()[0] == -boundaryX + RADIUS) this->setPos(-boundaryX + RADIUS * 2, ball.getPos()[1] - RADIUS * sqrt(3));
		else
			this->setPos(ball.getPos()[0] - RADIUS, ball.getPos()[1] - RADIUS * sqrt(3));
		Control::control.launch = false;
		break;
	case RIGHT_DOWN:
		this->setVel(0, 0);
		if (ball.getPos()[0] == boundaryX - RADIUS) this->setPos(boundaryX - RADIUS * 2, ball.getPos()[1] - RADIUS * sqrt(3));
		else
			this->setPos(ball.getPos()[0] + RADIUS, ball.getPos()[1] - RADIUS * sqrt(3));
		Control::control.launch = false;
		break;

	case PASS:
		break;
	}
}

Color Ball::getColor() const {
	return color;
}
// 11.15 ���� �߰�
