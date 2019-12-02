


#ifndef HIGHSCORES_H
#define HIGHSCORES_H

#include <stdio.h>
#include <math.h>

void createHighScores();

int strToIntScore(int len, char *str);

typedef struct HighScoreList
{
	int hscore;
	char hsname[20];
}HighScoreList;

void inputScore(int score);

void displayHighScores();

int getLowestScore();

enum constvalue
{
	xpos = 20,
	ypos = 11,
	display_ypos = 8
};

#endif
