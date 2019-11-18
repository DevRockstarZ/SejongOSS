#include <time.h>
#include <math.h>
#include "screen.h"
#include <stdio.h>
#include "inGame.h"

void createHighScores(void)
{
	FILE* file;
	errno_t err;
	int i;

	err = fopen_s(&file, "highscores.txt", "w+");
	if (err != 0)
	{
		printf("FILE OPEN FAILED...");
		exit(0);
	}

	for (i = 0; i < 5; i++)
	{
		fprintf(file, "%d", i + 1);
		fprintf(file, "%s", "\t0\t\t\tEMPTY\n");
	}

	fclose(file);
	return;
}

int getLowestScore()
{
	FILE* fp;
	errno_t err;
	char str[128];
	double lowestScore = 0;
	int i;
	int intLength;

	if ((err = fopen_s(&fp, "highscores.txt", "r")) == 0)
	{
		//Create the file, then try open it again.. if it fails this time exit.
		createHighScores(); //This should create a highscores file (If there isn't one)
	}

	while (!feof(fp))
	{
		fgets(str, 126, fp);
	}
	fclose(fp);

	i = 0;

	//Gets the Int length
	while (str[2 + i] != '\t')
	{
		i++;
	}

	intLength = i;

	//Gets converts the string to int
	for (i = 0; i < intLength; i++)
	{
		lowestScore = lowestScore + ((int)str[2 + i] - 48) * pow(10, intLength - i - 1);
	}

	return((int)lowestScore);
}

void inputScore(int score) //This seriously needs to be cleaned up
{
	FILE* fp;
	FILE* file;
	errno_t err;
	char str[20];
	double fScore;
	int i, s, y;
	int intLength;
	double scores[5];
	int x;
	char highScoreName[20];
	char highScoreNames[5][20];

	char name[20];

	int entered = 0;

	clrscr(); //clear the console
	err = fopen_s(&fp, "highscores.txt", "r");
	if (err == 0)
	{
		createHighScores(); //This should create a highscores file (If there isn't one)
	}
	else
	{
		printf("FILE OPEN ERROR!");
		exit(1);
	}
	gotoxy(10, 5);
	printf("Your Score made it into the top 5!!!");
	gotoxy(10, 6);
	printf("Please enter your name: ");

	gets_s(name, sizeof(char) * 20);

	x = 0;
	while (!feof(fp))
	{
		fgets(str, 126, fp);  //Gets a line of text

		i = 0;

		//Gets the Int length
		while (str[2 + i] != '\t')
		{
			i++;
		}

		s = i;
		intLength = i;
		i = 0;
		while (str[5 + s] != '\n')
		{
			//printf("%c",str[5+s]);
			highScoreName[i] = str[5 + s];
			s++;
			i++;
		}
		//printf("\n");

		fScore = 0;
		//Gets converts the string to int
		for (i = 0; i < intLength; i++)
		{
			//printf("%c", str[2+i]);
			fScore = fScore + ((int)str[2 + i] - 48) * pow(10, intLength - i - 1);
		}

		if (score >= fScore && entered != 1)
		{
			scores[x] = score;
			strcpy_s(highScoreNames[x], sizeof(char) * 20, name); // strcpy_s 로 변경

			//printf("%d",x+1);
			//printf("\t%d\t\t\t%s\n",score, name);      
			x++;
			entered = 1;
		}

		//printf("%d",x+1);
		//printf("\t%d\t\t\t%s\n",fScore, highScoreName);
		//strcpy(text, text+"%d\t%d\t\t\t%s\n");
		strcpy_s(highScoreNames[x], sizeof(char) * 20, highScoreName); // strcpy_s로 변경
		scores[x] = fScore;

		//highScoreName = "";
		for (y = 0; y < 20; y++)
		{
			highScoreName[y] = 0;
		}

		x++;
		if (x >= 5)
			break;
	}

	fclose(fp);

	fopen_s(&file, "highscores.txt", "w+");

	for (i = 0; i < 5; i++)
	{
		//printf("%d\t%d\t\t\t%s\n", i+1, scores[i], highScoreNames[i]);
		fprintf(file, "%d\t%d\t\t\t%s\n", i + 1, (int)scores[i], highScoreNames[i]);
	}

	fclose(file);

	return;
}

void displayHighScores(void) //NEED TO CHECK THIS CODE!!!
{
	FILE* fp;
	errno_t err;
	char str[128];
	int y = 5;

	clrscr(); //clear the console
	err = fopen_s(&fp, "highscores.txt", "r");
	if (err == 0)
	{
		//Create the file, then try open it again.. if it fails this time exit.
		createHighScores(); //This should create a highscores file (If there isn't one)
	}
	else
	{
		printf("FILE OPEN ERROR!!");
		exit(1);
	}
	gotoxy(10, y++);
	printf("High Scores");
	gotoxy(10, y++);
	printf("Rank\tScore\t\t\tName");
	while (!feof(fp)) {
		gotoxy(10, y++);
		if (fgets(str, 126, fp))
			printf("%s", str);
	}

	fclose(fp);   //Close the file
	gotoxy(10, y++);

	printf("Press any key to continue...");
	waitForAnyKey();
	return;
}

int main() //Need to fix this up
{
	system("mode con cols=80 lines=24");
	welcomeArt();
	gameOverScreen();
	do
	{
		switch (mainMenu())
		{
		case 0:
			loadGame();
			break;
		case 1:
			displayHighScores();
			break;
		case 2:
			controls();
			break;
		case 3:
			exitYN();
			break;
		}
	} while (1);

	return(0);
}

