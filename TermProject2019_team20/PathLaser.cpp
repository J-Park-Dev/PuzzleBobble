#include "PathLaser.h"

void PathLaser::draw(int angle) {
	double radian = (angle * M_PI / 180);
	glPushMatrix();
	// PathLaser 선을 부드럽게 하기 위해 안티앨리어싱 함
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	// 좌표계를 발사 지점으로 이동시킨다.
	glTranslatef(0, -260, -1.0);
	glColor3f(1.0, 0.0, 0.0);
	glDisable(GL_LIGHTING);
	glLineWidth(2.0);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 0, 0);
	
	// 오른쪽 side에 닿을 경우
	if (radian >= 0 && radian < atan((double)460/(double)180)) {
		glVertex3f(180, 180 * tan(radian), 0);
		// 오른쪽 side에 반사되어 위쪽 side로 향할 경우
		if ((460 - 180 * tan(radian)) * tan(M_PI / 2 - radian) < 380)
			glVertex3f(180 - (460 - 180 * tan(radian)) * tan(M_PI / 2 - radian), 460 - RADIUS, 0);
		// 오른쪽 side에 반사되어 왼쪽 side로 향할 경우
		else
			glVertex3f(-180, 180 * tan(radian) + 360 * tan(radian), 0);
	}
	// 왼쪽 side에 닿을 경우
	else if (radian > M_PI - atan((double)460 / (double)180)) {
		glVertex3f(-180, -180 * tan(radian), 0);
		// 왼쪽 side에 반사되어 위쪽 side로 향할 경우
		if ((460 + 180 * tan(radian)) * tan(M_PI / 2 + radian) < 380)
			glVertex3f(-180 + (460 + 180 * tan(radian)) * tan(M_PI / 2 + radian), 460 - RADIUS, 0);
		// 왼쪽 side에 반사되어 오른쪽 side로 향할 경우
		else
			glVertex3f(180, -180 * tan(radian) - 360 * tan(radian), 0);
	}
	// 위쪽 side에 닿을 경우
	else if (radian != M_PI / 2) {
		glVertex3f(460 * tan(M_PI / 2 - radian), 460 - RADIUS, 0);
	}
	// angle == 90
	else
		glVertex3f(0, 460, 0);

	glEnd();
	glEnable(GL_LIGHTING);
	glPopMatrix();
}