#ifndef SCREEN_H
#define SCREEN_H

#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <malloc.h>

enum keyboard {
	UP_ARROW = 72,
	LEFT_ARROW = 75,
	RIGHT_ARROW = 77,
	DOWN_ARROW = 80,
	ENTER_KEY = 13,
	EXIT_BUTTON = 27,
	PAUSE_BUTTON = 112
}; // define enum keyboard value

enum constValue {
	pauseX = 28,
	pauseY = 23,
	refreshLeftX = 5,
	refreshLeftY = 23,
	refreshRightX = 40,
	refreshRightY = 23,
	youwinX = 6,
	youwinY = 8,
	welcomeX = 16,
	welcomeY = 4,
	gameOverX = 15,
	gameOverY = 3,
	exitX = 20,
	exitY = 21,
	controlX = 16,
	controlY = 2,
	noOption = 0,
	printXMenu = 16,
	printYMenu = 4,
	optionXStart = 24,
	optionYStart = 17,
	optionYEnd = 20,
	optionLoadGame = 0,
	optionHighScores = 1,
	optionControls = 2,
	optionExit = 3
}; // define const values

void gotoxy(int, int);

void clrscr(void);

char waitForAnyKey(void);

void pauseMenu(void);

void refreshInfoBar(int);

void youWinScreen(void);

void gameOverScreen(void);

void welcomeArt(void);

void mainMenu(void);

BOOL exitYN(void);

void controls(void);

int menuSelector(void); // 매개변수 2개로 수정

void clearArrow(int, int); // 메인메뉴 화살표 제거 함수

void printArrow(int, int); // 메인메뉴 화살표 출력 함수

int * moveArrow(int, int, int, int); // 메인메뉴 화살표 움직이기 have to return array!!!

#endif
