#include "Arrow.h"

Arrow::Arrow() {
	angle = 90;
}
int Arrow::getAngle() {
	return angle;
}
// angle 값만큼 시계방향으로 회전
void Arrow::move_right(int angle) {
	this->angle -= angle;
}
// angle 값만큼 반시계방향으로 회전
void Arrow::move_left(int angle) {
	this->angle += angle;
}
// angle을 90도로 reset
void Arrow::angleReset() {
	this->angle = 90;
}
// arrow에 색 설정
void Arrow::setMTL() {
	mtl.setEmission(0.1, 0.1, 0.1, 1);
	mtl.setAmbient(0.05, 0.05, 0.05, 1.0);
	mtl.setDiffuse(0.5, 0.5, 0.5, 1.0);
	mtl.setSpecular(0.7, 0.7, 0.7, 1.0);
	mtl.setShininess(10);
}
void Arrow::draw() {
	// gluCylinder()를 사용하기 위해서 포인터 변수 선언
	GLUquadricObj* quadObj = gluNewQuadric();

	gluQuadricDrawStyle(quadObj, GLU_FILL);

	// Material 설정
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_EMISSION, mtl.getEmission());
	glMaterialfv(GL_FRONT, GL_AMBIENT, mtl.getAmbient());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mtl.getDiffuse());
	glMaterialfv(GL_FRONT, GL_SPECULAR, mtl.getSpecular());
	glMaterialfv(GL_FRONT, GL_SHININESS, mtl.getShininess());

	// 좌표계 기본 상태
	glPushMatrix();
		glTranslatef(0, -260, 0);

		// x축이 화면을 뚫고 들어가는 상태. angle이 0이면 z축은 오른쪽, 180이면 z축은 왼쪽
		glPushMatrix();
			glRotatef(90, 0, 1, 0);
			glRotatef((GLfloat)angle, -1, 0, 0);
			// gluCylinder 의 기본 위치는 아래 면이 z=0 평면이므로 원기둥 중심을 축으로 회전시키기 위해서는 좌표계를 원기둥 높이의 반만큼 내려야 한다.
			glTranslatef(0, 0, -CYLINDER_HEIGHT/2);
			gluCylinder(quadObj, 8, 8, CYLINDER_HEIGHT, 10, 10);

			glPushMatrix();
				// 좌표계를 원기둥 높이만큼 올려서 원기둥 윗면에 원뿔이 그려지도록 한다.
				glTranslatef(0, 0, CYLINDER_HEIGHT);
				//glColor3f(1, 1, 0);
				glutSolidCone(20, CONE_HEIGHT, 10, 10);
			glPopMatrix();

		glPopMatrix();

	glPopMatrix();

}