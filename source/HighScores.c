

#include "HighScores.h"
#include "screen.h"

//최고점 1~5위를 저장할 txt 파일 생성
void createHighScores()
{
	FILE* wfile;
	int i;

	fopen_s(&wfile, "highscores.txt", "w");

	if (wfile == NULL) //파일 열기 실패
	{
		printf("파일 생성을 실패했습니다.");
		exit(0);
	}

	//highscores.txt 파일에 해당 숫자와 문자열 저장
	for (i = 0; i < 5; i++)
	{
		fprintf(wfile, "%d", i + 1);
		fprintf(wfile, "%s", "\t0\t\t\tEMPTY\n");
	}

	fclose(wfile);
	return;
}

//str 점수 부분을 문자형에서 정수형으로 변환한 후, 점수 반환
int str_to_int_score(int len, char *str)
{
	int i;
	double sc = 0;

	for (i = 0; i < len; i++)
	{
		sc = sc + ((int)str[2 + i] - 48)*pow(10, len - i - 1);
	}

	return (int)sc;
}

//txt 파일에 저장되어 있는 점수들 중 가장 낮은 점수 반환
int getLowestScore()
{
	FILE* rfile;
	char str[128];
	int lowscore = 0;
	int i;

	fopen_s(&rfile, "highscores.txt", "r");
	if (rfile == NULL) //파일 열기 실패
	{
		createHighScores(); //새로운 txt 파일 생성
		fopen_s(&rfile, "highscores.txt", "r");
		if (rfile == NULL)
		{
			exit(1);
		}
	}

	//txt 파일을 한 줄씩 str에 저장
	while (!feof(rfile))
	{
		fgets(str, 126, rfile);
	}
	fclose(rfile);

	i = 0;
	while (str[2 + i] != '\t')
	{
		i++;
	}
	lowscore = str_to_int_score(i, str);

	return lowscore;
}

//새로운 점수와 txt 파일의 1~5위까지 점수를 비교한 후, 갱신 
void inputScore(int score)
{
	int xp = xpos, yp = ypos;
	HighScoreList hslist[5]; //1~5위까지 점수와 이름을 저장할 구조체
	FILE* rfile; //r 전용 파일 포인터
	FILE* wfile; //w+ 전용 파일 포인터
	char str[128];
	int fscore; //txt 파일에 저장된 점수
	char uname[20]; //입력받을 user name
	char fname[20]; //txt 파일에 저장된 이름
	int i, s, x;
	int entered = 0; //이미 점수가 hslist에 입력되었는지 확인

	clrscr(); //콘솔 화면 초기화

	fopen_s(&rfile, "highscores.txt", "r");
	if (rfile == NULL) //파일 열기 실패
	{
		createHighScores(); //새로운 파일 생성
		fopen_s(&rfile, "highscores.txt", "r");
		if (rfile == NULL)
		{
			exit(1);
		}
	}

	//해당 위치에 문구 출력
	gotoxy(xp, yp);
	printf("5위 안에 드는 점수를 달성했습니다!!!");
	gotoxy(xp, ++yp);
	printf("이름을 입력해주세요: ");

	gets_s(uname, sizeof(uname)); //user name 입력

	x = 0;
	while (!feof(rfile))
	{
		//txt 파일을 한 줄씩 str에 저장
		fgets(str, 126, rfile);

		//txt 파일에 저장된 점수
		i = 0;
		while (str[2 + i] != '\t')
		{
			i++;
		}
		fscore = str_to_int_score(i, str);

		//txt 파일에 저장된 이름
		s = i;
		i = 0;
		while (str[5 + s] != '\n')
		{
			fname[i] = str[5 + s];
			i++;
			s++;
		}
		fname[i] = '\0';

		//user 점수와 txt 파일에 저장된 점수 비교
		if (score >= fscore && entered != 1)
		{
			hslist[x].hscore = score;
			strcpy_s(hslist[x].hsname, sizeof(hslist[x].hsname), uname);
			entered = 1;
			x++;
		}
		if (x < 5)
		{
			hslist[x].hscore = fscore;
			strcpy_s(hslist[x].hsname, sizeof(hslist[x].hsname), fname);
		}
		else //5개의 점수와 이름을 입력받은 후, 반복문 정지
		{
			break;
		}
		
		x++;

		//txt 파일에 저장된 이름을 저장할 변수 초기화
		strcpy_s(fname, sizeof(fname), "NULL");
	}
	fclose(rfile);

	//txt 파일을 초기화한 후, 구조체에 저장된 점수와 이름 입력
	fopen_s(&wfile, "highscores.txt", "w+");
	for (i = 0; i < 5; i++)
	{
		fprintf(wfile, "%d\t%d\t\t\t%s\n", i + 1, hslist[i].hscore, hslist[i].hsname);
	}
	fclose(wfile);
	return;
}

//txt 파일에 저장된 1~5위까지의 점수와 이름을 화면에 출력
void displayHighScores()
{
	FILE* rfile;
	char str[128];
	int xp=xpos, yp = display_ypos;
	clrscr(); //콘솔 화면 초기화

	fopen_s(&rfile, "highscores.txt", "r");
	if (rfile == NULL) //파일 열기 실패
	{
		createHighScores(); //새로운 파일 생성
		fopen_s(&rfile, "highscores.txt", "r");
		if (rfile == NULL)
		{
			exit(1);
		}
	}

	//해당 위치에 문구 출력
	gotoxy(xp, yp++);
	printf("High Scores");
	gotoxy(xp, yp++);
	printf("Rank\tScore\t\t\tName");

	//txt 파일을 한 줄씩 출력
	while (!feof(rfile))
	{
		gotoxy(xp, yp++);
		if (fgets(str, 126, rfile))
		{
			printf("%s", str);
		}
	}
	fclose(rfile);

	//해당 위치에 문구 출력
	gotoxy(xp, yp++);
	printf("아무 버튼이나 눌러서 돌아가세요...");
	waitForAnyKey(); //키보드 값 입력
	clrscr(); //Issue #2 수정, menu 재선택 시 화면 클리어 문제(JooYoung)
	return;
}
