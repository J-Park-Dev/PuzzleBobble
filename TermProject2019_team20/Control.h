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
	bool startgame = false;		// 시작화면에서는 false, 게임이 진행되면 true
	bool gameover = false;		// 게임오버 여부
	bool launch = false;		// 공이 발사되어 움직이는 동안에는 true
	bool dark = true;			// 다크모드 여부
	bool GOD_MODE = false;		// 디버그용 GOD_MODE (사용 안함)
	bool laser = false;			// 진행 경로를 보여주는 레이저 스위치
	bool clearMode = true;		// 파일 입력으로 보드를 설정하는 경우, 파일 입력에 실패하면 false. 현재는 파일 입력을 시키지 않고 랜덤으로 초기화함
	bool cleared = false;		// 게임 클리어 여부
	bool muted = false;			// 소리 음소거 여부 (기본값은 재생)
	int score = 0;				// 게임 스코어
public:
	static Control control;		// 위 변수들은 이 static 멤버를 이용하여 제어한다
};

