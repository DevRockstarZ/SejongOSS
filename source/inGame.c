#include "inGame.h"
#include "screen.h"

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


int collisionSnake(int x, int y, int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int detect)
{
	int i;
	for (i = detect; i < snakeLength; i++) //Checks if the snake collided with itself
	{
		if (x == snakeXY[0][i] && y == snakeXY[1][i])
			return(1);
	}
	return(0);
}



//Generates food & Makes sure the food doesn't appear on top of the snake <- This sometimes causes a lag issue!!! Not too much of a problem tho
int generateFood(int foodXY[], int width, int height, int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength)
{
	// int i 안써서 제거
	do
	{
		srand((unsigned int)(time(NULL)));
		foodXY[0] = rand() % (width - 2) + 2;
		srand((unsigned int)(time(NULL)));
		foodXY[1] = rand() % (height - 6) + 2;
	} while (collisionSnake(foodXY[0], foodXY[1], snakeXY, snakeLength, 0)); //This should prevent the "Food" from being created on top of the snake. - However the food has a chance to be created ontop of the snake, in which case the snake should eat it...

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


int collisionDetection(int snakeXY[][SNAKE_ARRAY_SIZE], int consoleWidth, int consoleHeight, int snakeLength) //Need to Clean this up a bit
{
	int colision = 0;
	if ((snakeXY[0][0] == 1) || (snakeXY[1][0] == 1) || (snakeXY[0][0] == consoleWidth) || (snakeXY[1][0] == consoleHeight - 4)) //Checks if the snake collided wit the wall or it's self
		colision = 1;
	else
		if (collisionSnake(snakeXY[0][0], snakeXY[1][0], snakeXY, snakeLength, 1)) //If the snake collided with the wall, theres no point in checking if it collided with itself.
			colision = 1;

	return(colision);
}



//Messy, need to clean this function up
void startGame(int snakeXY[][SNAKE_ARRAY_SIZE], int foodXY[], int consoleWidth, int consoleHeight, int snakeLength, int direction, int score, int speed)
{
	int gameOver = 0;
	clock_t endWait;

	//CLOCKS_PER_SEC-(n-1)*(CLOCKS_PER_SEC/10)
	int waitMili = CLOCKS_PER_SEC - (speed) * (CLOCKS_PER_SEC / 10);   //Sets the correct wait time according to the selected speed
	int tempScore = 10 * speed;
	int oldDirection = 0;
	int canChangeDirection = 1;
	//int seconds = 1;

	endWait = clock() + waitMili;

	do
	{
		if (canChangeDirection)
		{
			oldDirection = direction;
			direction = checkKeysPressed(direction);
		}

		if (oldDirection != direction)//Temp fix to prevent the snake from colliding with itself
			canChangeDirection = 0;

		if (clock() >= endWait) //haha, it moves according to how fast the computer running it is...
		{
			//gotoxy(1,1);
			//printf("%d - %d",clock() , endWait);
			move(snakeXY, snakeLength, direction);
			canChangeDirection = 1;


			if (eatFood(snakeXY, foodXY))
			{
				generateFood(foodXY, consoleWidth, consoleHeight, snakeXY, snakeLength); //Generate More Food
				snakeLength++;
				score += speed;
				//x++;
				//gotoxy(1,1);
				//printf("%d >= %d", 10*speed+score, tempScore);
				if (score >= 10 * speed + tempScore)
					//if( 2 >= 2)
				{
					speed++;
					tempScore = score;

					if (speed <= 9)//this needs to be fixed
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

				refreshInfoBar(score, speed);
			}

			endWait = clock() + waitMili; //TEMP FIX, NEED TO FIND A WAY TO RESET CLOCK().. Na, seems to work fine this way...
		}

		gameOver = collisionDetection(snakeXY, consoleWidth, consoleHeight, snakeLength);

		if (snakeLength >= SNAKE_ARRAY_SIZE - 5) //Just to make sure it doesn't get longer then the array size & crash
		{
			gameOver = 2;//You Win! <- doesn't seem to work - NEED TO FIX/TEST THIS
			score += 1500; //When you win you get an extra 1500 points!!!
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

	if (score >= getLowestScore() && score != 0)
	{
		inputScore(score);
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
	int snakeXY[2][SNAKE_ARRAY_SIZE]; //Two Dimentional Array, the first array is for the X coordinates and the second array for the Y coordinates

	int snakeLength = 4; //Starting Length

	int direction = LEFT_ARROW; //DO NOT CHANGE THIS TO RIGHT ARROW, THE GAME WILL INSTANTLY BE OVER IF YOU DO!!! <- Unless the prepairSnakeArray function is changed to take into account the direction....

	int foodXY[] = { 5,5 };// Stores the location of the food

	int score = 0;
	//int level = 1;

	//Window Width * Height - at some point find a way to get the actual dimensions of the console... <- Also somethings that display dont take this dimentions into account.. need to fix this...
	int consoleWidth = 80;
	int consoleHeight = 25;

	int speed = getGameSpeed();

	//The starting location of the snake
	snakeXY[0][0] = 40;
	snakeXY[1][0] = 10;

	loadEnvironment(consoleWidth, consoleHeight); //borders
	prepairSnakeArray(snakeXY, snakeLength);
	loadSnake(snakeXY, snakeLength);
	generateFood(foodXY, consoleWidth, consoleHeight, snakeXY, snakeLength);
	refreshInfoBar(score, speed); //Bottom info bar. Score, Level etc
	startGame(snakeXY, foodXY, consoleWidth, consoleHeight, snakeLength, direction, score, speed);

	return;
}

void loadEnvironment(int consoleWidth, int consoleHeight)//This can be done in a better way... FIX ME!!!! Also i think it doesn't work properly in ubuntu <- Fixed
{
	int x = 1, y = 1;
	int rectangleHeight = consoleHeight - 4;
	clrscr(); //clear the console

	gotoxy(x, y); //Top left corner

	for (; y < rectangleHeight; y++)
	{
		gotoxy(x, y); //Left Wall 
		printf("%c", WALL);

		gotoxy(consoleWidth, y); //Right Wall
		printf("%c", WALL);
	}

	y = 1;
	for (; x < consoleWidth + 1; x++)
	{
		gotoxy(x, y); //Left Wall 
		printf("%c", WALL);

		gotoxy(x, rectangleHeight); //Right Wall
		printf("%c", WALL);
	}

	/*
	   for (i = 0; i < 80; i++)
	   {
	   printf("%c",WALL);
	   }
	   for (i = 0; i < 17; i++)
	   {
	   printf("%c\n",WALL);
	   }
	   for (i = 0; i < 21; i++)
	   {
	   printf("%c\n",WALL);
	   gotoxy(80,i);
	   }
	   for (i = 0; i < 81; i++)
	   {
	   printf("%c",WALL);
	   }
	*/
	return;
}

