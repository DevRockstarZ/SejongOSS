#ifndef INGAME_H
#define INGAME_H

#include <time.h>

#define SNAKE_ARRAY_SIZE 310

enum graphic {
	SNAKE_HEAD = 'X',
	SNAKE_BODY = '@',
	FOOD = '*',
	BLANK = ' ',
	WALL = '+',
}; // 열거형 사용


int getGameSpeed(void);

int checkKeysPressed(int direction);

int collisionSnake(struct point PT, int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int detect);

int generateFood(int foodXY[], struct window WD, int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength);

void moveSnakeArray(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int direction);

void move(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int direction);

int eatFood(int snakeXY[][SNAKE_ARRAY_SIZE], int foodXY[]);

int collisionDetection(int snakeXY[][SNAKE_ARRAY_SIZE], struct window WD, int snakeLength);

void startGame(int snakeXY[][SNAKE_ARRAY_SIZE], int foodXY[], struct window WD, struct gameInfo GI);

void loadSnake(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength);

void prepairSnakeArray(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength);

void loadGame(void);

void loadEnvironment(struct window WD);

#endif

