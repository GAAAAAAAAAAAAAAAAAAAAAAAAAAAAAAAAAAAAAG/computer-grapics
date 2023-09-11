#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <random>
#include <Windows.h>

using namespace std;

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> dis(1, 4);

int main()
{
	char arr[30][30];
	char nemo[100][100];

	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			arr[i][j] = '.';
			nemo[i][j] = '.';
		}
	}

	int x1, x2, y1, y2;

	cout << "좌표를 입력하세요 (두 번째 좌표가 첫번째 좌표보다 커야함) : ";
	cin >> x1 >> y1 >> x2 >> y2;

	for (int i = y1; i < y2; i++)
	{
		for (int j = x1; j < x2; j++)
		{
			nemo[i][j] = 'O';
		}
	}

	int X = x2 - x1;
	int Y = y2 - y1;

	int top = y1;
	int bottom = y2;
	int left = x1;
	int right = x2;

	int top2 = y1;
	int bottom2 = y2;
	int left2 = x1;
	int right2 = x2;

	bool Wappear = false;
	bool Aappear = false;
	bool Sappear = false;
	bool Dappear = false;

	char order;
	while (1)
	{
		//출력
		for (int i = 0; i < 30; i++)
		{
			for (int j = 0; j < 30; j++)
			{
				arr[i][j] = '.';
			}
		}

		for (int i = 0; i < 30; i++)
		{
			for (int j = 0; j < 30; j++)
			{
				if (nemo[i][j] == 'O')
				{
					arr[i][j] = nemo[i][j];
				}
			}
		}

		for (int i = 0; i < 30; i++)
		{
			for (int j = 0; j < 30; j++)
			{
				cout << arr[i][j] << ' ';
			}
			cout << endl;
		}

		cout << "명령어 입력 : ";
		cin >> order;

		switch (order)
		{
		case 'w':
			top -= 1;
			bottom -= 1;

			if (top <= 0)
			{
				if (Wappear)
				{
					top2 -= 1;
					if (bottom <= 0)
					{
						bottom2 -= 1;
					}
				}
				else if (!Wappear)
				{
					top2 = 29;
					bottom2 = 30;
					Wappear = true;
				}
			}

			//초기화
			for (int i = 0; i < 100; i++)
			{
				for (int j = 0; j < 100; j++)
				{
					nemo[i][j] = '.';
				}
			}

			//값 넣기
			for (int i = top; i < bottom; i++)
			{
				for (int j = left; j < right; j++)
				{
					nemo[i][j] = 'O';
				}
			}

			if (Wappear)
			{
				for (int i = top2; i < bottom2; i++)
				{
					for (int j = left2; j < right2; j++)
					{
						nemo[i][j] = 'O';
					}
				}
			}
			break;
		case 'a':
			left -= 1;
			right -= 1;

			if (left <= 0)
			{
				if (Aappear)
				{
					left2 -= 1;
					if (right <= 0)
					{
						right2 -= 1;
					}
				}
				else if (!Aappear)
				{
					left2 = 30;
					right2 = 31;
					Aappear = true;
				}
			}

			//초기화
			for (int i = 0; i < 100; i++)
			{
				for (int j = 0; j < 100; j++)
				{
					nemo[i][j] = '.';
				}
			}

			//값 넣기
			for (int i = top; i < bottom; i++)
			{
				for (int j = left; j < right; j++)
				{
					nemo[i][j] = 'O';
				}
			}

			if (Aappear)
			{
				for (int i = top2; i < bottom2; i++)
				{
					for (int j = left2; j < right2; j++)
					{
						nemo[i][j] = 'O';
					}
				}
			}
			break;
		case 's':
			break;
		case 'd':
			break;
		}
	}
}