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
	int selection = 0;	//2면 초기화

	while (1)
	{
		system("cls");
		//출력
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				cout << ' ' << arr[i][j].a;
			}
			cout << endl;
		}

		//키보드 입력 받기
		input = _getch();
		
		switch (input)
		{
		case '.':
			arr[I][J].a = arr[I][J].aa;
			arr[I][J].isSelection = true;
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
				if (arr[i][j].cursor && !arr[i][j].isSelection)
				{
					arr[i][j].a = 'O';
				}
				if (!arr[i][j].cursor && !arr[i][j].isSelection)
				{
					arr[i][j].a = '*';
				}
			}
		}
	}
}