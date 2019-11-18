#include "inGame.h"
#include "screen.h"

struct point {
	int x;
	int y;
};

struct window {
	int width;
	int height;
};

struct gameInfo {
	int snakeLength;
	int direction;
	int score;
	int speed;
};

int getGameSpeed(void)
{
	int speed;
	clrscr();

	do
	{
		gotoxy(10, 5);
		printf("Select The game speed between 1 and 9.");
		speed = waitForAnyKey() - 48;
	} while (speed < 1 || speed > 9);
	return(speed);
}

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
				pauseMenu();
		}
	}
	return(direction);
}


int collisionSnake(struct point PT, int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int detect)
{
	int i;
	for (i = detect; i < snakeLength; i++) //Checks if the snake collided with itself
	{
		if (PT.x == snakeXY[0][i] && PT.y == snakeXY[1][i])
			return(1);
	}
	return(0);
}



//Generates food & Makes sure the food doesn't appear on top of the snake <- This sometimes causes a lag issue!!! Not too much of a problem tho
int generateFood(int foodXY[], struct window WD, int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength)
{

	struct point PT;

	// int i 안써서 제거
	do
	{
		srand((unsigned int)(time(NULL)));
		foodXY[0] = rand() % (WD.width - 2) + 2;
		srand((unsigned int)(time(NULL)));
		foodXY[1] = rand() % (WD.height - 6) + 2;
		PT.x = foodXY[0];
		PT.y = foodXY[1];
	} while (collisionSnake(PT, snakeXY, snakeLength, 0)); //This should prevent the "Food" from being created on top of the snake. - However the food has a chance to be created ontop of the snake, in which case the snake should eat it...

	gotoxy(foodXY[0], foodXY[1]);
	printf("%c", FOOD);

	return(0);
}


void moveSnakeArray(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int direction)
{
	int i;
	for (i = snakeLength - 1; i >= 1; i--)
	{
		snakeXY[0][i] = snakeXY[0][i - 1];
		snakeXY[1][i] = snakeXY[1][i - 1];
	}

	/*
	because we dont actually know the new snakes head x y,
	we have to check the direction and add or take from it depending on the direction.
	*/
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
	}

	return;
}

void move(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int direction)
{
	int x;
	int y;

	//Remove the tail ( HAS TO BE DONE BEFORE THE ARRAY IS MOVED!!!!! )
	x = snakeXY[0][snakeLength - 1];
	y = snakeXY[1][snakeLength - 1];

	gotoxy(x, y);
	printf("%c", BLANK);

	//Changes the head of the snake to a body part
	gotoxy(snakeXY[0][0], snakeXY[1][0]);
	printf("%c", SNAKE_BODY);

	moveSnakeArray(snakeXY, snakeLength, direction);

	gotoxy(snakeXY[0][0], snakeXY[1][0]);
	printf("%c", SNAKE_HEAD);

	gotoxy(1, 1); //Gets rid of the darn flashing underscore.

	return;
}

//This function checks if the snakes head his on top of the food, if it is then it'll generate some more food...
int eatFood(int snakeXY[][SNAKE_ARRAY_SIZE], int foodXY[])
{
	if (snakeXY[0][0] == foodXY[0] && snakeXY[1][0] == foodXY[1])
	{
		foodXY[0] = 0;
		foodXY[1] = 0; //This should prevent a nasty bug (loops) need to check if the bug still exists...

		printf("\7"); //Beep
		return(1);
	}

	return(0);
}


int collisionDetection(int snakeXY[][SNAKE_ARRAY_SIZE], struct window WD, int snakeLength) //Need to Clean this up a bit
{
	struct point PT;
	PT.x = snakeXY[0][0];
	PT.y = snakeXY[1][0];
	int colision = 0;
	if ((snakeXY[0][0] == 1) || (snakeXY[1][0] == 1) || (snakeXY[0][0] == WD.width) || (snakeXY[1][0] == WD.height - 4)) //Checks if the snake collided wit the wall or it's self
		colision = 1;
	else
		if (collisionSnake(PT, snakeXY, snakeLength, 1)) //If the snake collided with the wall, theres no point in checking if it collided with itself.
			colision = 1;

	return(colision);
}



//Messy, need to clean this function up
void startGame(int snakeXY[][SNAKE_ARRAY_SIZE], int foodXY[], struct window WD, struct gameInfo GI)
{
	int gameOver = 0;
	clock_t endWait;

	//CLOCKS_PER_SEC-(n-1)*(CLOCKS_PER_SEC/10)
	int waitMili = CLOCKS_PER_SEC - (GI.speed) * (CLOCKS_PER_SEC / 10);   //Sets the correct wait time according to the selected speed
	int tempScore = 10 * GI.speed;
	int oldDirection = 0;
	int canChangeDirection = 1;
	//int seconds = 1;

	endWait = clock() + waitMili;

	do
	{
		if (canChangeDirection)
		{
			oldDirection = GI.direction;
			GI.direction = checkKeysPressed(GI.direction);
		}

		if (oldDirection != GI.direction)//Temp fix to prevent the snake from colliding with itself
			canChangeDirection = 0;

		if (clock() >= endWait) //haha, it moves according to how fast the computer running it is...
		{
			//gotoxy(1,1);
			//printf("%d - %d",clock() , endWait);
			move(snakeXY, GI.snakeLength, GI.direction);
			canChangeDirection = 1;


			if (eatFood(snakeXY, foodXY))
			{
				generateFood(foodXY, WD , snakeXY, GI.snakeLength); //Generate More Food
				GI.snakeLength++;
				GI.score += GI.speed;
				//x++;
				//gotoxy(1,1);
				//printf("%d >= %d", 10*speed+score, tempScore);
				if (GI.score >= 10 * GI.speed + tempScore)
					//if( 2 >= 2)
				{
					GI.speed++;
					tempScore = GI.score;

					if (GI.speed <= 9)//this needs to be fixed
						waitMili = waitMili - (CLOCKS_PER_SEC / 10);
					else
					{
						if (waitMili >= 40) //Maximum Speed (the game has to be beatable)
							waitMili = waitMili - (CLOCKS_PER_SEC / 200);

					}
					//level++;
					//gotoxy(1,2);
					//printf("    ");
					//gotoxy(1,2);
					//printf("%d",waitMili);
					//x = 0;
				}

				refreshInfoBar(GI.score, GI.speed);
			}

			endWait = clock() + waitMili; //TEMP FIX, NEED TO FIND A WAY TO RESET CLOCK().. Na, seems to work fine this way...
		}

		gameOver = collisionDetection(snakeXY, WD , GI.snakeLength);

		if (GI.snakeLength >= SNAKE_ARRAY_SIZE - 5) //Just to make sure it doesn't get longer then the array size & crash
		{
			gameOver = 2;//You Win! <- doesn't seem to work - NEED TO FIX/TEST THIS
			GI.score += 1500; //When you win you get an extra 1500 points!!!
		}

	} while (!gameOver);

	switch (gameOver)
	{
	case 1:
		printf("\7"); //Beep
		printf("\7"); //Beep

		gameOverScreen();

		break;
	case 2:
		youWinScreen();
		break;
	}

	if (GI.score >= getLowestScore() && GI.score != 0)
	{
		inputScore(GI.score);
		displayHighScores();
	}

	return;
}



void loadSnake(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength)
{
	int i;
	/*
	First off, The snake doesn't actually have enough XY coordinates (only 1 - the starting location), thus we use
	these XY coordinates to "create" the other coordinates. For this we can actually use the function used to move the snake.
	This helps create a "whole" snake instead of one "dot", when someone starts a game.
	*/
	//moveSnakeArray(snakeXY, snakeLength); //One thing to note ATM, the snake starts of one coordinate to whatever direction it's pointing...

	//This should print out a snake :P
	for (i = 0; i < snakeLength; i++)
	{
		gotoxy(snakeXY[0][i], snakeXY[1][i]);
		printf("%c", SNAKE_BODY); //Meh, at some point I should make it so the snake starts off with a head...
	}

	return;
}



/* NOTE, This function will only work if the snakes starting direction is left!!!!
Well it will work, but the results wont be the ones expected.. I need to fix this at some point.. */
void prepairSnakeArray(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength)
{
	int i;
	int snakeX = snakeXY[0][0];
	int snakeY = snakeXY[1][0];

	// this is used in the function move.. should maybe create a function for it...
	/*switch(direction)
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
	}
	*/


	for (i = 1; i <= snakeLength; i++)
	{
		snakeXY[0][i] = snakeX + i;
		snakeXY[1][i] = snakeY;
	}

	return;
}


//This function loads the enviroment, snake, etc
void loadGame(void)
{
	struct gameInfo GI;
	int snakeXY[2][SNAKE_ARRAY_SIZE]; //Two Dimentional Array, the first array is for the X coordinates and the second array for the Y coordinates

	GI.snakeLength= 4; //Starting Length

	GI.direction = LEFT_ARROW; //DO NOT CHANGE THIS TO RIGHT ARROW, THE GAME WILL INSTANTLY BE OVER IF YOU DO!!! <- Unless the prepairSnakeArray function is changed to take into account the direction....

	int foodXY[] = { 5,5 };// Stores the location of the food

	GI.score= 0;
	//int level = 1;

	struct window WD;
	WD.width = 80;
	WD.height = 25;
	GI.speed = getGameSpeed();

	//The starting location of the snake
	snakeXY[0][0] = 40;
	snakeXY[1][0] = 10;

	loadEnvironment(WD); //borders
	prepairSnakeArray(snakeXY, GI.snakeLength);
	loadSnake(snakeXY, GI.snakeLength);
	generateFood(foodXY, WD , snakeXY, GI.snakeLength);
	refreshInfoBar(GI.score, GI.speed); //Bottom info bar. Score, Level etc
	startGame(snakeXY, foodXY, WD, GI);

	return;
}


void loadEnvironment(struct window WD)//This can be done in a better way... FIX ME!!!! Also i think it doesn't work properly in ubuntu <- Fixed
{
	int x = 1, y = 1;
	int rectangleHeight = WD.height - 4;

	clrscr(); //clear the console

	gotoxy(x, y); //Top left corner

	for (; y < rectangleHeight; y++)
	{
		gotoxy(x, y); //Left Wall 
		printf("%c", WALL);

		gotoxy(WD.width, y); //Right Wall

		printf("%c", WALL);
	}

	y = 1;

	for (; x < WD.width + 1; x++)
	{
		gotoxy(x, y); //Left Wall 
		printf("%c", WALL);

		gotoxy(x, rectangleHeight); //Right Wall
		printf("%c", WALL);
	}
	return;
}

