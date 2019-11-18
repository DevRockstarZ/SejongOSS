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

int collisionSnake(int x, int y, int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int detect);

int generateFood(int foodXY[], int width, int height, int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength);

void moveSnakeArray(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int direction);

void move(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int direction);

int eatFood(int snakeXY[][SNAKE_ARRAY_SIZE], int foodXY[]);

int collisionDetection(int snakeXY[][SNAKE_ARRAY_SIZE], int consoleWidth, int consoleHeight, int snakeLength);

void startGame(int snakeXY[][SNAKE_ARRAY_SIZE], int foodXY[], int consoleWidth, int consoleHeight, int snakeLength, int direction, int score, int speed);

void loadSnake(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength);

void prepairSnakeArray(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength);

void loadGame(void);

void loadEnvironment(int consoleWidth, int consoleHeight);

#endif
