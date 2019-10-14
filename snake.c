/*
Author: Matthew Vlietstra
Version: 0.5
Date: 28/09/2010
Discription:
This is a console snake game that can (or should) work in linux & windows environments.
Windows:
Compile with borland
Linux:
Please note, tested under Ubuntu not sure if it works in other linux environments. I recommend compiling with borland under windows for best results.
Compile with gcc in linux using the following command:
gcc snake.c –lm –o snake.out
Cygwin:
Although this program may compile/ run in Cygwin it runs slowly.

*/

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <Windows.h>
#include <conio.h>

#define SNAKE_ARRAY_SIZE 310

#ifdef _WIN32
//Windows Libraries


//Windows Constants
//Controls
#define UP_ARROW 72
#define LEFT_ARROW 75
#define RIGHT_ARROW 77
#define DOWN_ARROW 80

#define ENTER_KEY 13

const char SNAKE_HEAD = '!';
const char SNAKE_BODY = '@';
const char WALL = '#';
const char FOOD = '*';
const char BLANK = ' ';

// 캐스팅시 상수값이 잘리기때문에 그냥 바로 선언하였음.

#else
//Linux Libraries
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

//Linux Constants

//Controls (arrow keys for Ubuntu) 
#define UP_ARROW (char)'A' //Originally I used constants but borland started giving me errors, so I changed to #define - I do realize that is not the best way.
#define LEFT_ARROW (char)'D'
#define RIGHT_ARROW (char)'C'
#define DOWN_ARROW (char)'B'

#define ENTER_KEY 10

const char SNAKE_HEAD = 'X';
const char SNAKE_BODY = '#';
const char WALL = '#';
const char FOOD = '*';
const char BLANK = ' ';

//Linux Functions - These functions emulate some functions from the windows only conio header file
//Code: http://ubuntuforums.org/showthread.php?t=549023


//http://cboard.cprogramming.com/c-programming/63166-kbhit-linux.html
int kbhit(void)
{
	struct termios oldt, newt;
	int ch;
	int oldf;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);

	if (ch != EOF)
	{
		ungetc(ch, stdin);
		return 1;
	}

	return 0;
}

//http://www.experts-exchange.com/Programming/Languages/C/Q_10119844.html - posted by jos
char getch()
{
	char c;
	system("stty raw");
	c = getchar();
	system("stty sane");
	//printf("%c",c);
	return(c);
}


//End linux Functions
#endif

//This should be the same on both operating systems
#define EXIT_BUTTON 27 //ESC
#define PAUSE_BUTTON 112 //P

void clrscr()
{
	system("cls");
	return;
} // Linux 라이브러리에서 빼옴

void gotoxy(int x, int y)
{
	printf("%c[%d;%df", 0x1B, y, x);
} // Linux 라이브러리 에서 빼옴



char waitForAnyKey(void)
{
	int pressed;

	while (!_kbhit());

	pressed = _getch();
	//pressed = tolower(pressed);
	return((char)pressed);
}

int getGameSpeed(void)
{
	int speed;
	clrscr();

	do
	{
		gotoxy(10, 5);
		printf("Select The game speed between 1 and 9.");
		speed = waitForAnyKey() - 48;
	} while (speed < 1 || speed > 9); // speed를 정상적으로 입력받기 전까지는 반복
	return(speed);
} // getGameSpeed

void pauseMenu(void)
{
	// int i 선언 제거.

	gotoxy(28, 23);
	printf("**Paused**");

	waitForAnyKey();
	gotoxy(28, 23);
	printf("            ");

	return;
}

//This function checks if a key has pressed, then checks if its any of the arrow keys/ p/esc key. It changes direction acording to the key pressed.
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
} // 현재 이동하고 있는 방향과 누른 키에 따른 방향 전환 및 Pause 함수

//Cycles around checking if the x y coordinates ='s the snake coordinates as one of this parts
//One thing to note, a snake of length 4 cannot collide with itself, therefore there is no need to call this function when the snakes length is <= 4
int collisionSnake(int x, int y, int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int detect)
{
	int i;
	for (i = detect; i < snakeLength; i++)
	{
		if (x == snakeXY[0][i] && y == snakeXY[1][i])
			return(1);
	}
	return(0);
} // 뱀이 어딘가에 부딪히는 지 검사하는 함수 (열매생성 위치, 벽에 부딪혔는지, 뱀스스로 부딪혔는지 검사하는 함수)

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
	} while (collisionSnake(foodXY[0], foodXY[1], snakeXY, snakeLength, 0)); // 랜덤위치가 뱀 위로 생성되지 않게 방지

	gotoxy(foodXY[0], foodXY[1]);
	printf("%c", FOOD);

	return(0);
} // 음식 생성 함수, 게임 시작, 음식을 먹었을 때 호출되어 음식을 생성함

/*
Moves the snake array forward, i.e.
This:
 x 1 2 3 4 5 6
 y 1 1 1 1 1 1
Becomes This:
 x 1 1 2 3 4 5
 y 1 1 1 1 1 1

 Then depending on the direction (in this case west - left) it becomes:

 x 0 1 2 3 4 5
 y 1 1 1 1 1 1

 snakeXY[0][0]--; <- if direction left, take 1 away from the x coordinate
*/
void moveSnakeArray(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int direction)
{
	int i;
	for (i = snakeLength - 1; i >= 1; i--)
	{
		snakeXY[0][i] = snakeXY[0][i - 1];
		snakeXY[1][i] = snakeXY[1][i - 1];
	} // 기존의 SnakeArray의 위치를 조정하는 반복문

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
} // 새로운 HEAD의 위치를 저장하기 위해 방향에 따라 HEAD좌표를 변경하는 함수

void move(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int direction)
{
	int x;
	int y;

	x = snakeXY[0][snakeLength - 1];
	y = snakeXY[1][snakeLength - 1];
	// 뱀을 이동시키기 전에 배열에서 꼬리 부분을 지움
	gotoxy(x, y);
	printf("%c", BLANK);
	// 뱀 꼬리 부분을 화면에서 지움
	gotoxy(snakeXY[0][0], snakeXY[1][0]);
	printf("%c", SNAKE_BODY);
	// 머리부분에 뱀 몸통을 그려줌
	moveSnakeArray(snakeXY, snakeLength, direction);
	// 이동 함수 호출
	gotoxy(snakeXY[0][0], snakeXY[1][0]);
	printf("%c", SNAKE_HEAD);
	// 이동한 뱀의 머리를 그려줌
	gotoxy(1, 1);
	// 포인트를 처음으로 옮겨줌
	return;
} // 기존에 HEAD에 BODY를 출력한 뒤 이동한 부분에 HEAD를 그려주는 함수

//This function checks if the snakes head his on top of the food, if it is then it'll generate some more food...
int eatFood(int snakeXY[][SNAKE_ARRAY_SIZE], int foodXY[])
{
	if (snakeXY[0][0] == foodXY[0] && snakeXY[1][0] == foodXY[1])
	{
		foodXY[0] = 0;
		foodXY[1] = 0;
		//음식위치 초기화
		printf("\7");
		return(1);
	}
	return(0);
} // 뱀의 머리와 음식의 위치가 같은지 검사하는 함수

int collisionDetection(int snakeXY[][SNAKE_ARRAY_SIZE], int consoleWidth, int consoleHeight, int snakeLength)
{
	int colision = 0;
	if ((snakeXY[0][0] == 1) || (snakeXY[1][0] == 1) || (snakeXY[0][0] == consoleWidth) || (snakeXY[1][0] == consoleHeight - 4))
		colision = 1; // 벽에 부딪혔는지 검사
 	else
		if (collisionSnake(snakeXY[0][0], snakeXY[1][0], snakeXY, snakeLength, 1))
			colision = 1; // 뱀 스스로 부딪혔는지 검사

	return(colision);
} // 벽에 부딪히거나 뱀 스스로 부딪혔을 때를 검사하는 함수

void refreshInfoBar(int score, int speed)
{
	gotoxy(5, 23);
	printf("Score: %d", score);

	gotoxy(5, 24);
	printf("Speed: %d", speed);

	gotoxy(52, 23);
	printf("Coder: Matthew Vlietstra");

	gotoxy(52, 24);
	printf("Version: 0.5");

	return;
}

//**************HIGHSCORE STUFF**************//

//-> The highscores system seriously needs to be clean. There are some bugs, entering a name etc

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

//**************END HIGHSCORE STUFF**************//

void youWinScreen(void)
{
	int x = 6, y = 7;
	gotoxy(x, y++);
	printf("'##:::'##::'#######::'##::::'##::::'##:::::'##:'####:'##::: ##:'####:");
	gotoxy(x, y++);
	printf(". ##:'##::'##.... ##: ##:::: ##:::: ##:'##: ##:. ##:: ###:: ##: ####:");
	gotoxy(x, y++);
	printf(":. ####::: ##:::: ##: ##:::: ##:::: ##: ##: ##:: ##:: ####: ##: ####:");
	gotoxy(x, y++);
	printf("::. ##:::: ##:::: ##: ##:::: ##:::: ##: ##: ##:: ##:: ## ## ##:: ##::");
	gotoxy(x, y++);
	printf("::: ##:::: ##:::: ##: ##:::: ##:::: ##: ##: ##:: ##:: ##. ####::..:::");
	gotoxy(x, y++);
	printf("::: ##:::: ##:::: ##: ##:::: ##:::: ##: ##: ##:: ##:: ##:. ###:'####:");
	gotoxy(x, y++);
	printf("::: ##::::. #######::. #######:::::. ###. ###::'####: ##::. ##: ####:");
	gotoxy(x, y++);
	printf(":::..::::::.......::::.......:::::::...::...:::....::..::::..::....::");
	gotoxy(x, y++);

	waitForAnyKey();
	clrscr(); //clear the console
	return;
}

void gameOverScreen(void)
{
	int x = 17, y = 3;

	//http://www.network-science.de/ascii/ <- Ascii Art Gen

	gotoxy(x, y++);
	printf(":'######::::::'###::::'##::::'##:'########:\n");
	gotoxy(x, y++);
	printf("'##... ##::::'## ##::: ###::'###: ##.....::\n");
	gotoxy(x, y++);
	printf(" ##:::..::::'##:. ##:: ####'####: ##:::::::\n");
	gotoxy(x, y++);
	printf(" ##::'####:'##:::. ##: ## ### ##: ######:::\n");
	gotoxy(x, y++);
	printf(" ##::: ##:: #########: ##. #: ##: ##...::::\n");
	gotoxy(x, y++);
	printf(" ##::: ##:: ##.... ##: ##:.:: ##: ##:::::::\n");
	gotoxy(x, y++);
	printf(". ######::: ##:::: ##: ##:::: ##: ########:\n");
	gotoxy(x, y++);
	printf(":......::::..:::::..::..:::::..::........::\n");
	gotoxy(x, y++);
	printf(":'#######::'##::::'##:'########:'########::'####:\n");
	gotoxy(x, y++);
	printf("'##.... ##: ##:::: ##: ##.....:: ##.... ##: ####:\n");
	gotoxy(x, y++);
	printf(" ##:::: ##: ##:::: ##: ##::::::: ##:::: ##: ####:\n");
	gotoxy(x, y++);
	printf(" ##:::: ##: ##:::: ##: ######::: ########::: ##::\n");
	gotoxy(x, y++);
	printf(" ##:::: ##:. ##:: ##:: ##...:::: ##.. ##::::..:::\n");
	gotoxy(x, y++);
	printf(" ##:::: ##::. ## ##::: ##::::::: ##::. ##::'####:\n");
	gotoxy(x, y++);
	printf(". #######::::. ###:::: ########: ##:::. ##: ####:\n");
	gotoxy(x, y++);
	printf(":.......::::::...:::::........::..:::::..::....::\n");

	waitForAnyKey();
	clrscr(); //clear the console
	return;
}

//Messy, need to clean this function up
void startGame(int snakeXY[][SNAKE_ARRAY_SIZE], int foodXY[], int consoleWidth, 
	int consoleHeight, int snakeLength, int direction, int score, int speed)
{
	int gameOver = 0;
	clock_t endWait;

	int waitMili = CLOCKS_PER_SEC - (speed) * (CLOCKS_PER_SEC / 10);  
	int tempScore = 10 * speed;
	int oldDirection = 0;
	int canChangeDirection = 1;

	endWait = clock() + waitMili;
	// 방향, 속도에 따른 점수와 속도를 선언하는 부분

	do
	{
		if (canChangeDirection)
		{
			oldDirection = direction;
			direction = checkKeysPressed(direction);
		}

		if (oldDirection != direction)
			canChangeDirection = 0;
		// BODY부분으로 방향을 틀 수 없도록 방지하는 부분

		if (clock() >= endWait)
		{
			move(snakeXY, snakeLength, direction);
			canChangeDirection = 1;
			// 게임이 끝나기 전까지는 계속 뱀을 이동시켜주는 부분

			if (eatFood(snakeXY, foodXY))
			{
				generateFood(foodXY, consoleWidth, consoleHeight, snakeXY, snakeLength);
				snakeLength++;
				score += speed;
				// 음식을 먹었을 때 스코어 갱신 및 새로 음식을 생성하는 부분
				if (score >= 10 * speed + tempScore)
				{
					speed++;
					tempScore = score;

					if (speed <= 9)
						waitMili = waitMili - (CLOCKS_PER_SEC / 10);
					else
					{
						if (waitMili >= 40)
							waitMili = waitMili - (CLOCKS_PER_SEC / 200);

					}
				} // 스코어에따라 속도와 점수를 갱신하는 부분

				refreshInfoBar(score, speed); // 정보 갱신
			}

			endWait = clock() + waitMili;
		}

		gameOver = collisionDetection(snakeXY, consoleWidth, consoleHeight, snakeLength); // 게임 종료 검사

		if (snakeLength >= SNAKE_ARRAY_SIZE - 5)
		{
			gameOver = 2;
			score += 1500;
		} // 게임을 승리하는 조건

	} while (!gameOver); // 게임이 종료될 때 까지 계속 확인

	switch (gameOver)
	{
	case 1: // 게임 패배
		printf("\7");
		printf("\7");

		gameOverScreen();

		break;
	case 2: // 게임 승리
		youWinScreen();
		break;
	}

	if (score >= getLowestScore() && score != 0)
	{
		inputScore(score);
		displayHighScores();
	} // 순위권 점수 저장

	return;
} // startGame

void loadEnviroment(int consoleWidth, int consoleHeight)//This can be done in a better way... FIX ME!!!! Also i think it doesn't work properly in ubuntu <- Fixed
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

void loadSnake(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength)
{
	int i;

	for (i = 0; i < snakeLength; i++)
	{
		gotoxy(snakeXY[0][i], snakeXY[1][i]);
		printf("%c", SNAKE_BODY);
	} // 배열에 저장된 정보로 가서 뱀 몸통 화면에 출력

	return;
} // loadSnake

/* NOTE, This function will only work if the snakes starting direction is left!!!!
Well it will work, but the results wont be the ones expected.. I need to fix this at some point.. */
void prepareSnakeArray(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength)
{
	int i;
	int snakeX = snakeXY[0][0];
	int snakeY = snakeXY[1][0];
	// 기존 X,Y좌표 갖고오기
	for (i = 1; i <= snakeLength; i++)
	{
		snakeXY[0][i] = snakeX + i;
		snakeXY[1][i] = snakeY;
	} // 뱀 길이만큼 배열에 정보 저장

	return;
} // prepareSnakeArrary

//This function loads the enviroment, snake, etc
void loadGame(void)
{
	int snakeXY[2][SNAKE_ARRAY_SIZE];
	// 뱀 배열 생성 [0]x좌표, [1]y좌표
	int snakeLength = 4;
	// 초기 뱀 길이
	int direction = LEFT_ARROW;
	// 초기 방향 설정
	int foodXY[] = { 5,5 };
	// 음식 초기화
	int score = 0;
	// 스코어 초기화
	int consoleWidth = 80;
	int consoleHeight = 25;
	// 화면 크기 초기화
	int speed = getGameSpeed();
	// 사용자가 게임 스피드 선택
	snakeXY[0][0] = 40;
	snakeXY[1][0] = 10;
	// 초기 뱀 위치 설정
	loadEnviroment(consoleWidth, consoleHeight);
	// 정보 화면
	prepareSnakeArray(snakeXY, snakeLength);
	// 뱀 배열 설정 함수
	loadSnake(snakeXY, snakeLength);
	// 설정되어있는 정보 불러오기
	generateFood(foodXY, consoleWidth, consoleHeight, snakeXY, snakeLength);
	// 음식 생성
	refreshInfoBar(score, speed);
	// 게임 정보 화면에 출력
	startGame(snakeXY, foodXY, consoleWidth, consoleHeight, snakeLength, direction, score, speed);
	// 게임 시작 함수

	return;
} // 가장 처음 시작할 때 게임 정보를 초기화 하는 함수

//**************MENU STUFF**************//

int menuSelector(int x, int y, int yStart)
{
	char key;
	int i = 0;
	x = x - 2;
	gotoxy(x, yStart);

	printf(">");

	gotoxy(1, 1);


	do
	{
		key = waitForAnyKey();
		//printf("%c %d", key, (int)key);
		if (key == (char)UP_ARROW)
		{
			gotoxy(x, yStart + i);
			printf(" ");

			if (yStart >= yStart + i)
				i = y - yStart - 2;
			else
				i--;
			gotoxy(x, yStart + i);
			printf(">");
		}
		else
			if (key == (char)DOWN_ARROW)
			{
				gotoxy(x, yStart + i);
				printf(" ");

				if (i + 2 >= y - yStart)
					i = 0;
				else
					i++;
				gotoxy(x, yStart + i);
				printf(">");
			}
		//gotoxy(1,1);
		//printf("%d", key);
	} while (key != (char)ENTER_KEY); //While doesn't equal enter... (13 ASCII code for enter) - note ubuntu is 10
	return(i);
}

void welcomeArt(void)
{
	clrscr(); //clear the console
	//Ascii art reference: http://www.chris.com/ascii/index.php?art=animals/reptiles/snakes
	printf("\n");
	printf("\t\t    _________         _________          \n");
	printf("\t\t   /         \\       /         \\          \n");
	printf("\t\t  /  /~~~~~\\  \\     /  /~~~~~\\  \\          \n");
	printf("\t\t  |  |     |  |     |  |     |  |          \n");
	printf("\t\t  |  |     |  |     |  |     |  |          \n");
	printf("\t\t  |  |     |  |     |  |     |  |         /   \n");
	printf("\t\t  |  |     |  |     |  |     |  |       //   \n");
	printf("\t\t (o  o)    \\  \\_____/  /     \\  \\_____/ /    \n");
	printf("\t\t  \\__/      \\         /       \\        /    \n");
	printf("\t\t    |        ~~~~~~~~~         ~~~~~~~~       \n");
	printf("\t\t    ^                                 \n");
	printf("\t      Welcome To The Snake Game!         \n");
	printf("\t             Press Any Key To Continue...   \n");
	printf("\n");

	waitForAnyKey();
	return;
}

void controls(void)
{
	int x = 10, y = 5;
	clrscr(); //clear the console
	gotoxy(x, y++);
	printf("Controls\n");
	gotoxy(x++, y++);
	printf("Use the following arrow keys to direct the snake to the food: ");
	gotoxy(x, y++);
	printf("Right Arrow");
	gotoxy(x, y++);
	printf("Left Arrow");
	gotoxy(x, y++);
	printf("Top Arrow");
	gotoxy(x, y++);
	printf("Bottom Arrow");
	gotoxy(x, y++);
	gotoxy(x, y++);
	printf("P & Esc pauses the game.");
	gotoxy(x, y++);
	gotoxy(x, y++);
	printf("Press any key to continue...");
	waitForAnyKey();
	return;
}

void exitYN(void)
{
	char pressed;
	gotoxy(9, 8);
	printf("Are you sure you want to exit(Y/N)\n");

	do
	{
		pressed = waitForAnyKey();
		pressed = (char)tolower(pressed);
	} while (!(pressed == 'y' || pressed == 'n'));

	if (pressed == 'y')
	{
		clrscr(); //clear the console
		exit(1);
	}
	return;
}

int mainMenu(void)
{
	int x = 10, y = 5;
	int yStart = y;

	int selected;

	clrscr(); //clear the console
	//Might be better with arrays of strings???
	gotoxy(x, y++);
	printf("New Game\n");
	gotoxy(x, y++);
	printf("High Scores\n");
	gotoxy(x, y++);
	printf("Controls\n");
	gotoxy(x, y++);
	printf("Exit\n");
	gotoxy(x, y++);

	selected = menuSelector(x, y, yStart);

	return(selected);
}

//**************END MENU STUFF**************//

int main() //Need to fix this up
{

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
	} while (1);   //

	return(0);
}