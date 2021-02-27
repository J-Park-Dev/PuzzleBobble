#pragma once
#include <vector>
#include "Ball.h"

using namespace std;

class Board_Ball :
	public Ball
{
private:
	bool exist = false;	// ���� ���� ����
	bool visited = false;	// remove �������� �ѹ� Ȯ���� ���� ���̻� Ȯ������ �ʵ��� �ϴ� ����
	bool toBeDeleted = false;	// ���� ���� ���� �������� Ȯ��. ������ true. main.cpp���� �������� 3�� �̻��̸� �����ϰ�, reset�Ѵ�.
	bool drop_mark = true;	// ���忡 �ִ� ���� üũ�ؼ�, ���� �پ� ������ false�� �ٲ�. üũ �Ŀ� true�� ���������� drop
	
public:
	Board_Ball();


	Board_Ball(Ball ball);
	void setExist(bool e);
	void setVisited(bool v);
	void setToBeDeleted(bool d);
	void setDropMark(bool dm);

	bool isExist() const;
	bool isVisited() const;
	bool istoBeDeleted() const;
	bool getDropMark() const;
	
	void reset();
};

