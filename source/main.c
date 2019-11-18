
#include "screen.h"
#include "inGame.h"
#include "HighScores.h"

int main() //Need to fix this up
{
	system("mode con cols=80 lines=24");
	welcomeArt();

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
