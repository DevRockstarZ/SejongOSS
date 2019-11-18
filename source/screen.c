#include "screen.h"

void gotoxy(int x, int y)
{
	printf("%c[%d;%df", 0x1B, y, x);
} // ���޹��� x,y ������ �ܼ�â Ŀ�� �̵�

void clrscr(void)
{
	system("cls");
} // console ȭ���� clear���ִ� �Լ�

char waitForAnyKey(void)
{
	int pressed;

	pressed = _getch(); // keyboard �Է°��� �޾ƿ�

	return((char)pressed);
} // Ű���忡 � ���� �ԷµǾ����� �޾ƿ��� �Լ�. return : ���� Ű���� ��

void pauseMenu(void)
{
	char* str[2] = { "**���� ����**","            " };
	int i;
	for (i = 0; i < 2; i++)
	{
		gotoxy(28, 23);
		if (i == 0)
		{
			waitForAnyKey();
		}
		printf("%s", str[i]);
	} // string�� �迭�� �ְ� for������ loop�ϸ� ���ڿ� ������� ����
	return;
} // ����ڰ� ������ ���� �Ͽ����� �Ͻ� ���� �ϴ� �Լ�

void refreshInfoBar(int score, int speed)
{
	char str[2][50] = { "","Sejong Univ OSS Project : SnakeGame" };
	int i;
	sprintf_s(str[0], sizeof(str[0]), "���� : %d", score);
	for (i = 0; i < 2; i++)
	{
		if (i == 0) gotoxy(5, 23); // ���� �ϴ����� �̵�
		else if (i == 1) gotoxy(40, 23); // ���� �ϴ����� �̵�
		printf("%s", str[i]);
	} // for loop���� string ��� ����ȭ

	return;
} // x,y�� �ܼ�â �ϴ����� �̵��Ͽ� �÷��̾��� ����, ���Ӽӵ� ���

void youWinScreen(void)
{
	int x = 6, y = 8; // �ʱ� screen x,y value
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
	waitForAnyKey(); // ������� �Է� ������ ���� 
	clrscr(); // Console ȭ�� Clear
	return;
}

void welcomeArt(void)
{
	int i;
	int x = 16, y = 4; // �ʱ� screen x,y value
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
		"           �����Ϸ��� �ƹ�Ű�� ��������..   "
	};
	clrscr(); //clear the console
	for (i = 0; i < 14; i++)
	{
		gotoxy(x, y++);
		printf("%s", str[i]);
	}
	waitForAnyKey();
	return;
} // ������ screen art for loop���� ����

void gameOverScreen(void)
{
	int x = 15, y = 3; // �ʱ� screen x,y value
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
	char* option[4] = { "���ο� ����","�ְ� ����","���۹�","���� ����" }; // array string���� ������.
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
	printf(" ������ �����Ͻðڽ��ϱ�? (Y/N)\n");
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
		"            ** ���� ���۹� **",
		"- Ű���� �����¿� Ű�� Snake�� �����Դϴ�.",
		"- P�Ǵ� ESCŰ�� ������ �Ͻ����� �� �� �ֽ��ϴ�.",
		"�ƹ���ư�̳� ������ ���ư�����..."
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