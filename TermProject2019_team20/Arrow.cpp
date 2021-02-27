#include "Arrow.h"

Arrow::Arrow() {
	angle = 90;
}
int Arrow::getAngle() {
	return angle;
}
// angle ����ŭ �ð�������� ȸ��
void Arrow::move_right(int angle) {
	this->angle -= angle;
}
// angle ����ŭ �ݽð�������� ȸ��
void Arrow::move_left(int angle) {
	this->angle += angle;
}
// angle�� 90���� reset
void Arrow::angleReset() {
	this->angle = 90;
}
// arrow�� �� ����
void Arrow::setMTL() {
	mtl.setEmission(0.1, 0.1, 0.1, 1);
	mtl.setAmbient(0.05, 0.05, 0.05, 1.0);
	mtl.setDiffuse(0.5, 0.5, 0.5, 1.0);
	mtl.setSpecular(0.7, 0.7, 0.7, 1.0);
	mtl.setShininess(10);
}
void Arrow::draw() {
	// gluCylinder()�� ����ϱ� ���ؼ� ������ ���� ����
	GLUquadricObj* quadObj = gluNewQuadric();

	gluQuadricDrawStyle(quadObj, GLU_FILL);

	// Material ����
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_EMISSION, mtl.getEmission());
	glMaterialfv(GL_FRONT, GL_AMBIENT, mtl.getAmbient());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mtl.getDiffuse());
	glMaterialfv(GL_FRONT, GL_SPECULAR, mtl.getSpecular());
	glMaterialfv(GL_FRONT, GL_SHININESS, mtl.getShininess());

	// ��ǥ�� �⺻ ����
	glPushMatrix();
		glTranslatef(0, -260, 0);

		// x���� ȭ���� �հ� ���� ����. angle�� 0�̸� z���� ������, 180�̸� z���� ����
		glPushMatrix();
			glRotatef(90, 0, 1, 0);
			glRotatef((GLfloat)angle, -1, 0, 0);
			// gluCylinder �� �⺻ ��ġ�� �Ʒ� ���� z=0 ����̹Ƿ� ����� �߽��� ������ ȸ����Ű�� ���ؼ��� ��ǥ�踦 ����� ������ �ݸ�ŭ ������ �Ѵ�.
			glTranslatef(0, 0, -CYLINDER_HEIGHT/2);
			gluCylinder(quadObj, 8, 8, CYLINDER_HEIGHT, 10, 10);

			glPushMatrix();
				// ��ǥ�踦 ����� ���̸�ŭ �÷��� ����� ���鿡 ������ �׷������� �Ѵ�.
				glTranslatef(0, 0, CYLINDER_HEIGHT);
				//glColor3f(1, 1, 0);
				glutSolidCone(20, CONE_HEIGHT, 10, 10);
			glPopMatrix();

		glPopMatrix();

	glPopMatrix();

}