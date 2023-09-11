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
	char nemo[150][150];
	
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
		system("cls");
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
		case 'r':
			for (int i = 0; i < 30; i++)
			{
				for (int j = 0; j < 30; j++)
				{
					nemo[i][j] = '.';
				}
			}

			cout << "좌표를 입력하세요 (두 번째 좌표가 첫번째 좌표보다 커야함) : ";
			cin >> x1 >> y1 >> x2 >> y2;

			for (int i = y1; i < y2; i++)
			{
				for (int j = x1; j < x2; j++)
				{
					nemo[i][j] = 'O';
				}
			}

			X = x2 - x1;
			Y = y2 - y1;

			top = y1;
			bottom = y2;
			left = x1;
			right = x2;

			top2 = y1;
			bottom2 = y2;
			left2 = x1;
			right2 = x2;

			Wappear = false;
			Aappear = false;
			Sappear = false;
			Dappear = false;
			break;
		case 'X':
			left -= 1;
			right += 1;
			left2 -= 1;
			right2 += 1;

			for (int i = 0; i < 30; i++)
			{
				for (int j = 0; j < 30; j++)
				{
					nemo[i][j] = '.';
				}
			}

			for (int i = top; i < bottom; i++)
			{
				for (int j = left; j < right; j++)
				{
					nemo[i][j] = 'O';
				}
			}

			for (int i = top2; i < bottom2; i++)
			{
				for (int j = left2; j < right2; j++)
				{
					nemo[i][j] = 'O';
				}
			}
			break;
		case 'x':
			left += 1;
			right -= 1;
			left2 += 1;
			right2 -= 1;

			for (int i = 0; i < 30; i++)
			{
				for (int j = 0; j < 30; j++)
				{
					nemo[i][j] = '.';
				}
			}

			for (int i = top; i < bottom; i++)
			{
				for (int j = left; j < right; j++)
				{
					nemo[i][j] = 'O';
				}
			}

			for (int i = top2; i < bottom2; i++)
			{
				for (int j = left2; j < right2; j++)
				{
					nemo[i][j] = 'O';
				}
			}
			break;
		case 'Y':
			top -= 1;
			bottom += 1;
			top2 -= 1;
			bottom2 += 1;

			for (int i = 0; i < 30; i++)
			{
				for (int j = 0; j < 30; j++)
				{
					nemo[i][j] = '.';
				}
			}

			for (int i = top; i < bottom; i++)
			{
				for (int j = left; j < right; j++)
				{
					nemo[i][j] = 'O';
				}
			}

			for (int i = top2; i < bottom2; i++)
			{
				for (int j = left2; j < right2; j++)
				{
					nemo[i][j] = 'O';
				}
			}
			break;
		case 'y':
			top += 1;
			bottom -= 1;
			top2 += 1;
			bottom2 -= 1;

			for (int i = 0; i < 30; i++)
			{
				for (int j = 0; j < 30; j++)
				{
					nemo[i][j] = '.';
				}
			}

			for (int i = top; i < bottom; i++)
			{
				for (int j = left; j < right; j++)
				{
					nemo[i][j] = 'O';
				}
			}

			for (int i = top2; i < bottom2; i++)
			{
				for (int j = left2; j < right2; j++)
				{
					nemo[i][j] = 'O';
				}
			}
			break;
		case '+':
			top -= 1;
			bottom += 1;
			left -= 1;
			right += 1;

			top2 -= 1;
			bottom2 += 1;
			left2 -= 1;
			right2 += 1;

			for (int i = 0; i < 30; i++)
			{
				for (int j = 0; j < 30; j++)
				{
					nemo[i][j] = '.';
				}
			}

			for (int i = top; i < bottom; i++)
			{
				for (int j = left; j < right; j++)
				{
					nemo[i][j] = 'O';
				}
			}

			for (int i = top2; i < bottom2; i++)
			{
				for (int j = left2; j < right2; j++)
				{
					nemo[i][j] = 'O';
				}
			}
			break;
		case '-':
			top += 1;
			bottom -= 1;
			left += 1;
			right -= 1;

			top2 += 1;
			bottom2 -= 1;
			left2 += 1;
			right2 -= 1;

			for (int i = 0; i < 30; i++)
			{
				for (int j = 0; j < 30; j++)
				{
					nemo[i][j] = '.';
				}
			}

			for (int i = top; i < bottom; i++)
			{
				for (int j = left; j < right; j++)
				{
					nemo[i][j] = 'O';
				}
			}

			for (int i = top2; i < bottom2; i++)
			{
				for (int j = left2; j < right2; j++)
				{
					nemo[i][j] = 'O';
				}
			}
			break;
		case 'w':
			top -= 1;
			bottom -= 1;
			if (Aappear || Sappear || Dappear)
			{
				top2 -= 1;
				bottom2 -= 1;
				
			}
			else if (top <= 0)
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
					left2 = left;
					right2 = right;
					top2 = 29;
					bottom2 = 30;
					Wappear = true;
				}
			}

			//초기화
			for (int i = 0; i < 30; i++)
			{
				for (int j = 0; j < 30; j++)
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

			if (Wappear||Aappear||Sappear||Dappear)
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
			if (Wappear || Sappear || Dappear)
			{
				left2 -= 1;
				right2 -= 1;

			}
			else if (left <= 0)
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
					top2 = top;
					bottom2 = bottom;
					left2 = 30;
					right2 = 31;
					Aappear = true;
				}
			}

			//초기화
			for (int i = 0; i < 30; i++)
			{
				for (int j = 0; j < 30; j++)
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

			if (Wappear || Aappear || Sappear || Dappear)
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
			top += 1;
			bottom += 1;
			if (Aappear || Wappear || Dappear)
			{
				top2 += 1;
				bottom2 += 1;
			}
			else if (bottom >= 30)
			{
				if (Sappear)
				{
					bottom2 += 1;
					if (top >= 30)
					{
						top2 += 1;
					}
				}
				else if (!Sappear)
				{
					left2 = left;
					right2 = right;
					top2 = 0;
					bottom2 = 1;
					Sappear = true;
				}
			}

			//초기화
			for (int i = 0; i < 30; i++)
			{
				for (int j = 0; j < 30; j++)
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

			if (Wappear || Aappear || Sappear || Dappear)
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
		case 'd':
			left += 1;
			right += 1;
			if (Aappear || Wappear || Sappear)
			{
				left2 += 1;
				right2 += 1;
			}
			else if (right >= 30)
			{
				if (Dappear)
				{
					right2 += 1;
					if (left >= 30)
					{
						left2 += 1;
					}
				}
				else if (!Dappear)
				{
					top2 = top;
					bottom2 = bottom;
					left2 = 0;
					right2 = 1;
					Dappear = true;
				}
			}
			//초기화
			for (int i = 0; i < 30; i++)
			{
				for (int j = 0; j < 30; j++)
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

			if (Wappear || Aappear || Sappear || Dappear)
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
		}
	}
}