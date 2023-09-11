#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <random>
#include <Windows.h>

using namespace std;

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> dis(1, 4);

struct ARR
{
	char a{};
	bool O = false;
};

int main()
{
	ARR arr[30][30];
	ARR arr2[30][30];
	ARR arr3[30][30];

	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			arr[i][j].a = '.';
		}
	}
	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			arr2[i][j].a = '.';
		}
	}

	char order;
	int x1, x2, y1, y2;

	cout << "좌표를 입력하세요. (ex : 1 15 20 17 / 주의 : 두 번째 좌표가 첫 좌표보다 커야함) : ";
	cin >> x1 >> y1 >> x2 >> y2;

	int top = y1;
	int	bottom = y2;
	int left = x1;
	int right = x2;

	int top2, bottom2, left2, right2;
	bool appear1 = false, appear2 = false;
	int X, Y;

	X = x2 - x1;
	Y = y2 - y1;

	//출력
	/*for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			cout << arr[i][j].a << ' ';
		}
		cout << endl;
	}*/

	for (int i = x1; i < x2; i++)
	{
		for (int j = y1; j < y2; j++)
		{
			arr[i][j].a = 'O';
			arr[i][j].O = true;
		}
	}

	while (1)
	{
		system("cls");

		for (int i = 0; i < 30; i++)
		{
			for (int j = 0; j < 30; j++)
			{
				if (arr2[i][j].a == 'O')
				{
					arr[i][j] = arr2[i][j];
				}
			}
		}

		for (int i = 0; i < 30; i++)
		{
			for (int j = 0; j < 30; j++)
			{
				cout << arr[i][j].a << ' ';
			}
			cout << endl;
		}

		cout << "명령어를 입력하세요. : ";
		cin >> order;

		switch (order)
		{
		case 'w':
			if (top >= 1)
			{
				if (appear2)
				{
					top2 -= 1;
					bottom2 -= 1;
				}
				top -= 1;
				bottom -= 1;
			}
			
			if (top < 1)
			{
				bottom -= 1;
				if (appear2)
				{
					top2 -= 1;
					if ((bottom2 - top2) == Y)
					{
						bottom2 -= 1;

					}
				}
				if (!appear2)
				{
					appear2 = true;
					top2 = 28;
					bottom2 = 29;
					left2 = left;
					right2 = right;
				}
			}
			
			for (int i = 0; i < 30; i++)
			{
				for (int j = 0; j < 30; j++)
				{
					arr[i][j].a = '.';
				}
			}

			for (int i = top; i <= bottom; i++)
			{
				for (int j = left; j <= right; j++)
				{
					arr[i][j].a = 'O';
				}
			}

			if (appear2)
			{
				for (int i = top2; i <= bottom2; i++)
				{
					for (int j = left2; j <= right2; j++)

					{
						arr2[i][j].a = 'O';
					}
				}
			}
			
			break;
		case 'a':
			break;
		case 's':
			break;
		case 'd':
			break;
		}
	}
}