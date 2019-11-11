#include "screen.h"

void gotoxy(int x, int y)
{
	printf("%c[%d;%df", 0x1B, y, x);
} // 전달받은 x,y 값으로 콘솔창 커서 이동

void clrscr()
{
	system("cls");
	return;
} // console 화면을 clear해주는 함수

char waitForAnyKey(void)
{
	int pressed;

	// while (!_kbhit()); // keyboard 입력이 없으면 loop하며 대기한다.

	pressed = _getch(); // keyboard 입력값을 받아옴

	return((char)pressed);
} // 키보드에 어떤 값이 입력되었는지 받아오는 함수. return : 누른 키보드 값

void pauseMenu(void)
{

	gotoxy(28, 23); // 콘솔 하단으로 이동
	printf("**Paused**");

	waitForAnyKey(); // 사용자의 입력을 기다림
	gotoxy(28, 23);
	printf("            ");

	return;
} // 사용자가 게임을 정지 하였을때 일시 정지 하는 함수

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
} // x,y를 콘솔창 하단으로 이동하여 플레이어의 점수, 게임속도 출력

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

	waitForAnyKey(); // 사용자의 입력 응답을 받음 
	clrscr(); // Console 화면 Clear
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

