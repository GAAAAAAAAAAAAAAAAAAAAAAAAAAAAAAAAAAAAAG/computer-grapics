#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <random>
#include <Windows.h>
#include <conio.h>

#define UP 72
#define DOWN 80
#define RIGHT 77
#define LEFT 75

using namespace std;

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> dis(1, 4);

struct ARR
{
	char aa;	//숨은 알파벳
	char a;	//드러난 문자
	bool iscorrect = false;
	bool isSelection = false;
	bool isFull = false;
	bool cursor = false;
};

int main()
{
	ARR arr[5][5];
	
	//초기화
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			arr[i][j].a = '*';
		}
	}

	arr[0][0].a = ' ';
	arr[0][1].a = 'a';
	arr[0][2].a = 'b';
	arr[0][3].a = 'c';
	arr[0][4].a = 'd';

	arr[1][0].a = '1';
	arr[2][0].a = '2';
	arr[3][0].a = '3';
	arr[4][0].a = '4';

	//랜덤 문자 넣기
	int M, K;
	int put = 0;	//0,1 = 'A' 2,3 = 'B', 4,5 = 'C'...
	while (put != 16)
	{
		M = dis(gen);
		K = dis(gen);

		if (put < 2 && !arr[M][K].isFull)
		{
			arr[M][K].aa = 'A';
			arr[M][K].isFull = true;
			put++;
		}
		if (put >= 2 && put < 4 && !arr[M][K].isFull)
		{
			arr[M][K].aa = 'B';
			arr[M][K].isFull = true;
			put++;
		}
		if (put >= 4 && put < 6 && !arr[M][K].isFull)
		{
			arr[M][K].aa = 'C';
			arr[M][K].isFull = true;
			put++;
		}
		if (put >= 6 && put < 8 && !arr[M][K].isFull)
		{
			arr[M][K].aa = 'D';
			arr[M][K].isFull = true;
			put++;
		}
		if (put >= 8 && put < 10 && !arr[M][K].isFull)
		{
			arr[M][K].aa = 'E';
			arr[M][K].isFull = true;
			put++;
		}
		if (put >= 10 && put < 12 && !arr[M][K].isFull)
		{
			arr[M][K].aa = 'F';
			arr[M][K].isFull = true;
			put++;
		}
		if (put >= 12 && put < 14 && !arr[M][K].isFull)
		{
			arr[M][K].aa = 'G';
			arr[M][K].isFull = true;
			put++;
		}
		if (put >= 14 && put < 16 && !arr[M][K].isFull)
		{
			arr[M][K].aa = 'H';
			arr[M][K].isFull = true;
			put++;
		}
	}

	/*for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			cout <<' ' << arr[i][j].a;
		}
		cout << endl;
	}*/

	char input;
	int I = 1;
	int J = 1;
	int selection = 30; //0되면 게임오버
	int score = 0;

	//임시 저장
	char c1 = ' ', c2 = ' ';
	int i1, i2, i3, i4;

	while (1)
	{
		system("cls");
		//출력
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				if (arr[i][j].isSelection)
				{
					switch (arr[i][j].aa)
					{
					case 'A':
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
						break;
					case 'B':
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
						break;
					case 'C':
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
						break;
					case 'D':
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
						break;
					case 'E':
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5);
						break;
					case 'F':
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
						break;
					case 'G':
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
						break;
					case 'H':
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
						break;

					}
					
					cout << ' ' << arr[i][j].a;
				}
				else
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					cout << ' ' << arr[i][j].a;
				}
			}
			cout << endl;
		}

		cout << "남은 기회 : " << selection << endl;
		cout << "점수 : " << score << endl;
		if (selection < 0)
		{
			break;
		}
		if (score == 80)
		{
			cout << "game win" << endl;
		}

		//키보드 입력 받기
		input = _getch();
		
		switch (input)
		{
		case 'r':

			for (int i = 1; i < 5; i++)
			{
				for (int j = 1; j < 5; j++)
				{
					arr[i][j].iscorrect = false;
					arr[i][j].isSelection = false;
					arr[i][j].isFull = false;
					arr[i][j].cursor = false;
				}
			}


			I = 1;
			J = 1;
			selection = 30;
			score = 0;

			//초기화
			for (int i = 0; i < 5; i++)
			{
				for (int j = 0; j < 5; j++)
				{
					arr[i][j].a = '*';
				}
			}

			arr[0][0].a = ' ';
			arr[0][1].a = 'a';
			arr[0][2].a = 'b';
			arr[0][3].a = 'c';
			arr[0][4].a = 'd';

			arr[1][0].a = '1';
			arr[2][0].a = '2';
			arr[3][0].a = '3';
			arr[4][0].a = '4';

			//랜덤 문자 넣기
			put = 0;	//0,1 = 'A' 2,3 = 'B', 4,5 = 'C'...
			while (put != 16)
			{
				M = dis(gen);
				K = dis(gen);

				if (put < 2 && !arr[M][K].isFull)
				{
					arr[M][K].aa = 'A';
					arr[M][K].isFull = true;
					put++;
				}
				if (put >= 2 && put < 4 && !arr[M][K].isFull)
				{
					arr[M][K].aa = 'B';
					arr[M][K].isFull = true;
					put++;
				}
				if (put >= 4 && put < 6 && !arr[M][K].isFull)
				{
					arr[M][K].aa = 'C';
					arr[M][K].isFull = true;
					put++;
				}
				if (put >= 6 && put < 8 && !arr[M][K].isFull)
				{
					arr[M][K].aa = 'D';
					arr[M][K].isFull = true;
					put++;
				}
				if (put >= 8 && put < 10 && !arr[M][K].isFull)
				{
					arr[M][K].aa = 'E';
					arr[M][K].isFull = true;
					put++;
				}
				if (put >= 10 && put < 12 && !arr[M][K].isFull)
				{
					arr[M][K].aa = 'F';
					arr[M][K].isFull = true;
					put++;
				}
				if (put >= 12 && put < 14 && !arr[M][K].isFull)
				{
					arr[M][K].aa = 'G';
					arr[M][K].isFull = true;
					put++;
				}
				if (put >= 14 && put < 16 && !arr[M][K].isFull)
				{
					arr[M][K].aa = 'H';
					arr[M][K].isFull = true;
					put++;
				}
			}

			break;
		case '.':
			if (!arr[I][J].isSelection)
			{
				arr[I][J].a = arr[I][J].aa;
				arr[I][J].isSelection = true;
				selection--;
				if (selection % 2 == 1)
				{
					c1 = arr[I][J].aa;
					i1 = I;
					i2 = J;
				}
				if (selection % 2 == 0)
				{
					c2 = arr[I][J].aa;
					i3 = I;
					i4 = J;
				}
			}
			
			if (selection % 2 == 0)
			{
				if (c1 == c2)
				{
					arr[i1][i2].iscorrect = true;
					arr[i3][i4].iscorrect = true;
					score += 10;
				}
				else
				{
					system("cls");
					//출력
					for (int i = 0; i < 5; i++)
					{
						for (int j = 0; j < 5; j++)
						{
							if (arr[i][j].isSelection)
							{
								switch (arr[i][j].aa)
								{
								case 'A':
									SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
									break;
								case 'B':
									SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
									break;
								case 'C':
									SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
									break;
								case 'D':
									SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
									break;
								case 'E':
									SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5);
									break;
								case 'F':
									SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
									break;
								case 'G':
									SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
									break;
								case 'H':
									SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
									break;

								}

								cout << ' ' << arr[i][j].a;
							}
							else
							{
								SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
								cout << ' ' << arr[i][j].a;
							}
						}
						cout << endl;
					}

					cout << "남은 기회 : " << selection << endl;
					cout << "점수 : " << score << endl;

					arr[i1][i2].isSelection = false;
					arr[i3][i4].isSelection = false;

					arr[i1][i2].a = '*';
					arr[i3][i4].a = '*';
					Sleep(1000);
				}
			}
			break;
		case LEFT:
			
			if (J > 1)
			{
				for (int i = 1; i < 5; i++)
				{
					for (int j = 1; j < 5; j++)
					{
						arr[i][j].cursor = false;
					}
				}
				J--;
				arr[I][J].cursor = true;
			}
			break;
		case RIGHT:
			
			if (J <4)
			{
				for (int i = 1; i < 5; i++)
				{
					for (int j = 1; j < 5; j++)
					{
						arr[i][j].cursor = false;
					}
				}
				J++;
				arr[I][J].cursor = true;
			}
			break;
		case UP:
			
			if (I > 1)
			{
				for (int i = 1; i < 5; i++)
				{
					for (int j = 1; j < 5; j++)
					{
						arr[i][j].cursor = false;
					}
				}
				I--;
				arr[I][J].cursor = true;
			}
			break;
		case DOWN:
			
			if (I < 4)
			{
				for (int i = 1; i < 5; i++)
				{
					for (int j = 1; j < 5; j++)
					{
						arr[i][j].cursor = false;
					}
				}
				I++;
				arr[I][J].cursor = true;
			}
			break;
		}

		//키보드 커서 확인
		for (int i = 1; i < 5; i++)
		{
			for (int j = 1; j < 5; j++)
			{
				if (arr[i][j].cursor && !arr[i][j].isSelection && !arr[i][j].iscorrect)
				{
					arr[i][j].a = 'O';
				}
				if (!arr[i][j].cursor && !arr[i][j].isSelection && !arr[i][j].iscorrect)
				{
					arr[i][j].a = '*';
				}
			}
		}
	}

	cout << "game over" << endl;
}