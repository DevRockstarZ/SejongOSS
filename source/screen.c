#include "screen.h"
#include "HighScores.h"
#include "inGame.h"

void gotoxy(int x, int y)
{
	printf("%c[%d;%df", 0x1B, y, x);
} // 전달받은 x,y 값으로 콘솔창 커서 이동

void clrscr(void)
{
	system("cls");
} // console 화면을 clear해주는 함수

void clearArrow(int x, int y)
{
	gotoxy(x, y);
	printf(" ");
	return;
} // 메인메뉴 옵션 화살표 clear function

void printArrow(int x, int y)
{
	gotoxy(x, y);
	printf(">");
	return;
}// 메인메뉴 옵션 화살표 print function

void mainMenu(void)
{
	int exit = FALSE;
	system("mode con cols=80 lines=24"); // command console 크기 변경
	welcomeArt();
	clrscr(); //clear the console
	do // do-while loop until exit is TRUE
	{
		switch (menuSelector())
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
			exit = exitYN(); // 나갈지 여부를 받음.
			break;
		default:
			break;
		}
	} while (exit != TRUE);

	return; // if returns, goto main, and ends.
}

char waitForAnyKey(void)
{
	int pressed;
	pressed = _getch(); // keyboard 입력값을 받아옴
	return ((char)pressed);
} // 키보드에 어떤 값이 입력되었는지 받아오는 함수. return : 누른 키보드 값

void pauseMenu(void)
{
	char *str[2] = { "**게임 정지**","            " };
	int i;
	for (i = 0; i < 2; i++)
	{
		gotoxy(pauseX, pauseY);
		if (i == 0)
		{
			waitForAnyKey();
		}
		printf("%s", str[i]);
	} // string을 배열에 넣고 for문으로 loop하며 문자열 출력으로 변경
	return;
} // 사용자가 게임을 정지 하였을때 일시 정지 하는 함수

void refreshInfoBar(int score)
{
	char str[2][50] = { "","Sejong Univ OSS Project : SnakeGame" };
	int i;
	sprintf_s(str[0], sizeof(str[0]), "점수 : %d", score);
	for (i = 0; i < 2; i++)
	{
		if (i == 0) gotoxy(refreshLeftX, refreshLeftY); // 좌측 하단으로 이동
		else if (i == 1) gotoxy(refreshRightX, refreshRightY); // 우측 하단으로 이동
		printf("%s", str[i]);
	} // for loop으로 string 출력 간소화

	return;
} // x,y를 콘솔창 하단으로 이동하여 플레이어의 점수, 게임속도 출력

void youWinScreen(void)
{
	int yPos = youwinY;
	int i;
	char str[8][70] = {
		"'##:::'##::'#######::'##::::'##::::'##:::::'##:'####:'##::: ##:'####:",
		". ##:'##::'##.... ##: ##:::: ##:::: ##:'##: ##:. ##:: ###:: ##: ####:",
		":. ####::: ##:::: ##: ##:::: ##:::: ##: ##: ##:: ##:: ####: ##: ####:",
		"::. ##:::: ##:::: ##: ##:::: ##:::: ##: ##: ##:: ##:: ## ## ##:: ##::",
		"::: ##:::: ##:::: ##: ##:::: ##:::: ##: ##: ##:: ##:: ##. ####::..:::",
		"::: ##:::: ##:::: ##: ##:::: ##:::: ##: ##: ##:: ##:: ##:. ###:'####:",
		"::: ##::::. #######::. #######:::::. ###. ###::'####: ##::. ##: ####:",
		":::..::::::.......::::.......:::::::...::...:::....::..::::..::....::" };
	for (i = 0; i < 8; i++)
	{
		gotoxy(youwinX, yPos++);
		printf("%s", str[i]);
	}
	waitForAnyKey(); // 사용자의 입력 응답을 받음 
	clrscr(); // Console 화면 Clear
	return;
}

void welcomeArt(void)
{
	int i;
	int yPos = welcomeY; // 초기 screen x,y value
	char str[14][50] = {
		"    _________         _________          ",
		"   /          )      /         )          ",
		"  /  /~~~~~|  |     /  /~~~~~)  |          ",
		"  |  |     |  |     |  |     |  |          ",
		"  |  |     |  |     |  |     |  |          ",
		"  |  |     |  |     |  |     |  |          /",
		"  |  |     |  |     |  |     |  |        //",
		" (o  o)    (  |_____/  /     (  (______/ /",
		"  |__/      (         /       (         /",
		"    |        ~~~~~~~~~         ~~~~~~~~",
		"    ^                                 ",
		"      Classic Retro Snake Game!         ",
		"           시작하려면 아무키나 누르세요..   "
	};
	clrscr(); //clear the console
	for (i = 0; i < 14; i++)
	{
		gotoxy(welcomeX, yPos++);
		printf("%s", str[i]);
	}
	waitForAnyKey();
	return;
} // 복잡한 screen art for loop으로 정리

void gameOverScreen(void)
{
	int yPos = gameOverY; // 초기 screen x,y value
	int i;
	clrscr();
	char str[16][50] = {
		":'######::::::'###::::'##::::'##:'########:",
		"'##... ##::::'## ##::: ###::'###: ##.....::",
		" ##:::..::::'##:. ##:: ####'####: ##:::::::",
		" ##::'####:'##:::. ##: ## ### ##: ######:::",
		" ##::: ##:: #########: ##. #: ##: ##...::::",
		" ##::: ##:: ##.... ##: ##:.:: ##: ##:::::::",
		". ######::: ##:::: ##: ##:::: ##: ########:",
		":......::::..:::::..::..:::::..::........::",
		":'#######::'##::::'##:'########:'########::'####:",
		"'##.... ##: ##:::: ##: ##.....:: ##.... ##: ####:",
		" ##:::: ##: ##:::: ##: ##::::::: ##:::: ##: ####:",
		" ##:::: ##: ##:::: ##: ######::: ########::: ##::",
		" ##:::: ##:. ##:: ##:: ##...:::: ##.. ##::::..:::",
		" ##:::: ##::. ## ##::: ##::::::: ##::. ##::'####:",
		". #######::::. ###:::: ########: ##:::. ##: ####:",
		":.......::::::...:::::........::..:::::..::....::"
	};
	for (i = 0; i < 16; i++)
	{
		gotoxy(gameOverX, yPos++);
		printf("%s", str[i]);
	}
	waitForAnyKey();
	clrscr(); //clear the console
	return;
}

BOOL exitYN(void)
{
	char pressed;
	gotoxy(exitX, exitY);
	printf(" 게임을 종료하시겠습니까? (Y/N)\n");
	do
	{
		pressed = waitForAnyKey();
		pressed = (char)tolower(pressed); // 소문자 입력할수도 있음
	} while (!(pressed == 'y' || pressed == 'n'));

	if (pressed == 'y')
	{
		clrscr(); //clear the console
		return TRUE;
	}
	return FALSE;
}

void controls(void)
{
	int yPos = controlY;
	int i;
	clrscr(); //clear the console
	char str[4][50] = {
		"            ** 게임 조작법 **",
		"- 키보드 상하좌우 키로 Snake를 움직입니다.",
		"- P또는 ESC키로 게임을 일시중지 할 수 있습니다.",
		"아무버튼이나 눌러서 돌아가세요..."
	};
	for (i = 0; i < 4; i++)
	{
		gotoxy(controlX, yPos++);
		gotoxy(controlX, yPos++);
		printf("%s", str[i]);
	}

	waitForAnyKey(); //입력 대기
	clrscr();
	return;
}

int * moveArrow(int x,int y,int option,int KEY)
{
	int *arr = (int *)malloc(sizeof(int) * 2);
	clearArrow(x, y);
	if (KEY == (char)UP_ARROW)
	{
		if (y == optionYStart)
		{
			y = optionYEnd;
			option = optionExit;
		}
		else
		{
			option = option - 1;
			y--;
		}
		printArrow(x, y);
	}
	else if (KEY == (char)DOWN_ARROW)
	{
		if (y == optionYEnd)
		{
			y = optionYStart;
			option = optionLoadGame;
		}
		else
		{
			option = option + 1;
			y++;
		}
		printArrow(x, y);
	}
	arr[0] = option;
	arr[1] = y;
	return arr;
}

int menuSelector(void)
{
	char key;
	int *arr;
	char *str[4] = { "새로운 게임","최고 점수","조작법","게임 종료" }; // array string으로 변경함.
	int option = noOption;
	int x = optionXStart;
	int y = optionYStart;
	int i;
	int printY = printYMenu;
	for (i = 0; i < 4; i++)
	{
		gotoxy(printXMenu, printY++);
		printf("%s", str[i]);
	}
	printArrow(x, y);
	do
	{
		key = waitForAnyKey();
		arr = moveArrow(x, y, option, key);
		option = arr[0];
		y = arr[1];
	} while (key != (char)ENTER_KEY); // 엔터키 입력 전까지 LOOP
	return (option);
}
