#include "screen.h"

void gotoxy(int x, int y)
{
	printf("%c[%d;%df", 0x1B, y, x);
} // ���޹��� x,y ������ �ܼ�â Ŀ�� �̵�

void clrscr(void)
{
	system("cls");
	return;
} // console ȭ���� clear���ִ� �Լ�

char waitForAnyKey(void)
{
	int pressed;

	// while (!_kbhit()); // keyboard �Է��� ������ loop�ϸ� ����Ѵ�.

	pressed = _getch(); // keyboard �Է°��� �޾ƿ�

	return((char)pressed);
} // Ű���忡 � ���� �ԷµǾ����� �޾ƿ��� �Լ�. return : ���� Ű���� ��

void pauseMenu(void)
{

	gotoxy(28, 23); // �ܼ� �ϴ����� �̵�
	printf("**Paused**");

	waitForAnyKey(); // ������� �Է��� ��ٸ�
	gotoxy(28, 23);
	printf("            ");

	return;
} // ����ڰ� ������ ���� �Ͽ����� �Ͻ� ���� �ϴ� �Լ�

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
} // x,y�� �ܼ�â �ϴ����� �̵��Ͽ� �÷��̾��� ����, ���Ӽӵ� ���

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

	waitForAnyKey(); // ������� �Է� ������ ���� 
	clrscr(); // Console ȭ�� Clear
	return;
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

