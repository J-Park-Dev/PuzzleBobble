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

// 11.22 �� �ϳ� �� �߰�
Light* light0;
Light* light1;

Arrow arrow;
TimeBar timebar;
PathLaser pathlaser;

// �߻� ���� ���� next ���� ������ ����
vector<Ball> ball_not_launched;
// ���忡 �����ϴ� ��� ���� ����, exist ������ ���� ���� �Ǻ�
vector<Board_Ball*> board_balls;
// drop ��ų ���� ����
vector<Ball> dropball;

// 11.23 Texture vector ����
vector<Texture*> tex;

// for sound
// ���� ��ġ�� ������ engine == NULL
ISoundEngine* engine = createIrrKlangDevice();
ISound* opening;
ISound* maintheme;
ISound* ending;
ISound* cleartheme;

void gotoCursor(int x, int y);
void instruction() {
	// cout << "������������" << endl;
	// �� ���� �׸���
	gotoCursor(0, 8); printf("��");	for (int i = 0; i < 92; i++) printf("��");	printf("��\n");
	// �¿� ������ �׸���
	for (int i = 0; i < 22; i++) {
		printf("��");	gotoCursor(93, i + 9);	printf("��\n");
	}
	// �߰� ���� ���м� �׸���
	gotoCursor(1, 18);	for (int i = 0; i < 92; i++) printf("��");
	// �� �Ʒ��� �׸���
	gotoCursor(0, 30);	printf("��");	for (int i = 0; i < 92; i++) printf("��");	printf("��");
	// ���� �κ� �׸���
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
	gotoCursor(10, 22);	printf("��   ��   :   Rotate 5 degrees");
	gotoCursor(10, 23);	printf("<    >    :   Rotate 1 degree");
	gotoCursor(10, 24);	printf("L         :   Laser");
	// SetConsoleTextAttribute �Լ��� �ι�° �Ķ���Ϳ��� �ؽ�Ʈ�� ������ ���ڻ��� ���Ѵ�
	// 16���� ������ LSB�� ���ڻ�, �ι�° bit�� ������ �ش��Ѵ�. ��� �Ŀ��� ������� �ǵ��� ���ƾ� �Ѵ�.
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
	gotoCursor(30, 29);	printf("��� ���� ���ָ� Ŭ����!!");
	// �������� Ŀ���� �� �Ʒ��ٷ� �̵�
	gotoCursor(0, 31);

	//cout << "��  ######                                        ######                                      ��" << endl;
	//cout << "��  #     # #    # ###### ###### #      ######    #     #  ####  #####  #####  #      ######  ��" << endl;
	//cout << "��  #     # #    #     #      #  #      #         #     # #    # #    # #    # #      #       ��" << endl;
	//cout << "��  ######  #    #    #      #   #      #####     ######  #    # #####  #####  #      #####   ��" << endl;
	//cout << "��  #       #    #   #      #    #      #         #     # #    # #    # #    # #      #       ��" << endl;
	//cout << "��  #       #    #  #      #     #      #         #     # #    # #    # #    # #      #       ��" << endl;
	//cout << "��  #        ####  ###### ###### ###### ######    ######   ####  #####  #####  ###### ######  ��" << endl;
	//cout << "��                                                                                            ��" << endl;
	//cout << "��������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������" << endl;
	//cout << "��                                                                                            ��" << endl;
	//cout << "��                                  < Instructions >                                          ��" << endl;
	//cout << "��                                                                                            ��" << endl;
	//cout << "��         ��   ��   :   Rotate 5 degrees                                                     ��" << endl;
	//cout << "��         <    >    :   Rotate 1 degree                                                      ��" << endl;
	//cout << "��         L         :   Laser              ON / OFF                                          ��" << endl;
	//cout << "��         D         :   Dark Mode          ON / OFF                                          ��" << endl;
	//cout << "��         S         :   Background Music   ON / OFF                                          ��" << endl;
	//cout << "��         SPACE     :   Shoot                                                                ��" << endl;
	//cout << "��                                                                                            ��" << endl;
	//cout << "��                       ��� ���� ���ָ� Ŭ����!!                                            ��" << endl;
	//cout << "��                                                                                            ��" << endl;
	//cout << "��������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������" << endl;
}
// �ܼ�â���� Ŀ���� ��ġ�� (x, y)�� �̵����� �ش�.
void gotoCursor(int x, int y) {
	COORD Pos;	Pos.X = x;	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

// �������� ���带 �ʱ�ȭ�� ��� ����ϴ� �Լ�
// ó�� ������ �����Ͽ��� �� �� ��ġ.
void makeBoard() {
	// Ȯ���� ������ ���� ���� ����
	srand((unsigned int)time(0));
	// 4�ٸ� ����
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

// ���� �Է����� ���带 �ʱ�ȭ�� ��� ����ϴ� �Լ�
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

/* Next�� �� ���� �߰��ϴ� �κ� */
// 11.22 ���� ���� ���� �������� �����°� ����
// tempcol ������ �߻�� �� ���� �����ϰ�, ���� ���� �ٴ��� �� ���� tempcol�� ������
// �� ���� ���� tempcol�� 1�� ���ؼ� �����Ѵ�.
// �̶� ���ڰ� 4�� ���� �ʵ��� ������ �����Ѵ�.
void nextBall() {
	
	Color tempcol = ball_not_launched[0].getColor();

	ball_not_launched.erase(ball_not_launched.begin()); // �պκ� ����
	ball_not_launched[0].setPos(0, -260); // [0]�� �� next�� position ������
	Ball ball(RADIUS, 50, 50);
	ball.setPos(-125, -260);
	ball.setMTL((Color)(rand() % 4 + 1));
	// �� �κп��� tempcol+1�� �� ����, ���� 4�� ������ 1�� �ٲٰ�, �ƴϸ� �״�� ����.
	if (tempcol == ball_not_launched[0].getColor() && tempcol == ball.getColor()) {
		ball.setMTL((tempcol + 1 > 4) ? (Color)(tempcol - 3) : (Color)(tempcol + 1));
	}
	ball_not_launched.push_back(ball);
	ball_not_launched[1].setVel(0, 0); // next ball �ʱ�ȭ

}

// i, j�� index�� �޾� recursion���� �˻��Ѵ�. recursion �˻��� ����� �� ���� �����ؾ� �� ���� �����Ƿ� visited�� tobedeleted ������ true �� �ȴ�.
void removeCheck(int i, int j) {
	int col;
	// 11.18 by ����
	board_balls[i][j].setVisited(true);
	board_balls[i][j].setToBeDeleted(true);

	// Ư�� ���� �ֺ� 6�� ���� �˻��ϱ� ���� ����
	for (int row = -1; row <= 1; row++) {
		for (int col2 = -1; col2 < 1; col2++) {
			col = col2;
			if (i % 2 == 1 && col == -1 && row != 0)
				col = 1;
			else if (row == 0 && col == 0)
				col = 1;
			// 11.18 by ����
			// [Ȧ��][8] ��ġ���� row==0, col==1 �̸� ������ �����. �Ʒ��� �ִ� ���������� �߰��ϰ� �Ǿ���.
			// �Ʒ����� i�� Ȧ���϶� if�� �� ���� �� (j+col)<=8 �� 9�� �ٲ�� ����
			else if (i % 2 == 1 && j == 8 && row == 0 && col == 1)
				break;

			// 11.18 by ����
			// �ڵ忡 ������ �ִ�. ���� ���, [i][j]==[1][8] �̰�, row==-1, col==1 �� ��츦 �����غ���.
			// [0][9]�� �ǹǷ� �������� �����ε�, if ���ǿ� �ɸ��� �ȴ�. �׷��� (j+col)�� ������ �����ؾ� �Ѵ�.

			// ���� ������ �˻� �ϴµ�, ���� ���� ���� �˻����� ���� ��� recursion, �˻����� ���� ����ε� ���� �ٸ��� visited�� true�� �Ѵ�.
			// ¦��
			if (i % 2 == 0) {
				if ((i + row) >= 0 && (i + row) <= 10 && (j + col) >= 0 && (j + col) <= 9) {
					if (!((row == 1) && (col == 1)) && board_balls[i + row][j + col].isExist() && board_balls[i][j].getColor() == board_balls[i + row][j + col].getColor() && board_balls[i + row][j + col].isVisited() == false)
						removeCheck(i + row, j + col);
					else if (board_balls[i + row][j + col].isExist() && board_balls[i][j].getColor() != board_balls[i + row][j + col].getColor() && board_balls[i + row][j + col].isVisited() == false)
						board_balls[i + row][j + col].setVisited(true);
				}
			}
			// Ȧ��
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

// remove�� ���� ���� �� drop�� �˻��ϴ� �Լ�.
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
			// 11.18 by ����
			// [Ȧ��][8] ��ġ���� row==0, col==1 �̸� ������ �����. �Ʒ��� �ִ� ���������� �߰��ϰ� �Ǿ���.
			// �Ʒ����� i�� Ȧ���϶� if�� �� ���� �� (j+col)<=8 �� 9�� �ٲ�� ����
			else if (i % 2 == 1 && j == 8 && row == 0 && col == 1)
				break;

			// ¦��
			// 11.21 (i+row)<=10 ���� (i+row)<=11 �� �ٲ��. 11��° �ε��� �����ؼ� �����ϸ鼭 �� �κ� ������ ���� '�귯����' ������ �߻��Ͽ� ������.
			if (i % 2 == 0) {
				if ((i + row) >= 0 && (i + row) <= 11 && (j + col) >= 0 && (j + col) <= 9) {
					if (!((row == 1) && (col == 1)) && board_balls[i + row][j + col].isExist() && board_balls[i + row][j + col].getDropMark() == true)
						dropCheck(i + row, j + col);
				}
			}
			// Ȧ��
			else {
				if ((i + row) >= 0 && (i + row) <= 10 && (j + col) >= 0 && (j + col) <= 9) {
					if (board_balls[i + row][j + col].isExist() && board_balls[i + row][j + col].getDropMark() == true)
						dropCheck(i + row, j + col);
				}
			}
		}
	}
}

// board_balls�� ��������� true�� ��ȯ, �ƴϸ� false. ù �ٿ� ���� ���� ��� ���ٿ��� �翬�� �����Ƿ� �� �� �ٸ� �˻��Ѵ�.
bool isBoardEmpty() {
	for (int j = 0; j < 10; j++) {
		if (board_balls[0][j].isExist())
			return false;
	}
	return true;
}

void init() {
	// ó�� init �Լ��� ������ ���� �� ������ true�̰� �� �������ʹ� false�̴�.
	static bool first = true;

	// for sound
	if (engine != NULL) {
		// ó�� init �Լ��� ������ ����
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

	// Texture �ڵ�. Ŭ������ Texture.h �� �ִ�
	// 11.23 Texture Ŭ������ ����� �ϳ��� Texture ��ü�� �ϳ��� �̹����� �����ǵ��� �Ͽ���. �̰� ��ü���⿡ �� ��︮�� �� ����.
	// 11.25 ���� Ŭ���� �ؽ��� �߰�
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

	//���� ��ƸԴ� ���̴� 40(�⺻ 2��) + 20��Ʈ3*10 (�ٵ� �� 11����)
	// 11.21 �������ٿ��� 3�� �ٿ��� �� ���ӿ����Ǵ� ������ �ذ��ϱ� ���� ���� �� �߰�

	// board_balls �� row���� Board_Ball array�� �����Ҵ� �Ѵ�
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

	instruction(); // �ܼ�â�� ���� ���� key ���� �����ϴ� �Լ�
	
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
	
	//�� ó�� ��.
	for (int i = 0; i < 2; i++) {
		Ball ball(RADIUS, 50, 50);
		ball.setPos(-125*i, -260);
		ball.setMTL((Color)(rand() % 4 + 1));
		//ball.setMTL(1); // for test
		ball_not_launched.push_back(ball);
		//next�� ��ġ�ϱ� ���� 2���� �������
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
			// ���� �߻�Ǿ� �̵����� ���
			if (Control::control.launch == true) {
				ball_not_launched[0].collisionHandling(); 
				if (isBoardEmpty()==false) {
					for (int i = 0; i < 11; i++) {
						bool row_exist = false; // � row�� ���� ���� ��� �� �Ʒ� �ٵ� �������̹Ƿ� �� ���ٺ��ʹ� check�� �����Ѵ�.
						for (int j = 0; j < 10 -(i%2); j++) {
							row_exist = row_exist || board_balls[i][j].isExist();
							if (board_balls[i][j].isExist())
								ball_not_launched[0].collisionHandling(board_balls[i][j]);
						}
						if (row_exist == false) break;
					}
				}
				ball_not_launched[0].move();
				//���� ��ġ�� �Ϸ��ϰ� ������ ���
				if (ball_not_launched[0].getVel()[0] == 0 && ball_not_launched[0].getVel()[1] == 0) {

					//���� ��ǥ�� ���� index ����
					int i = floor(((double)(boundaryX - RADIUS) - (double)ball_not_launched[0].getPos()[1]) / (RADIUS * sqrt(3)) + 0.5);
					int j = floor((ball_not_launched[0].getPos()[0] + boundaryY - 100 - RADIUS - (i % 2) * RADIUS) / 40 + 0.5);

					// �߻��� ���� ��ġ�Ǿ����Ƿ� board_balls�� �˸��� ��ġ�� �־��ش�.
					Board_Ball newball(ball_not_launched[0]);
					board_balls[i][j] = newball;
					board_balls[i][j].setExist(true);

					int count = 0;
					// �� ��ġ �Ϸ� �� ���ſ��θ� �Ǵ��ϴ� �Լ�.
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

					// ���� ���� �������� 3�� �̻� �پ����� ��� �����ϰ�, �ƴ� ��� remove�� ���� �������� �ٽ� reset ���ش�.
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

					// �� �� �ٺ��� dropcheck�� �����Ѵ�. drop�� ����� �Ǵ� ������ dropmark�� ������.
					for (int l = 0; l < 10; l++)
						if (board_balls[0][l].isExist())
							dropCheck(0, l);

					// dropmark ������ true�� �͵��� ã�� dropball vector�� �־��ش�.
					for (int k = 0; k < 12; k++) {
						for (int l = 0; l < 10 - (k % 2); l++) {
							if (board_balls[k][l].isExist() && (board_balls[k][l].getDropMark())) {
								dropball.push_back(board_balls[k][l]);
								board_balls[k][l].setExist(false);	
							}
							board_balls[k][l].setDropMark(true);
						}
					}
					// drop��� ���� �ö󰡴� ������� ����. ���⼭ �ӵ��� ���Ѵ�. FPS �� ���ڿ� ���� �����ȴ�.
					for (vector<Ball>::size_type i = 0; i < dropball.size(); i++) {
						dropball[i].setVel(5.0 / FPS * (double)dropball[i].getPos()[0], 6.0 / FPS * ((double)dropball[i].getPos()[1] + 200));
					}

					nextBall(); // next�� �ִ� ���� �߻�뿡 ����, next�� ���ο� ���� �߰��Ѵ�.
				}
			}

			/* ���⼭���� if(launched == true) �ٱ��κ� */

			// drop�� ���� �����̰�, ������ �߰��Ѵ�.
			for (vector<Ball>::size_type i = 0; i < dropball.size(); i++) {
				dropball[i].move();
				if (dropball[i].getPos()[1] + RADIUS > boundaryY + 100) {
					dropball.erase(dropball.begin() + i);
					Control::control.score += 20;
				}
			}

			// 11.19 by ����
			// ���� �� ������ idle�Լ��� ���� ó���� ���Դµ�, �׷��� �����̽� ��Ÿ�� �� ������ �����
			
			// Arrow �� ���� �ӵ��� �����ϰ�, Ÿ�ӹٸ� ���δ�.
			if (Control::control.launch == false) {
				ball_not_launched[0].setVel((SPEED * cos(arrow.getAngle() * M_PI / 180.0)), SPEED * sin(arrow.getAngle() * M_PI / 180.0));
				timebar.decrease();
				if (timebar.getRemaintime() == 0) {
					Control::control.launch = true;
					timebar.reset();
				}
			}

			// 11.25 Clear Mode���� Ŭ���� ���� �˻�. clearmode�� true�̰� board_balls�� ��� ������� ��.
			// drop���� ������ ��� dropball�� ��� ����� �� ����
			if (Control::control.clearMode == true && isBoardEmpty() && dropball.empty()) {
				Control::control.cleared = true;
				instruction();
			}

			// 11.21 ���ӿ����� remove�� drop üũ�� �� ���� �ڿ� �˻�
			// Gameover ����
			for (int j = 0; j < 9; j++) {
				if (board_balls[11][j].isExist()) {
					Control::control.gameover = true;
					return;
				}
			}
		}

		// 11.26 SŰ�� ������ ���� ON/OFF
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

// Ű���� ����
void ControlKey(unsigned char key, int x, int y) 
{
	
	if (key == ENTER) {
		// ���� ������ Ŭ���� �ÿ� ����Ű�� ������ ó��ȭ������
		if (Control::control.gameover == true || Control::control.cleared == true) {
			Control::control.startgame = false;
			Control::control.cleared = false;
			Control::control.gameover = false;
			// �ʱ�ȭ
			for (vector<Board_Ball*>::size_type i = 0; i < board_balls.size(); i++) {
				delete[] board_balls[i];
			}
			for (vector<Texture*>::size_type i = 0; i < tex.size(); i++) {
				delete tex[i];
			}

			// ���͵� �� ����ֱ�
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
		// ó�� ȭ���϶� ���� ������ ���� ����
		else if (Control::control.startgame == false) {
			Control::control.startgame = true;
		}
	}
	// 11.23 ���� �� ESCŰ�� ������ �ٷ� ������ ����Ǵ� ���� �ƴ϶� ���ӿ��� ȭ������ �Ѿ�� ��
	// 11.25 Ŭ����� ESCŰ ������ ����ǵ��� ����
	else if (key == ESC) {
		if (Control::control.startgame == false || Control::control.cleared == true || Control::control.gameover == true)
			exit(0);
		else
			Control::control.gameover = true;
	}
	// 11.21 ������ ������ ���Ŀ��� �����̽��ٸ� �ν��ϰ� ��
	// 11.24 ���� ������ �� ���Ŀ��� �����̽��� �ν��� ����
	// 11.26 ���� Ŭ���� ���Ŀ��� �����̽��� �ν��� ����
	else if (key == SPACE) {
		if (Control::control.startgame == true && Control::control.cleared == false && Control::control.gameover == false) {
			Control::control.launch = true;
			timebar.reset();
			// for sound
			if(engine != NULL)
				engine->play2D("../Sounds/laser.wav", false);
		}

	}
	// 11.21 �������� Ű���� LŰ�� �� �� �ְ� ��. ���콺�� �մ�� �����Ƽ�...
	else if (key == 'l' || key == 'L') {
		Control::control.laser = !Control::control.laser;
		instruction();
	}
	// 11.23 ��ũ��带 Ű���� DŰ�� �� �� �ְ� ��.
	else if (key == 'd' || key == 'D') {
		Control::control.dark = !Control::control.dark;
		instruction();
	}
	// 11.23 <> Ű�� ������ �������� ��� �߰�, arrow�� 1���� �ű�.
	else if (key == ',') {
		if (arrow.getAngle() >= 10 && arrow.getAngle() < 170)
			arrow.move_left(1);
	}
	else if (key == '.') {
		if (arrow.getAngle() > 10 && arrow.getAngle() <= 170)
			arrow.move_right(1);
	}
	// 11.26 ���带 Ű���� SŰ�� �Ѱ� �� �� �ְ� ��.
	else if (key == 's' || key == 'S') {
		Control::control.muted = !Control::control.muted;
		instruction();
	}
}

// ����Ű ����
void SpecialKey(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_RIGHT:
		// 11.23 ������ �������� ����� �־����� ���� ��� 13���� ��Ȳ���� ���������� 5�� �����̸� 8���� �Ǿ� �� �̻� �������� �ʴ� ������ �߻�.
		// arrow.getAngle()-5 >= 10 �̷� ������ '������ ���� ����' �� �˻��Ͽ� �ذ�. ����Ű�� ��������.
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

// 11.19 by ����
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

// ȭ�� ũ�⸦ �ٲ��� �� ȭ���� �ְ�Ǵ� ���� �����ϱ� ���� �Լ�.
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
	
	// ��ǥ�� ����
	// 11.25 reshape�Լ����� ��ǥ�� �����ϴ°ɷ� ����. ȭ�� ũ�Ⱑ ���ص� ���� ȭ���� ������ ����
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glOrtho(-boundaryX, boundaryX, -boundaryY, boundaryY, -100.0, 100.0);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	// ��ü light�� ��� �����ϰ� �Ѵ�
	glEnable(GL_LIGHTING);
	// light0�� ����ġ�� �Ҵ�
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	light0->draw();
	light1->draw();
	
	// ������ ȿ���� �ֱ� ���� static ���� �߰�
	static int blinking = 0;

	// ���� ���� ��
	// 11.25 Control::control.cleared == false ���� �߰�
	if (Control::control.startgame == true && Control::control.gameover == false && Control::control.cleared == false) {
		if (engine != NULL) {
			opening->setIsPaused(true);
			maintheme->setVolume(2.0);
			maintheme->setIsPaused(false);
		}
		// 11.23 displayOn�� dark�� �ٲ�
		// dark�� �⺻���� true�̰� ���콺 ��Ŭ�� �޴����� �� �� �ִ�.
		if (Control::control.dark) {
			// Texture �ڵ�. �Լ� ������ Texture.h �� �ִ�
			tex[1]->drawBackWithTexturing();
		}
		else {
			tex[0]->drawBackWithTexturing();
		}
		
		// laser�� �⺻���� false�̰� ���콺 ��Ŭ�� �޴����� �� �� �ִ�
		if (Control::control.laser) {
			pathlaser.draw(arrow.getAngle());
		}

		// ����׿� God Mode ����.
		/*if (Control::control.GOD_MODE) {
			int x, y, color;
			char exit;
			cout << "::::::::::::::GOD MODE::::::::::::::" << endl << endl;
			cout << "��ǥ �Է� (��ǥ�� ����) : ";
			scanf_s("%d, %d", &x, &y);
			cout << "�� �Է� (R=1, G=2, S=3, Y=4) : ";
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

		// ��� �̹����� ���� ����
		char fpschar[15];
		sprintf_s(fpschar, "%d fps", currentFrames);
		draw_characters(GLUT_BITMAP_TIMES_ROMAN_10, fpschar, -200, 290);

		char intchar[5];
		sprintf_s(intchar, "%d", Control::control.score);
		draw_characters(GLUT_BITMAP_TIMES_ROMAN_24, intchar, -148, 230);

		// �߻�� ���� next �� ��ġ
		for ( vector<Ball>::size_type i = 0; i < ball_not_launched.size(); i++)
			ball_not_launched[i].draw();
		// board_balls�� �����ϴ� ���� �׸���
		for (int i = 0; i < 12; i++) {
			bool row_exist = false;
			for (int j = 0; j < 10 - (i % 2); j++) {
				row_exist = row_exist || board_balls[i][j].isExist();
				if (board_balls[i][j].isExist())
					board_balls[i][j].draw();
			}
			if (row_exist == false) break;
		}
		// dropball ���󰡴� ��� �׸���
		for (vector<Ball>::size_type i = 0; i < dropball.size(); i++)
			dropball[i].draw();
		
		timebar.draw();
	}
	// ���� Ŭ����
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
	// ���� ����
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
	// ���� ���� �� ȭ��
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

// ���α׷� ����� �޸� ����
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

	// 11.19 by ����
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
	glutKeyboardFunc(ControlKey);	// Ű���� ����
	glutSpecialFunc(SpecialKey); // ����Ű
	// enter GLUT event processing cycle
	atexit(exitProgram);
	glutMainLoop();

	return 0;
}

