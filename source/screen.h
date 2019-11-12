#ifndef SCREEN_H
#define SCREEN_H

#include <stdio.h>
#include <Windows.h>

enum keyboard {
	UP_ARROW = 72,
	LEFT_ARROW = 75,
	RIGHT_ARROW = 77,
	DOWN_ARROW = 80,
	ENTER_KEY = 13,
	EXIT_BUTTON = 27,
	PAUSE_BUTTON = 112
};

// define enum keyboard value

void gotoxy(int, int);

void clrscr(void);

char waitForAnyKey(void);

void pauseMenu(void);

void refreshInfoBar(int, int);

void youWinScreen(void);

void gameOverScreen(void);

#endif
