#include "Board_Ball.h"
#include "Board_Ball.h"

Board_Ball::Board_Ball() {
}


Board_Ball::Board_Ball(Ball ball) : Ball(ball) {
	exist = true;
	visited = false;
	toBeDeleted = false;
}
void Board_Ball::setExist(bool e) {
	exist = e;
}
void Board_Ball::setVisited(bool v) {
	visited = v;
}

void Board_Ball::setToBeDeleted(bool d) {
	toBeDeleted = d;
}

void Board_Ball::setDropMark(bool dm) {
	drop_mark = dm;
}
bool Board_Ball::isExist() const {
	return exist;
}
bool Board_Ball::isVisited() const {
	return visited;
}
bool Board_Ball::istoBeDeleted() const {
	return toBeDeleted;
}

bool Board_Ball::getDropMark() const {
	return drop_mark;
}
void Board_Ball::reset() {
	visited = false;
	toBeDeleted = false;
}

