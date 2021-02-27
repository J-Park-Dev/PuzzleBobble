//////////////////////////////////////////////////////////////////////////
//																		//
//					"Puzzle Bobble"										//
//																		//
//					Fall Semester 2019									//
//					Programming Methodologies Term Project				//
//					Team 20												//
//																		//
//////////////////////////////////////////////////////////////////////////

// for console output
#include <Windows.h>

// for sound
#include <irrKlang/irrKlang.h>
#pragma comment(lib, "irrKlang.lib")
using namespace irrklang;

// 11.25 for load data
#include <fstream>

#define _USE_MATH_DEFINES
#include <cstdlib>
#include <vector>
#include <time.h>
#include "Control.h"
#include "Light.h"
#include "Ball.h"
#include "Arrow.h"
#include "Board_Ball.h"
#include "Texture.h"
#include "TimeBar.h"
#include "PathLaser.h"

using namespace std;

clock_t start_clock = clock();
clock_t end_clock;

// FPS checker
double lastTime = clock() / 1000.0;
int nbFrames = 0;
int currentFrames = 0;

// 11.22 빛 하나 더 추가
Light* light0;
Light* light1;

Arrow arrow;
TimeBar timebar;
PathLaser pathlaser;

// 발사 전의 공과 next 공을 포함한 벡터
vector<Ball> ball_not_launched;
// 보드에 존재하는 모든 공을 저장, exist 변수로 존재 여부 판별
vector<Board_Ball*> board_balls;
// drop 시킬 공을 저장
vector<Ball> dropball;

// 11.23 Texture vector 생성
vector<Texture*> tex;

// for sound
// 사운드 장치가 없으면 engine == NULL
ISoundEngine* engine = createIrrKlangDevice();
ISound* opening;
ISound* maintheme;
ISound* ending;
ISound* cleartheme;

void gotoCursor(int x, int y);
void instruction() {
	// cout << "─│┌┐┘└" << endl;
	// 맨 윗줄 그리기
	gotoCursor(0, 8); printf("┌");	for (int i = 0; i < 92; i++) printf("─");	printf("┐\n");
	// 좌우 세로줄 그리기
	for (int i = 0; i < 22; i++) {
		printf("│");	gotoCursor(93, i + 9);	printf("│\n");
	}
	// 중간 가로 구분선 그리기
	gotoCursor(1, 18);	for (int i = 0; i < 92; i++) printf("─");
	// 맨 아랫줄 그리기
	gotoCursor(0, 30);	printf("└");	for (int i = 0; i < 92; i++) printf("─");	printf("┘");
	// 글자 부분 그리기
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Control::control.cleared ? 0x0B : 0x0E);
	gotoCursor(3, 10);	printf("######                                        ######");
	gotoCursor(3, 11);	printf("#     # #    # ###### ###### #      ######    #     #  ####  #####  #####  #      ######");
	gotoCursor(3, 12);	printf("#     # #    #     #      #  #      #         #     # #    # #    # #    # #      #");
	gotoCursor(3, 13);	printf("######  #    #    #      #   #      #####     ######  #    # #####  #####  #      #####");
	gotoCursor(3, 14);	printf("#       #    #   #      #    #      #         #     # #    # #    # #    # #      #");
	gotoCursor(3, 15);	printf("#       #    #  #      #     #      #         #     # #    # #    # #    # #      #");
	gotoCursor(3, 16);	printf("#        ####  ###### ###### ###### ######    ######   ####  #####  #####  ###### ######");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
	gotoCursor(35, 20);	printf("< Instructions >");
	gotoCursor(10, 22);	printf("←   →   :   Rotate 5 degrees");
	gotoCursor(10, 23);	printf("<    >    :   Rotate 1 degree");
	gotoCursor(10, 24);	printf("L         :   Laser");
	// SetConsoleTextAttribute 함수의 두번째 파라미터에서 텍스트의 배경색과 글자색을 정한다
	// 16진수 숫자의 LSB가 글자색, 두번째 bit이 배경색에 해당한다. 사용 후에는 원래대로 되돌려 놓아야 한다.
	gotoCursor(45, 24);	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Control::control.laser ? 0xA0 : 0x07);	printf("[ ON ]");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);	printf(" / ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Control::control.laser ? 0x07 : 0xC0);	printf("[ OFF ]");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);

	gotoCursor(10, 25);	printf("D         :   Dark Mode");
	gotoCursor(45, 25);	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Control::control.dark ? 0xA0 : 0x07);	printf("[ ON ]");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);	printf(" / ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Control::control.dark ? 0x07 : 0xC0);	printf("[ OFF ]");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);

	gotoCursor(10, 26);	printf("S         :   Background Music");
	gotoCursor(45, 26);	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Control::control.muted ? 0x07 : 0xA0);	printf("[ ON ]");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);	printf(" / ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Control::control.muted ? 0xC0 : 0x07);	printf("[ OFF ]");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);

	gotoCursor(10, 27);	printf("SPACE     :   Shoot");
	gotoCursor(30, 29);	printf("모든 공을 없애면 클리어!!");
	// 마지막에 커서를 맨 아랫줄로 이동
	gotoCursor(0, 31);

	//cout << "│  ######                                        ######                                      │" << endl;
	//cout << "│  #     # #    # ###### ###### #      ######    #     #  ####  #####  #####  #      ######  │" << endl;
	//cout << "│  #     # #    #     #      #  #      #         #     # #    # #    # #    # #      #       │" << endl;
	//cout << "│  ######  #    #    #      #   #      #####     ######  #    # #####  #####  #      #####   │" << endl;
	//cout << "│  #       #    #   #      #    #      #         #     # #    # #    # #    # #      #       │" << endl;
	//cout << "│  #       #    #  #      #     #      #         #     # #    # #    # #    # #      #       │" << endl;
	//cout << "│  #        ####  ###### ###### ###### ######    ######   ####  #####  #####  ###### ######  │" << endl;
	//cout << "│                                                                                            │" << endl;
	//cout << "│────────────────────────────────────────────────────────────────────────────────────────────│" << endl;
	//cout << "│                                                                                            │" << endl;
	//cout << "│                                  < Instructions >                                          │" << endl;
	//cout << "│                                                                                            │" << endl;
	//cout << "│         ←   →   :   Rotate 5 degrees                                                     │" << endl;
	//cout << "│         <    >    :   Rotate 1 degree                                                      │" << endl;
	//cout << "│         L         :   Laser              ON / OFF                                          │" << endl;
	//cout << "│         D         :   Dark Mode          ON / OFF                                          │" << endl;
	//cout << "│         S         :   Background Music   ON / OFF                                          │" << endl;
	//cout << "│         SPACE     :   Shoot                                                                │" << endl;
	//cout << "│                                                                                            │" << endl;
	//cout << "│                       모든 공을 없애면 클리어!!                                            │" << endl;
	//cout << "│                                                                                            │" << endl;
	//cout << "└────────────────────────────────────────────────────────────────────────────────────────────┘" << endl;
}
// 콘솔창에서 커서의 위치를 (x, y)로 이동시켜 준다.
void gotoCursor(int x, int y) {
	COORD Pos;	Pos.X = x;	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

// 랜덤으로 보드를 초기화할 경우 사용하는 함수
// 처음 게임이 시작하였을 때 공 배치.
void makeBoard() {
	// 확실한 랜덤을 위해 난수 생성
	srand((unsigned int)time(0));
	// 4줄만 생성
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 10 - (i % 2); j++) {
			board_balls[i][j].setProperties(RADIUS, 50, 50);
			board_balls[i][j].setExist(true);
			board_balls[i][j].setMTL((Color)(rand() % 4 + 1));
			board_balls[i][j].setVel(0, 0);
			board_balls[i][j].setPos(-boundaryX + RADIUS + (i % 2) * RADIUS + 40 * j, boundaryY - 100 - RADIUS - RADIUS * sqrt(3) * i);
		}
	}
	gotoCursor(0, 5);
	cout << "## Board initialization complete! ##" << endl;
	cout << "   Start as Clear Mode." << endl << endl;
}

// 파일 입력으로 보드를 초기화할 경우 사용하는 함수
// 11.25 for load data
void loadData(const char* filename) {
	ifstream data;
	data.open(filename);

	if (data.fail()) {
		gotoCursor(0, 5);
		cout << "Could not find initial file: " << filename << endl;
		cout << "Start as simple mode." << endl << endl;
		Control::control.clearMode = false;
		return;
	}

	while (!data.eof()) {
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 10 - (i % 2); j++) {
				int k = 0;
				data >> k;
				if (k >= 1 && k <= 4) {
					board_balls[i][j].setProperties(RADIUS, 50, 50);
					board_balls[i][j].setExist(true);
					board_balls[i][j].setMTL((Color)k);
					board_balls[i][j].setVel(0, 0);
					board_balls[i][j].setPos(-boundaryX+RADIUS + (i % 2) * RADIUS + 40 * j, boundaryY-100-RADIUS - RADIUS * sqrt(3) * i);
				}
			}
		}
		gotoCursor(0, 5);
		cout << "## Board loading complete! ##" << endl;
		cout << "   Start as Clear Mode." << endl << endl;
	}
	data.close();
}

/* Next에 새 공을 추가하는 부분 */
// 11.22 같은 색이 세번 연속으로 나오는것 방지
// tempcol 변수에 발사된 공 색을 저장하고, 다음 공과 다다음 공 색이 tempcol과 같으면
// 새 공의 색은 tempcol에 1을 더해서 저장한다.
// 이때 숫자가 4를 넘지 않도록 적절히 조절한다.
void nextBall() {
	
	Color tempcol = ball_not_launched[0].getColor();

	ball_not_launched.erase(ball_not_launched.begin()); // 앞부분 지움
	ball_not_launched[0].setPos(0, -260); // [0]이 된 next의 position 재지정
	Ball ball(RADIUS, 50, 50);
	ball.setPos(-125, -260);
	ball.setMTL((Color)(rand() % 4 + 1));
	// 이 부분에서 tempcol+1을 한 다음, 값이 4를 넘으면 1로 바꾸고, 아니면 그대로 쓴다.
	if (tempcol == ball_not_launched[0].getColor() && tempcol == ball.getColor()) {
		ball.setMTL((tempcol + 1 > 4) ? (Color)(tempcol - 3) : (Color)(tempcol + 1));
	}
	ball_not_launched.push_back(ball);
	ball_not_launched[1].setVel(0, 0); // next ball 초기화

}

// i, j의 index를 받아 recursion으로 검사한다. recursion 검사의 대상이 된 공은 제거해야 할 수도 있으므로 visited와 tobedeleted 변수가 true 가 된다.
void removeCheck(int i, int j) {
	int col;
	// 11.18 by 지수
	board_balls[i][j].setVisited(true);
	board_balls[i][j].setToBeDeleted(true);

	// 특정 공의 주변 6개 공을 검사하기 위한 수식
	for (int row = -1; row <= 1; row++) {
		for (int col2 = -1; col2 < 1; col2++) {
			col = col2;
			if (i % 2 == 1 && col == -1 && row != 0)
				col = 1;
			else if (row == 0 && col == 0)
				col = 1;
			// 11.18 by 지수
			// [홀수][8] 위치에서 row==0, col==1 이면 범위를 벗어난다. 아래에 있는 문제때문에 추가하게 되었다.
			// 아래에서 i가 홀수일때 if문 속 조건 중 (j+col)<=8 을 9로 바꿨기 때문
			else if (i % 2 == 1 && j == 8 && row == 0 && col == 1)
				break;

			// 11.18 by 지수
			// 코드에 문제가 있다. 예를 들어, [i][j]==[1][8] 이고, row==-1, col==1 인 경우를 생각해보자.
			// [0][9]가 되므로 정상적인 접근인데, if 조건에 걸리게 된다. 그래서 (j+col)의 범위를 수정해야 한다.

			// 범위 내에서 검사 하는데, 색이 같고 아직 검사하지 않은 경우 recursion, 검사하지 않은 경우인데 색이 다르면 visited만 true로 한다.
			// 짝수
			if (i % 2 == 0) {
				if ((i + row) >= 0 && (i + row) <= 10 && (j + col) >= 0 && (j + col) <= 9) {
					if (!((row == 1) && (col == 1)) && board_balls[i + row][j + col].isExist() && board_balls[i][j].getColor() == board_balls[i + row][j + col].getColor() && board_balls[i + row][j + col].isVisited() == false)
						removeCheck(i + row, j + col);
					else if (board_balls[i + row][j + col].isExist() && board_balls[i][j].getColor() != board_balls[i + row][j + col].getColor() && board_balls[i + row][j + col].isVisited() == false)
						board_balls[i + row][j + col].setVisited(true);
				}
			}
			// 홀수
			else {
				if ((i + row) >= 0 && (i + row) <= 10 && (j + col) >= 0 && (j + col) <= 9) {
					if (board_balls[i + row][j + col].isExist() && board_balls[i][j].getColor() == board_balls[i + row][j + col].getColor() && board_balls[i + row][j + col].isVisited() == false)
						removeCheck(i + row, j + col);
					else if (board_balls[i + row][j + col].isExist() && board_balls[i][j].getColor() != board_balls[i + row][j + col].getColor() && board_balls[i + row][j + col].isVisited() == false)
						board_balls[i + row][j + col].setVisited(true);
				}
			}
		}
	}
}

// remove로 공을 제거 후 drop을 검사하는 함수.
void dropCheck(int i, int j) {
	int col;
	board_balls[i][j].setDropMark(false);
	for (int row = -1; row <= 1; row++) {
		for (int col2 = -1; col2 < 1; col2++) {
			col = col2;
			if (i % 2 == 1 && col == -1 && row != 0)
				col = 1;
			else if (row == 0 && col == 0)
				col = 1;
			// 11.18 by 지수
			// [홀수][8] 위치에서 row==0, col==1 이면 범위를 벗어난다. 아래에 있는 문제때문에 추가하게 되었다.
			// 아래에서 i가 홀수일때 if문 속 조건 중 (j+col)<=8 을 9로 바꿨기 때문
			else if (i % 2 == 1 && j == 8 && row == 0 && col == 1)
				break;

			// 짝수
			// 11.21 (i+row)<=10 에서 (i+row)<=11 로 바꿨다. 11번째 인덱스 관련해서 수정하면서 이 부분 때문에 공이 '흘러가는' 현상이 발생하여 수정함.
			if (i % 2 == 0) {
				if ((i + row) >= 0 && (i + row) <= 11 && (j + col) >= 0 && (j + col) <= 9) {
					if (!((row == 1) && (col == 1)) && board_balls[i + row][j + col].isExist() && board_balls[i + row][j + col].getDropMark() == true)
						dropCheck(i + row, j + col);
				}
			}
			// 홀수
			else {
				if ((i + row) >= 0 && (i + row) <= 10 && (j + col) >= 0 && (j + col) <= 9) {
					if (board_balls[i + row][j + col].isExist() && board_balls[i + row][j + col].getDropMark() == true)
						dropCheck(i + row, j + col);
				}
			}
		}
	}
}

// board_balls가 비어있으면 true를 반환, 아니면 false. 첫 줄에 공이 없을 경우 밑줄에도 당연히 없으므로 맨 윗 줄만 검사한다.
bool isBoardEmpty() {
	for (int j = 0; j < 10; j++) {
		if (board_balls[0][j].isExist())
			return false;
	}
	return true;
}

void init() {
	// 처음 init 함수를 실행할 때는 이 변수가 true이고 그 다음부터는 false이다.
	static bool first = true;

	// for sound
	if (engine != NULL) {
		// 처음 init 함수를 실행할 때만
		if (first) {
			opening = engine->play2D("../Sounds/Opening.mp3", true, true);
			maintheme = engine->play2D("../Sounds/maintheme.mp3", true, true);
			ending = engine->play2D("../Sounds/ending.mp3", true, true);
			cleartheme = engine->play2D("../Sounds/gameclear.mp3", true, true);

			first = false;
		}

		engine->setSoundVolume(0.1);
		opening->setPlayPosition(300);
		maintheme->setPlayPosition(100);
		ending->setPlayPosition(300);
		cleartheme->setPlayPosition(100);
	}

	// Texture 코드. 클래스는 Texture.h 에 있다
	// 11.23 Texture 클래스를 만들고 하나의 Texture 객체에 하나의 이미지가 대응되도록 하였다. 이게 객체지향에 더 어울리는 것 같다.
	// 11.25 게임 클리어 텍스쳐 추가
	Texture* background_1 = new Texture("../Background Image/background_1.png");
	tex.push_back(background_1);

	Texture* background_2 = new Texture("../Background Image/background_2.png");
	tex.push_back(background_2);

	Texture* start = new Texture("../Background Image/start.png");
	tex.push_back(start);

	Texture* over = new Texture("../Background Image/gameover.png");
	tex.push_back(over);

	Texture* clear = new Texture("../Background Image/gameclear.png");
	tex.push_back(clear);

	//공이 잡아먹는 길이는 40(기본 2줄) + 20루트3*10 (근데 총 11줄임)
	// 11.21 마지막줄에서 3개 붙였을 때 게임오버되는 문제를 해결하기 위해 한줄 더 추가

	// board_balls 각 row마다 Board_Ball array를 동적할당 한다
	for (int i = 0; i < 12; i++){
		if (i % 2 == 0) {
			Board_Ball* BB_array_10 = new Board_Ball[10];
			board_balls.push_back(BB_array_10);
		}
		else {
			Board_Ball* BB_array_9 = new Board_Ball[9];
			board_balls.push_back(BB_array_9);
		}
	}

	// 11.25 for load data
	loadData("board.dat");

	// 11.25 random board
	// makeBoard();

	instruction(); // 콘솔창에 게임 동작 key 등을 설명하는 함수
	
	light0 = new Light(6, 6, 8, GL_LIGHT0);
	light0->setAmbient(0.8, 0.8, 0.8, 1.0);
	light0->setDiffuse(1.0, 1.0, 1.0, 1.0);
	light0->setSpecular(0.7, 0.7, 0.7, 1.0);

	light1 = new Light(-8, -8, 10, GL_LIGHT1);
	light1->setAmbient(0.3, 0.3, 0.3, 1.0);
	light1->setDiffuse(0.2, 0.2, 0.2, 1.0);
	light1->setSpecular(0.4, 0.4, 0.4, 1.0);

	arrow.setMTL();
	srand(static_cast<unsigned int>(time(NULL)));
	
	//맨 처음 공.
	for (int i = 0; i < 2; i++) {
		Ball ball(RADIUS, 50, 50);
		ball.setPos(-125*i, -260);
		ball.setMTL((Color)(rand() % 4 + 1));
		//ball.setMTL(1); // for test
		ball_not_launched.push_back(ball);
		//next를 배치하기 위해 2개를 만들었음
	}
}

void draw_characters(void* font, const char* c, float x, float y) {
	glColor3f(1.0, 0.0, 0.0);
	glDisable(GL_LIGHTING);
	glRasterPos2f(x, y);
	for (size_t i = 0; i < strlen(c); i++)
		glutBitmapCharacter(font, c[i]);
	glEnable(GL_LIGHTING);
}

void idle() {
  	end_clock = clock();
	if (end_clock - start_clock > (float)1000 / FPS) {
		// Measure speed
		double currentTime = clock() / 1000.0;
		nbFrames++;
		if (currentTime - lastTime >= 1) { // If last prinf() was more than 1 sec ago
			// printf and reset timer
			//printf("%lf frame/s\n", double(nbFrames));
			currentFrames = nbFrames;
			nbFrames = 0;
			lastTime += 1.0;
		}

		if (Control::control.startgame == true && Control::control.cleared == false && Control::control.gameover == false) {
			// 공이 발사되어 이동중인 경우
			if (Control::control.launch == true) {
				ball_not_launched[0].collisionHandling(); 
				if (isBoardEmpty()==false) {
					for (int i = 0; i < 11; i++) {
						bool row_exist = false; // 어떤 row에 공이 없을 경우 그 아랫 줄도 없을것이므로 그 밑줄부터는 check를 생략한다.
						for (int j = 0; j < 10 -(i%2); j++) {
							row_exist = row_exist || board_balls[i][j].isExist();
							if (board_balls[i][j].isExist())
								ball_not_launched[0].collisionHandling(board_balls[i][j]);
						}
						if (row_exist == false) break;
					}
				}
				ball_not_launched[0].move();
				//공이 배치를 완료하고 멈췄을 경우
				if (ball_not_launched[0].getVel()[0] == 0 && ball_not_launched[0].getVel()[1] == 0) {

					//공의 좌표에 따른 index 수식
					int i = floor(((double)(boundaryX - RADIUS) - (double)ball_not_launched[0].getPos()[1]) / (RADIUS * sqrt(3)) + 0.5);
					int j = floor((ball_not_launched[0].getPos()[0] + boundaryY - 100 - RADIUS - (i % 2) * RADIUS) / 40 + 0.5);

					// 발사한 공이 배치되었으므로 board_balls에 알맞은 위치에 넣어준다.
					Board_Ball newball(ball_not_launched[0]);
					board_balls[i][j] = newball;
					board_balls[i][j].setExist(true);

					int count = 0;
					// 공 배치 완료 후 제거여부를 판단하는 함수.
					removeCheck(i, j);
					for (int k = 0; k < 12; k++) {
						bool row_exist = false;
						for (int l = 0; l < 10 - (k % 2); l++) {
							row_exist = row_exist || board_balls[k][l].isExist();
							if (board_balls[k][l].isExist() && board_balls[k][l].istoBeDeleted()) {
								count++;
							}
						}
						if (row_exist == false) break;
					}

					// 같은 공이 연속으로 3개 이상 붙어있을 경우 제거하고, 아닐 경우 remove에 대한 변수들을 다시 reset 해준다.
					if (count >= 3) {
						for (int k = 0; k < 12; k++) {
							bool row_exist = false;
							for (int l = 0; l < 10 - (k % 2); l++) {
								row_exist = row_exist || board_balls[k][l].isExist();
								if (board_balls[k][l].istoBeDeleted() && board_balls[k][l].isExist()) {
									board_balls[k][l].setExist(false);
									Control::control.score += 10;
								}
								if (board_balls[k][l].isExist())
									board_balls[k][l].reset();
							}
							if (row_exist == false) break;
						}
						// for sound
						if (engine != NULL) {
							ISound* explosion = engine->play2D("../Sounds/explosion.wav", false, true);
							explosion->setVolume(4.0);
							explosion->setIsPaused(false);
							explosion->drop();
						}
					}
					else {
						for (int k = 0; k < 12; k++) {
							bool row_exist = false;
							for (int l = 0; l < 10 - (k % 2); l++) {
								row_exist = row_exist || board_balls[k][l].isExist();
								if (board_balls[k][l].isExist())
									board_balls[k][l].reset();
							}
							if (row_exist == false) break;
						}

					}

					// 맨 위 줄부터 dropcheck를 시작한다. drop의 대상이 되는 공들은 dropmark가 찍힌다.
					for (int l = 0; l < 10; l++)
						if (board_balls[0][l].isExist())
							dropCheck(0, l);

					// dropmark 변수가 true인 것들을 찾아 dropball vector에 넣어준다.
					for (int k = 0; k < 12; k++) {
						for (int l = 0; l < 10 - (k % 2); l++) {
							if (board_balls[k][l].isExist() && (board_balls[k][l].getDropMark())) {
								dropball.push_back(board_balls[k][l]);
								board_balls[k][l].setExist(false);	
							}
							board_balls[k][l].setDropMark(true);
						}
					}
					// drop대신 위로 올라가는 방식으로 제거. 여기서 속도를 정한다. FPS 앞 숫자에 따라 결정된다.
					for (vector<Ball>::size_type i = 0; i < dropball.size(); i++) {
						dropball[i].setVel(5.0 / FPS * (double)dropball[i].getPos()[0], 6.0 / FPS * ((double)dropball[i].getPos()[1] + 200));
					}

					nextBall(); // next에 있는 공을 발사대에 놓고, next에 새로운 공을 추가한다.
				}
			}

			/* 여기서부터 if(launched == true) 바깥부분 */

			// drop된 공을 움직이고, 점수를 추가한다.
			for (vector<Ball>::size_type i = 0; i < dropball.size(); i++) {
				dropball[i].move();
				if (dropball[i].getPos()[1] + RADIUS > boundaryY + 100) {
					dropball.erase(dropball.begin() + i);
					Control::control.score += 20;
				}
			}

			// 11.19 by 지수
			// 원래 이 문장이 idle함수의 가장 처음에 나왔는데, 그러면 스페이스 연타할 때 문제가 생긴다
			
			// Arrow 위 공의 속도를 설정하고, 타임바를 줄인다.
			if (Control::control.launch == false) {
				ball_not_launched[0].setVel((SPEED * cos(arrow.getAngle() * M_PI / 180.0)), SPEED * sin(arrow.getAngle() * M_PI / 180.0));
				timebar.decrease();
				if (timebar.getRemaintime() == 0) {
					Control::control.launch = true;
					timebar.reset();
				}
			}

			// 11.25 Clear Mode에서 클리어 조건 검사. clearmode가 true이고 board_balls가 모두 비어있을 때.
			// drop으로 끝나는 경우 dropball이 모두 사라진 후 종료
			if (Control::control.clearMode == true && isBoardEmpty() && dropball.empty()) {
				Control::control.cleared = true;
				instruction();
			}

			// 11.21 게임오버를 remove와 drop 체크가 다 끝난 뒤에 검사
			// Gameover 조건
			for (int j = 0; j < 9; j++) {
				if (board_balls[11][j].isExist()) {
					Control::control.gameover = true;
					return;
				}
			}
		}

		// 11.26 S키가 눌리면 사운드 ON/OFF
		if (engine != NULL) {
			if (Control::control.muted == true)
				engine->setSoundVolume(0);
			else
				engine->setSoundVolume(0.1);
		}

		start_clock = end_clock;
	}

	glutPostRedisplay();
}

// 키보드 설정
void ControlKey(unsigned char key, int x, int y) 
{
	
	if (key == ENTER) {
		// 게임 오버나 클리어 시에 엔터키를 누르면 처음화면으로
		if (Control::control.gameover == true || Control::control.cleared == true) {
			Control::control.startgame = false;
			Control::control.cleared = false;
			Control::control.gameover = false;
			// 초기화
			for (vector<Board_Ball*>::size_type i = 0; i < board_balls.size(); i++) {
				delete[] board_balls[i];
			}
			for (vector<Texture*>::size_type i = 0; i < tex.size(); i++) {
				delete tex[i];
			}

			// 벡터들 다 비워주기
			board_balls.clear();
			ball_not_launched.clear();
			dropball.clear();
			tex.clear();

			timebar.reset();
			arrow.angleReset();
			Control::control.score = 0;
			delete light0;
			delete light1;
			init();
		}
		// 처음 화면일때 엔터 누르면 게임 시작
		else if (Control::control.startgame == false) {
			Control::control.startgame = true;
		}
	}
	// 11.23 게임 중 ESC키를 누르면 바로 게임이 종료되는 것이 아니라 게임오버 화면으로 넘어가게 함
	// 11.25 클리어때도 ESC키 누르면 종료되도록 수정
	else if (key == ESC) {
		if (Control::control.startgame == false || Control::control.cleared == true || Control::control.gameover == true)
			exit(0);
		else
			Control::control.gameover = true;
	}
	// 11.21 게임이 시작한 이후에만 스페이스바를 인식하게 함
	// 11.24 게임 오버가 된 이후에는 스페이스바 인식을 막음
	// 11.26 게임 클리어 이후에는 스페이스바 인식을 막음
	else if (key == SPACE) {
		if (Control::control.startgame == true && Control::control.cleared == false && Control::control.gameover == false) {
			Control::control.launch = true;
			timebar.reset();
			// for sound
			if(engine != NULL)
				engine->play2D("../Sounds/laser.wav", false);
		}

	}
	// 11.21 레이저를 키보드 L키로 켤 수 있게 함. 마우스에 손대기 귀찮아서...
	else if (key == 'l' || key == 'L') {
		Control::control.laser = !Control::control.laser;
		instruction();
	}
	// 11.23 다크모드를 키보드 D키로 켤 수 있게 함.
	else if (key == 'd' || key == 'D') {
		Control::control.dark = !Control::control.dark;
		instruction();
	}
	// 11.23 <> 키로 세밀한 각도조절 기능 추가, arrow를 1도씩 옮김.
	else if (key == ',') {
		if (arrow.getAngle() >= 10 && arrow.getAngle() < 170)
			arrow.move_left(1);
	}
	else if (key == '.') {
		if (arrow.getAngle() > 10 && arrow.getAngle() <= 170)
			arrow.move_right(1);
	}
	// 11.26 사운드를 키보드 S키로 켜고 끌 수 있게 함.
	else if (key == 's' || key == 'S') {
		Control::control.muted = !Control::control.muted;
		instruction();
	}
}

// 방향키 설정
void SpecialKey(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_RIGHT:
		// 11.23 세밀한 각도조절 기능을 넣었더니 예를 들어 13도인 상황에서 오른쪽으로 5도 움직이면 8도가 되어 더 이상 움직이지 않는 문제가 발생.
		// arrow.getAngle()-5 >= 10 이런 식으로 '움직인 후의 각도' 를 검사하여 해결. 왼쪽키도 마찬가지.
		if (arrow.getAngle()-5 >= 10 && arrow.getAngle() <= 170)
			arrow.move_right(5);
		break;
		
	case GLUT_KEY_LEFT:
		if (arrow.getAngle() >= 10 && arrow.getAngle()+5 <= 170)
			arrow.move_left(5);
		break;

	default:
		break;
	}
	glutPostRedisplay();
}

// 11.19 by 지수
void main_menu_function(int option) {
	if (option == -1)
		exit(0);
	/*else if (option == 777)
		Control::control.GOD_MODE = true;*/
}

void background_menu_function(int option) {
	if (option == 0)
		Control::control.dark = false;
	else if (option == 1)
		Control::control.dark = true;
}

void laser_menu_function(int option) {
	if (option == 0)
		Control::control.laser = false;
	else if (option == 1)
		Control::control.laser = true;
}

// 화면 크기를 바꿨을 때 화면이 왜곡되는 것을 방지하기 위한 함수.
void reshape(int NewWidth, int NewHeight) {
	glViewport(0, 0, NewWidth, NewHeight);

	GLdouble WidthFactor = (GLdouble)NewWidth / (GLdouble)WIDTH;
	GLdouble HeightFactor = (GLdouble)NewHeight / (GLdouble)HEIGHT;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-boundaryX * WidthFactor, boundaryX * WidthFactor,
		-boundaryY * HeightFactor, boundaryY * HeightFactor, -100.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// 좌표계 설정
	// 11.25 reshape함수에서 좌표계 설정하는걸로 변경. 화면 크기가 변해도 게임 화면은 변하지 않음
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glOrtho(-boundaryX, boundaryX, -boundaryY, boundaryY, -100.0, 100.0);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	// 전체 light를 사용 가능하게 한다
	glEnable(GL_LIGHTING);
	// light0의 스위치를 켠다
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	light0->draw();
	light1->draw();
	
	// 깜빡임 효과를 넣기 위해 static 변수 추가
	static int blinking = 0;

	// 게임 진행 중
	// 11.25 Control::control.cleared == false 조건 추가
	if (Control::control.startgame == true && Control::control.gameover == false && Control::control.cleared == false) {
		if (engine != NULL) {
			opening->setIsPaused(true);
			maintheme->setVolume(2.0);
			maintheme->setIsPaused(false);
		}
		// 11.23 displayOn을 dark로 바꿈
		// dark는 기본값이 true이고 마우스 우클릭 메뉴에서 끌 수 있다.
		if (Control::control.dark) {
			// Texture 코드. 함수 내용은 Texture.h 에 있다
			tex[1]->drawBackWithTexturing();
		}
		else {
			tex[0]->drawBackWithTexturing();
		}
		
		// laser는 기본값이 false이고 마우스 우클릭 메뉴에서 끌 수 있다
		if (Control::control.laser) {
			pathlaser.draw(arrow.getAngle());
		}

		// 디버그용 God Mode 구현.
		/*if (Control::control.GOD_MODE) {
			int x, y, color;
			char exit;
			cout << "::::::::::::::GOD MODE::::::::::::::" << endl << endl;
			cout << "좌표 입력 (쉼표로 구분) : ";
			scanf_s("%d, %d", &x, &y);
			cout << "색 입력 (R=1, G=2, S=3, Y=4) : ";
			cin >> color;
			board_balls[x][y].setExist(true);
			board_balls[x][y].setMTL(color);
			board_balls[x][y].setVel(0, 0);
			board_balls[x][y].setPos(-180 + (x % 2) * RADIUS + 40 * y, 180 - RADIUS * sqrt(3) * x);
			board_balls[x][y].setProperties(RADIUS, 50, 50);
			cout << endl << endl;
			cout << "Exit GOD_MODE? (y/n) : ";
			cin >> exit;
			if (exit == 'y') Control::control.GOD_MODE = false;
			cout << endl << endl;
		}*/

		arrow.draw();

		// 배경 이미지에 글자 포함
		char fpschar[15];
		sprintf_s(fpschar, "%d fps", currentFrames);
		draw_characters(GLUT_BITMAP_TIMES_ROMAN_10, fpschar, -200, 290);

		char intchar[5];
		sprintf_s(intchar, "%d", Control::control.score);
		draw_characters(GLUT_BITMAP_TIMES_ROMAN_24, intchar, -148, 230);

		// 발사대 공과 next 공 배치
		for ( vector<Ball>::size_type i = 0; i < ball_not_launched.size(); i++)
			ball_not_launched[i].draw();
		// board_balls에 존재하는 공들 그리기
		for (int i = 0; i < 12; i++) {
			bool row_exist = false;
			for (int j = 0; j < 10 - (i % 2); j++) {
				row_exist = row_exist || board_balls[i][j].isExist();
				if (board_balls[i][j].isExist())
					board_balls[i][j].draw();
			}
			if (row_exist == false) break;
		}
		// dropball 날라가는 장면 그리기
		for (vector<Ball>::size_type i = 0; i < dropball.size(); i++)
			dropball[i].draw();
		
		timebar.draw();
	}
	// 게임 클리어
	if (Control::control.startgame == true && Control::control.cleared == true) {
		if (engine != NULL) {
			maintheme->setIsPaused(true);
			cleartheme->setVolume(2.5);
			cleartheme->setIsPaused(false);
		}
		tex[4]->drawBackWithTexturing();

		if (blinking % 480 < 240) {
			draw_characters(GLUT_BITMAP_TIMES_ROMAN_24, "Clear!!! Congratulations", -110, -100);
			draw_characters(GLUT_BITMAP_TIMES_ROMAN_24, "Press ESC to exit", -80, -150);
			draw_characters(GLUT_BITMAP_TIMES_ROMAN_24, "Press ENTER to restart", -107, -200);
			blinking++;
		}
		else {
			blinking++;
		}
		blinking = (blinking == 480) ? 0 : blinking;
		
	}
	// 게임 오버
	if (Control::control.startgame == true && Control::control.gameover == true) {
		if (engine != NULL) {
			maintheme->setIsPaused(true);
			ending->setVolume(3.0);
			ending->setIsPaused(false);
		}
		tex[3]->drawBackWithTexturing();

		if (blinking % 480 < 240) {
			draw_characters(GLUT_BITMAP_TIMES_ROMAN_24, "Press ESC to exit", -80, -100);
			draw_characters(GLUT_BITMAP_TIMES_ROMAN_24, "Press ENTER to restart", -110, -150);
			blinking++;
		}
		else {
			blinking++;
		}
		blinking = (blinking == 480) ? 0 : blinking;
	}
	// 게임 시작 전 화면
	if (Control::control.startgame == false) {
		if (engine != NULL) {
			maintheme->setIsPaused(true);
			ending->setIsPaused(true);
			cleartheme->setIsPaused(true);
			opening->setVolume(3.0);
			opening->setIsPaused(false);
		}
		tex[2]->drawBackWithTexturing();

		if (blinking % 480 < 240) {
			draw_characters(GLUT_BITMAP_TIMES_ROMAN_24, "Press ENTER to start", -100, -100);
			draw_characters(GLUT_BITMAP_TIMES_ROMAN_24, "Press ESC to exit", -80, -150);
			blinking++;
		}
		else {
			blinking++;
		}
		blinking = (blinking == 480) ? 0 : blinking;
	}
	
 	glutSwapBuffers();
}

// 프로그램 종료시 메모리 해제
void exitProgram() {
	//cout << "delete" << endl;
	for (vector<Board_Ball*>::size_type i = 0; i < board_balls.size(); i++)
		delete[] board_balls[i];
	for (vector<Texture*>::size_type i = 0; i < tex.size(); i++) {
		delete tex[i];
	}
	delete light0;
	delete light1;
}


int main(int argc, char** argv) {
	
	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(900, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Puzzle Bobble");
	init();

	// 11.19 by 지수
	// Popup menu
	int mainmenu, background_submenu, laser_submenu;
	background_submenu = glutCreateMenu(background_menu_function);
	glutAddMenuEntry("OFF", 0);
	glutAddMenuEntry("ON", 1);
	laser_submenu = glutCreateMenu(laser_menu_function);
	glutAddMenuEntry("OFF", 0);
	glutAddMenuEntry("ON", 1);
	mainmenu = glutCreateMenu(main_menu_function);
	glutAddSubMenu("Dark Mode", background_submenu);
	glutAddSubMenu("Laser", laser_submenu);
	// glutAddMenuEntry("GOD_MODE", 777);
	glutAddMenuEntry("Exit", -1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	
	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutKeyboardFunc(ControlKey);	// 키보드 설정
	glutSpecialFunc(SpecialKey); // 방향키
	// enter GLUT event processing cycle
	atexit(exitProgram);
	glutMainLoop();

	return 0;
}

