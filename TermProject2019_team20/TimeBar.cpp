#include "TimeBar.h"


TimeBar::TimeBar() : remainTime(BAR_TIME * FPS * 0.8) {}
void TimeBar::decrease() {
	remainTime--;
}
void TimeBar::reset() {
	remainTime = BAR_TIME * FPS * 0.8;
}
int TimeBar::getRemaintime() {
	return remainTime;
}
void TimeBar::draw() {
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glDisable(GL_LIGHTING);
	glBegin(GL_QUADS);
	glVertex3f(30, 240, 0);
	glVertex3f(30, 230, 0);
	glVertex3f(30 + (remainTime / (float)(BAR_TIME * FPS)) * 150, 230, 0);
	glVertex3f(30 + (remainTime / (float)(BAR_TIME * FPS)) * 150, 240, 0);
	glEnd();
	glEnable(GL_LIGHTING);
	glPopMatrix();
}