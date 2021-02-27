#pragma once
#include <vector>
#include "Ball.h"

using namespace std;

class Board_Ball :
	public Ball
{
private:
	bool exist = false;	// 공의 존재 여부
	bool visited = false;	// remove 과정에서 한번 확인한 공은 더이상 확인하지 않도록 하는 변수
	bool toBeDeleted = false;	// 붙은 공과 같은 색깔인지 확인. 같으면 true. main.cpp에서 같은색이 3개 이상이면 제거하고, reset한다.
	bool drop_mark = true;	// 보드에 있는 공을 체크해서, 공이 붙어 있으면 false로 바꿈. 체크 후에 true로 남아있으면 drop
	
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

