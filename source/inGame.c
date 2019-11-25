#include "inGame.h"
#include "screen.h"
#include "HighScores.h"

int getGameStart(void)
{
	clrscr();

	do
	{
		gotoxy(gameStartX, gameStartY);
		printf("게임을 시작하려면 아무 키나 누르세요.");
	} while (!waitForAnyKey()); // 키가 입력될때 까지

	return true;
} // 게임 시작을 하는 함수

int checkKeysPressed(int direction)
{
	int pressed;

	if (_kbhit()) // kbhit에서 _kbhit으로 대체됨
	{
		pressed = _getch(); // getch에서 _getch로 대체됨
		if (direction != pressed)
		{
			if (pressed == DOWN_ARROW && direction != UP_ARROW)
				direction = pressed;
			else if (pressed == UP_ARROW && direction != DOWN_ARROW)
				direction = pressed;
			else if (pressed == LEFT_ARROW && direction != RIGHT_ARROW)
				direction = pressed;
			else if (pressed == RIGHT_ARROW && direction != LEFT_ARROW)
				direction = pressed;
			else if (pressed == EXIT_BUTTON || pressed == PAUSE_BUTTON)
				pauseMenu(); // 일시정지
		} // 입력한 키가 유효한지 확인하는 조건문
	}
	return(direction);
} // 입력한 방향키를 확인하는 함수


int collisionSnake(struct point PT, int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int detect)
{
	int i;
	for (i = detect; i < snakeLength; i++)
	{
		if (PT.x == snakeXY[0][i] && PT.y == snakeXY[1][i])
			return(true);
	} // 입력받은 detect인자부터 뱀의 길이만큼의 좌표와 입력한 PT 좌표를 비교
	return(false);
} // 뱀머리PT와 몸이 부딪혔는지, 또는 Food생성시 Food와 접촉했는지 확인하는 함수

int collisionObst(struct point PT, struct obstInfo OI)
{
	int i;
	for (i = 0; i < OI.obstCount; i++)
	{
		if (PT.x == OI.obstXY[0][i] && PT.y == OI.obstXY[1][i])
			return(true);
	} // 입력한 PT좌표와 생성된 장애물들 좌표 비교
	return(false);
} // 장애물이 중복된 위치에 생성되지 않도록 하는 함수


int generateFood(int foodXY[], int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength)
{
	struct point PT;
	do
	{
		srand((unsigned int)(time(NULL)));
		foodXY[0] = (int)((double)rand() / (RAND_MAX) * (defaultWidth - 2.0) * 100000) % (defaultWidth - 2) + 2;
		srand((unsigned int)(time(NULL)));
		foodXY[1] = (int)((double)rand() / (RAND_MAX) * (defaultWidth - 2.0) * 100000) % (defaultHeight - 6) + 2;
		PT.x = foodXY[0];
		PT.y = foodXY[1];
	} while (collisionSnake(PT, snakeXY, snakeLength, 0)); // 콘솔 내 좌표 랜덤값과 뱀의 위에 Food가 생성되지 않도록 방지

	gotoxy(foodXY[0], foodXY[1]);
	printf("%c", FOOD); // Food 출력

	return(0);
} // Food 생성 함수

int generateObst(struct obstInfo *OI, int foodXY[], int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength)
{
	struct point PT;
	do
	{
		srand((unsigned int)(time(NULL)));
		OI->obstXY[0][OI->obstCount] = (int)((double)rand() / (RAND_MAX) * (defaultWidth - 2.0) * 1000000) % (defaultWidth - 2) + 2;
		srand((unsigned int)(time(NULL)));
		OI->obstXY[1][OI->obstCount] = (int)((double)rand() / (RAND_MAX) * (defaultWidth - 2.0) * 1000000) % (defaultHeight - 6) + 2;
		PT.x = OI->obstXY[0][OI->obstCount];
		PT.y = OI->obstXY[1][OI->obstCount];
	} while (collisionSnake(PT, snakeXY, snakeLength, 0) 
		|| collisionObst(PT, *OI)
		|| (PT.x == foodXY[0] && PT.y == foodXY[1])
		); // 생성되면 안되는 위치에 생성하는 것 방지

	gotoxy(OI->obstXY[0][OI->obstCount], OI->obstXY[1][OI->obstCount]);
	printf("%c", OBSTACLE); // 장애물 출력

	return(0);
} // 장애물 생성 함수


void moveSnakeArray(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int direction)
{
	int i;
	for (i = snakeLength - 1; i >= 1; i--)
	{
		snakeXY[0][i] = snakeXY[0][i - 1];
		snakeXY[1][i] = snakeXY[1][i - 1];
	} // 뱀배열의 값을 한칸씩 이동시켜주는 반복문

	switch (direction)
	{
	case DOWN_ARROW:
		snakeXY[1][0]++;
		break;
	case RIGHT_ARROW:
		snakeXY[0][0]++;
		break;
	case UP_ARROW:
		snakeXY[1][0]--;
		break;
	case LEFT_ARROW:
		snakeXY[0][0]--;
		break;
	} // 현재 이동방향에 따라 뱀배열 Head좌표 수정

	return;
} // 뱀배열 좌표를 이동시키는 함수

void move(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int direction)
{
	int x;
	int y;

	x = snakeXY[0][snakeLength - 1];
	y = snakeXY[1][snakeLength - 1];

	gotoxy(x, y); // 뱀배열 제일 끝 좌표로 이동
	printf("%c", BLANK); // 콘솔창 지우기

	gotoxy(snakeXY[0][0], snakeXY[1][0]); // 뱀배열 Head좌표로 이동
	printf("%c", SNAKE_BODY); // 현재 머리부분에 몸통 출력

	moveSnakeArray(snakeXY, snakeLength, direction); // 뱀배열 값 수정

	gotoxy(snakeXY[0][0], snakeXY[1][0]); // 새로운 뱀 배열 Head좌표로 이동
	printf("%c", SNAKE_HEAD); // Head출력

	gotoxy(zeroPoint, zeroPoint); // 입력창 지우기

	return;
} // 뱀을 이동시키는 함수(새로 출력하는 함수)

int eatFood(int snakeXY[][SNAKE_ARRAY_SIZE], int foodXY[])
{
	if (snakeXY[0][0] == foodXY[0] && snakeXY[1][0] == foodXY[1])
	{
		foodXY[0] = zeroPoint;
		foodXY[1] = zeroPoint;

		printf("\7"); // 알림음
		return(1);
	}

	return(0);
} // 뱀머리좌표와 음식의 좌표를 확인하는 함수

int collisionDetection(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, struct obstInfo OI)
{
	struct point PT;
	PT.x = snakeXY[0][0];
	PT.y = snakeXY[1][0];
	int collision = false;
	if ((snakeXY[0][0] == 1) || (snakeXY[1][0] == 1) || (snakeXY[0][0] == defaultWidth) || (snakeXY[1][0] == defaultHeight - calcWall))
		collision = true; // 벽에 부딪혔는지 조건 검사
	else if (collisionObst(PT, OI))
		collision = true; // 장애물에 부딪혔는지 조건 검사
	else
		if (collisionSnake(PT, snakeXY, snakeLength, 1))
			collision = true; // 뱀 스스로 부딪혔는지 검사

	return(collision);
} // 게임종료 조건의 충돌을 검사하는 함수

void startGame(int snakeXY[][SNAKE_ARRAY_SIZE], int foodXY[], struct gameInfo GI)
{
	int gameOver = false;
	clock_t endWait;

	int waitMili = CLOCKS_PER_SEC - (addScore) * (CLOCKS_PER_SEC / difficulty);// 진행 속도 설정

	int canChangeDirection = true; // 방향 전환시 바뀌기 전에 
	int oldDirection = false;
	struct obstInfo OI; // 장애물 정보를 담는 구조체
	OI.obstCount = initObstCount;

	endWait = clock() + waitMili; // 현재시각 + 기다릴시간 계산

	do
	{

		if (canChangeDirection)
		{
			oldDirection = GI.direction;
			GI.direction = checkKeysPressed(GI.direction);
		}

		if (oldDirection != GI.direction) // 방향이 바뀐 뒤 이동하기 전에 자신의 몸방향으로 이동하는 것을 방지
			canChangeDirection = false;


		if (clock() >= endWait) // 지정해 놓은 시간을 넘어갔을때
		{
			move(snakeXY, GI.snakeLength, GI.direction); // move함수 호출
			canChangeDirection = true; // 이동한 뒤에는 다시 키 입력 가능

			if (eatFood(snakeXY, foodXY)) // 음식을 먹었을 때
			{
				generateFood(foodXY, snakeXY, GI.snakeLength); // 음식을 새로 하나 생성
				GI.snakeLength++; // 뱀 길이 증가
				GI.score += addScore; // 점수 증가

				if (GI.snakeLength % createObst == false)
				{
					generateObst(&OI, foodXY, snakeXY, GI.snakeLength);
				} // 뱀이 일정길이가 될때마다 장애물 생성
				OI.obstCount++; // 장애물 갯수 카운트

				refreshInfoBar(GI.score); // 정보 최신화
			}

			endWait = clock() + waitMili; // 새로운 시간 설정
		}

		gameOver = collisionDetection(snakeXY, GI.snakeLength, OI); // 게임 종료 검사

		if (GI.snakeLength >= SNAKE_ARRAY_SIZE - 5) // 게임 승리 조건
		{
			gameOver = win; // 승리 저장
			GI.score += extraPoint; // 승리시 추가 포인트
		}

	} while (!gameOver); // 조건에 검사되기 전까지 반복

	switch (gameOver) // 종료 case에 따라 결과 표시
	{
	case lose:
		printf("\7"); // 알람
		printf("\7"); // 알람

		gameOverScreen(); // 패배화면

		break;
	case win:
		youWinScreen(); // 승리화면
		break;
	}

	if (GI.score >= getLowestScore() && GI.score != 0)
	{
		inputScore(GI.score);
		displayHighScores();
	} // 스코어보드에 올라갈 점수라면 갱신

	return;
} // 게임함수



void loadSnake(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength)
{
	int i;
	for (i = 0; i < snakeLength; i++)
	{
		gotoxy(snakeXY[0][i], snakeXY[1][i]);
		printf("%c", SNAKE_BODY); // 몸통 출력
	}

	return;
} // 가장 처음 뱀 그리는 함수


void prepairSnakeArray(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength)
{
	int i;
	int snakeX = snakeXY[0][0];
	int snakeY = snakeXY[1][0]; // 첫 좌표 저장

	for (i = 1; i <= snakeLength; i++)
	{
		snakeXY[0][i] = snakeX + i;
		snakeXY[1][i] = snakeY;
	} // 배열에 좌표 저장

	return;
} // 가장 처음 뱀 배열에 좌표 저장하는 함수


void loadGame(void)
{
	struct gameInfo GI;
	int snakeXY[2][SNAKE_ARRAY_SIZE]; // 뱀 배열

	GI.snakeLength= defaultSnakeL; // 시작 길이

	GI.direction = LEFT_ARROW; // 처음설정 방향

	int foodXY[2];// 음식 위치 저장할 배열

	GI.score= 0; // 시작 점수

	getGameStart(); // speed 입력받는 함수
	snakeXY[0][0] = startSnakeX; // 시작위치
	snakeXY[1][0] = startSnakeY;

	loadEnvironment(); // 배경설정 불러오기
	prepairSnakeArray(snakeXY, GI.snakeLength); // 뱀배열에 값넣기
	loadSnake(snakeXY, GI.snakeLength); // 뱀배열 그리기
	generateFood(foodXY, snakeXY, GI.snakeLength); // 음식생성
	refreshInfoBar(GI.score); // 하단에 정보 초기화
	startGame(snakeXY, foodXY, GI); // 게임 시작함수 호출

	return;
}


void loadEnvironment(void)
{
	int x = zeroPoint, y = zeroPoint;
	int heightCount = defaultHeight - calcWall;

	clrscr(); // 화면 초기화

	gotoxy(x, y); // 화면 좌측 상단으로 이동

	for (; y < heightCount; y++)
	{
		gotoxy(x, y); // 좌측 벽
		printf("%c", WALL);

		gotoxy(defaultWidth, y); // 우측 벽
		printf("%c", WALL);
	}

	y = 1;

	for (; x < defaultWidth + 1; x++)
	{
		gotoxy(x, y); // 상단 벽
		printf("%c", WALL);

		gotoxy(x, heightCount); // 하단 벽
		printf("%c", WALL);
	}
	return;
} // 배경 불러오는 함수
