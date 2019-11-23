


#ifndef HIGHSCORES_H
#define HIGHSCORES_H

#include <stdio.h>
#include <math.h>

void createHighScores();

int str_to_int_score(int len, char *str);

int getLowestScore();

typedef struct HighScoreList
{
	int hscore;
	char hsname[20];
}HighScoreList;

void inputScore(int score);

void displayHighScores();

enum constvalue
{
	xpos = 20,
	ypos = 11,
	display_ypos = 8
};

#endif
