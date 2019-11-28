#ifndef INGAME_H
#define INGAME_H

#include <time.h>

#define SNAKE_ARRAY_SIZE 310
#define WALL_SIZE 20

struct point {
	int x;
	int y;
}; // x,y 좌표 저장

struct window {
	int width;
	int height;
}; // 콘솔창 크기 저장

struct gameInfo {
	int snakeLength;
	int direction;
	int score;
}; // 게임 정보 저장

struct obstInfo {
	int obstXY[2][WALL_SIZE];
	int obstCount;
};

enum graphic {
	SNAKE_HEAD = 'X',
	SNAKE_BODY = '@',
	FOOD = '*',
	BLANK = ' ',
	WALL = '+',
	OBSTACLE = '#'
}; // 열거형 사용

enum constValue {
	zeroPoint = 0,
	false = 0,
	true = 1,
	lose = 1,
	win = 2,
	difficulty = 10,
	speed = 9,
	maxWaitMili = 40,
	calcSec = 200,
	extraPoint = 1500,
	defaultSnakeL = 4,
	defaultWidth = 80,
	defaultHeight = 25,
	startSnakeX = 40,
	startSnakeY = 10,
	calcWall = 4,
	addScore = 9,
	gameStartX = 22,
	gameStartY = 11,
	initObstCount = 0,
	createObst = 3,
};

int getGameStart(void);

int checkKeysPressed(int);

int collisionSnake(struct point, int [][SNAKE_ARRAY_SIZE], int, int);

int collisionObst(struct point, struct obstInfo);

int generateFood(int [], int [][SNAKE_ARRAY_SIZE], int);

int generateObst(struct obstInfo* , int [], int [][SNAKE_ARRAY_SIZE], int);

void moveSnakeArray(int [][SNAKE_ARRAY_SIZE], int, int);

void move(int [][SNAKE_ARRAY_SIZE], int, int);

int eatFood(int [][SNAKE_ARRAY_SIZE], int []);

int collisionDetection(int [][SNAKE_ARRAY_SIZE], int, struct obstInfo);

void startGame(int [][SNAKE_ARRAY_SIZE], int [], struct gameInfo);

void loadSnake(int [][SNAKE_ARRAY_SIZE], int);

void prepairSnakeArray(int [][SNAKE_ARRAY_SIZE], int);

void loadGame(void);

void loadEnvironment(void);

#endif

