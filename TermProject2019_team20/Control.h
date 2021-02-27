#pragma once
#define WIDTH		400
#define HEIGHT		600
#define SPACE		32
#define ESC			27
#define ENTER		13
#define SPEED		12
#define FPS			120
#define BAR_TIME	5
#define RADIUS		20

#define boundaryX WIDTH/2
#define boundaryY HEIGHT/2

class Control
{
public:
	bool startgame = false;		// ����ȭ�鿡���� false, ������ ����Ǹ� true
	bool gameover = false;		// ���ӿ��� ����
	bool launch = false;		// ���� �߻�Ǿ� �����̴� ���ȿ��� true
	bool dark = true;			// ��ũ��� ����
	bool GOD_MODE = false;		// ����׿� GOD_MODE (��� ����)
	bool laser = false;			// ���� ��θ� �����ִ� ������ ����ġ
	bool clearMode = true;		// ���� �Է����� ���带 �����ϴ� ���, ���� �Է¿� �����ϸ� false. ����� ���� �Է��� ��Ű�� �ʰ� �������� �ʱ�ȭ��
	bool cleared = false;		// ���� Ŭ���� ����
	bool muted = false;			// �Ҹ� ���Ұ� ���� (�⺻���� ���)
	int score = 0;				// ���� ���ھ�
public:
	static Control control;		// �� �������� �� static ����� �̿��Ͽ� �����Ѵ�
};

