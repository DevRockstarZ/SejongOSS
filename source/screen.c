#include "screen.h"

void gotoxy(int x, int y)
{
	printf("%c[%d;%df", 0x1B, y, x);
} // 전달받은 x,y 값으로 콘솔창 커서 이동

void clrscr(void)
{
	system("cls");
} // console 화면을 clear해주는 함수

char waitForAnyKey(void)
{
	int pressed;

	pressed = _getch(); // keyboard 입력값을 받아옴

	return((char)pressed);
} // 키보드에 어떤 값이 입력되었는지 받아오는 함수. return : 누른 키보드 값

void pauseMenu(void)
{
	char* str[2] = { "**게임 정지**","            " };
	int i;
	for (i = 0; i < 2; i++)
	{
		gotoxy(28, 23);
		if (i == 0)
		{
			waitForAnyKey();
		}
		printf("%s", str[i]);
	} // string을 배열에 넣고 for문으로 loop하며 문자열 출력으로 변경
	return;
} // 사용자가 게임을 정지 하였을때 일시 정지 하는 함수

void refreshInfoBar(int score, int speed)
{
	char str[2][50] = { "","Sejong Univ OSS Project : SnakeGame" };
	int i;
	sprintf_s(str[0], sizeof(str[0]), "점수 : %d", score);
	for (i = 0; i < 2; i++)
	{
		if (i == 0) gotoxy(5, 23); // 좌측 하단으로 이동
		else if (i == 1) gotoxy(40, 23); // 우측 하단으로 이동
		printf("%s", str[i]);
	} // for loop으로 string 출력 간소화

	return;
} // x,y를 콘솔창 하단으로 이동하여 플레이어의 점수, 게임속도 출력

void youWinScreen(void)
{
	int x = 6, y = 8; // 초기 screen x,y value
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
		gotoxy(x, y++);
		printf("%s", str[i]);
	}
	waitForAnyKey(); // 사용자의 입력 응답을 받음 
	clrscr(); // Console 화면 Clear
	return;
}

void welcomeArt(void)
{
	int i;
	int x = 16, y = 4; // 초기 screen x,y value
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
		gotoxy(x, y++);
		printf("%s", str[i]);
	}
	waitForAnyKey();
	return;
} // 복잡한 screen art for loop으로 정리

void gameOverScreen(void)
{
	int x = 15, y = 3; // 초기 screen x,y value
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
		gotoxy(x, y++);
		printf("%s", str[i]);
	}
	waitForAnyKey();
	clrscr(); //clear the console
	return;
}

int mainMenu(void)
{
	int x = 10, y = 5;
	int i;
	int yStart = y;
	char* option[4] = { "새로운 게임","최고 점수","조작법","게임 종료" }; // array string으로 변경함.
	int selected;

	clrscr(); //clear the console

	for (i = 0; i < 4; i++)
	{
		gotoxy(x, y++);
		printf("%s", option[i]);
	}
	gotoxy(x, y++);
	selected = menuSelector(x, y, yStart);

	return (selected);
}

void exitYN(void)
{
	char pressed;
	gotoxy(9, 8);
	printf(" 게임을 종료하시겠습니까? (Y/N)\n");
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

void controls(void)
{
	int x = 16, y = 2;
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
		gotoxy(x, y++);
		gotoxy(x, y++);
		printf("%s", str[i]);
	}
	waitForAnyKey();
	return;
}

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
	} while (key != (char)ENTER_KEY); //While doesn't equal enter... (13 ASCII code for enter) - note ubuntu is 10
	return(i);
}