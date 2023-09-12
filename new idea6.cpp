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
	char nemo[300][300];
	
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

	int top3 = y1;
	int bottom3 = y2;
	int left3 = x1;
	int right3 = x2;

	int top4 = y1;
	int bottom4 = y2;
	int left4 = x1;
	int right4 = x2;

	int top5 = y1;
	int bottom5 = y2;
	int left5 = x1;
	int right5 = x2;

	bool Wappear = false;
	bool Aappear = false;
	bool Sappear = false;
	bool Dappear = false;

	bool Wappear2 = false;
	bool Aappear2 = false;
	bool Sappear2 = false;
	bool Dappear2 = false;

	bool Wappear3 = false;
	bool Aappear3 = false;
	bool Sappear3 = false;
	bool Dappear3 = false;

	bool Wappear4 = false;
	bool Aappear4 = false;
	bool Sappear4 = false;
	bool Dappear4 = false;

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

			top3 = y1;
			bottom3 = y2;
			left3 = x1;
			right3 = x2;

			top4 = y1;
			bottom4 = y2;
			left4 = x1;
			right4 = x2;

			top5 = y1;
			bottom5 = y2;
			left5 = x1;
			right5 = x2;

			Wappear = false;
			Aappear = false;
			Sappear = false;
			Dappear = false;

			Wappear2 = false;
			Aappear2 = false;
			Sappear2 = false;
			Dappear2 = false;

			Wappear3 = false;
			Aappear3 = false;
			Sappear3 = false;
			Dappear3 = false;

			Wappear4 = false;
			Aappear4 = false;
			Sappear4 = false;
			Dappear4 = false;
			break;
		case 'X':
			left -= 1;
			right += 1;
			left2 -= 1;
			right2 += 1;
			left3 -= 1;
			right3 += 1;
			left4 -= 1;
			right4 += 1;
			left5 -= 1;
			right5 += 1;

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

			for (int i = top3; i < bottom3; i++)
			{
				for (int j = left3; j < right3; j++)
				{
					nemo[i][j] = 'O';
				}
			}

			for (int i = top4; i < bottom4; i++)
			{
				for (int j = left4; j < right4; j++)
				{
					nemo[i][j] = 'O';
				}
			}

			for (int i = top5; i < bottom5; i++)
			{
				for (int j = left5; j < right5; j++)
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
			left3 += 1;
			right3 -= 1;
			left4 += 1;
			right4 -= 1;
			left5 += 1;
			right5 -= 1;

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

			for (int i = top3; i < bottom3; i++)
			{
				for (int j = left3; j < right3; j++)
				{
					nemo[i][j] = 'O';
				}
			}

			for (int i = top4; i < bottom4; i++)
			{
				for (int j = left4; j < right4; j++)
				{
					nemo[i][j] = 'O';
				}
			}

			for (int i = top5; i < bottom5; i++)
			{
				for (int j = left5; j < right5; j++)
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
			top3 -= 1;
			bottom3 += 1;
			top4 -= 1;
			bottom4 += 1;
			top5 -= 1;
			bottom5 += 1;

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

			for (int i = top3; i < bottom3; i++)
			{
				for (int j = left3; j < right3; j++)
				{
					nemo[i][j] = 'O';
				}
			}

			for (int i = top4; i < bottom4; i++)
			{
				for (int j = left4; j < right4; j++)
				{
					nemo[i][j] = 'O';
				}
			}

			for (int i = top5; i < bottom5; i++)
			{
				for (int j = left5; j < right5; j++)
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
			top3 += 1;
			bottom3 -= 1;
			top4 += 1;
			bottom4 -= 1;
			top5 += 1;
			bottom5 -= 1;

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

			for (int i = top3; i < bottom3; i++)
			{
				for (int j = left3; j < right3; j++)
				{
					nemo[i][j] = 'O';
				}
			}

			for (int i = top4; i < bottom4; i++)
			{
				for (int j = left4; j < right4; j++)
				{
					nemo[i][j] = 'O';
				}
			}

			for (int i = top5; i < bottom5; i++)
			{
				for (int j = left5; j < right5; j++)
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

			top3 -= 1;
			bottom3 += 1;
			left3 -= 1;
			right3 += 1;

			top4 -= 1;
			bottom4 += 1;
			left4 -= 1;
			right4 += 1;

			top5 -= 1;
			bottom5 += 1;
			left5 -= 1;
			right5 += 1;

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

			for (int i = top3; i < bottom3; i++)
			{
				for (int j = left3; j < right3; j++)
				{
					nemo[i][j] = 'O';
				}
			}

			for (int i = top4; i < bottom4; i++)
			{
				for (int j = left4; j < right4; j++)
				{
					nemo[i][j] = 'O';
				}
			}

			for (int i = top5; i < bottom5; i++)
			{
				for (int j = left5; j < right5; j++)
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

			top3 += 1;
			bottom3 -= 1;
			left3 += 1;
			right3 -= 1;

			top4 += 1;
			bottom4 -= 1;
			left4 += 1;
			right4 -= 1;

			top5 += 1;
			bottom5 -= 1;
			left5 += 1;
			right5 -= 1;

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

			for (int i = top3; i < bottom3; i++)
			{
				for (int j = left3; j < right3; j++)
				{
					nemo[i][j] = 'O';
				}
			}

			for (int i = top4; i < bottom4; i++)
			{
				for (int j = left4; j < right4; j++)
				{
					nemo[i][j] = 'O';
				}
			}

			for (int i = top5; i < bottom5; i++)
			{
				for (int j = left5; j < right5; j++)
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
			//3
			if (Aappear2 || Sappear2 || Dappear2)
			{
				top3 -= 1;
				bottom3 -= 1;

			}
			else if (top2 <= 0)
			{
				if (Wappear2)
				{
					top3 -= 1;
					if (bottom2 <= 0)
					{
						bottom3 -= 1;
					}
				}
				else if (!Wappear2)
				{
					left3 = left2;
					right3 = right2;
					top3 = 29;
					bottom3 = 30;
					Wappear2 = true;
				}
			}

			if (Aappear3 || Sappear3 || Dappear3)
			{
				top4 -= 1;
				bottom4 -= 1;

			}
			else if (top3 <= 0)
			{
				if (Wappear3)
				{
					top4 -= 1;
					if (bottom3 <= 0)
					{
						bottom4 -= 1;
					}
				}
				else if (!Wappear3)
				{
					left4 = left3;
					right4 = right3;
					top4 = 29;
					bottom4 = 30;
					Wappear3 = true;
				}
			}

			if (Aappear4 || Sappear4 || Dappear4)
			{
				top5 -= 1;
				bottom5 -= 1;

			}
			else if (top4 <= 0)
			{
				if (Wappear4)
				{
					top5 -= 1;
					if (bottom4 <= 0)
					{
						bottom5 -= 1;
					}
				}
				else if (!Wappear4)
				{
					left5 = left4;
					right5 = right4;
					top5 = 29;
					bottom5 = 30;
					Wappear4 = true;
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

			if (Wappear2 || Aappear2 || Sappear2 || Dappear2)
			{
				for (int i = top3; i < bottom3; i++)
				{
					for (int j = left3; j < right3; j++)
					{
						nemo[i][j] = 'O';
					}
				}
			}

			if (Wappear3 || Aappear3 || Sappear3 || Dappear3)
			{
				for (int i = top4; i < bottom4; i++)
				{
					for (int j = left4; j < right4; j++)
					{
						nemo[i][j] = 'O';
					}
				}
			}

			if (Wappear4 || Aappear4 || Sappear4 || Dappear4)
			{
				for (int i = top5; i < bottom5; i++)
				{
					for (int j = left5; j < right5; j++)
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


			if (Wappear2 || Sappear2 || Dappear2)
			{
				left3 -= 1;
				right3 -= 1;

			}
			else if (left2 <= 0)
			{
				if (Aappear2)
				{
					left3 -= 1;
					if (right2 <= 0)
					{
						right3 -= 1;
					}
				}
				else if (!Aappear2)
				{
					top3 = top2;
					bottom3 = bottom2;
					left3 = 30;
					right3 = 31;
					Aappear2 = true;
				}
			}

			if (Wappear3 || Sappear3 || Dappear3)
			{
				left4 -= 1;
				right4 -= 1;

			}
			else if (left3 <= 0)
			{
				if (Aappear3)
				{
					left4 -= 1;
					if (right3 <= 0)
					{
						right4 -= 1;
					}
				}
				else if (!Aappear3)
				{
					top4 = top3;
					bottom4 = bottom3;
					left4 = 30;
					right4 = 31;
					Aappear3 = true;
				}
			}

			if (Wappear4 || Sappear4 || Dappear4)
			{
				left5 -= 1;
				right5 -= 1;

			}
			else if (left4 <= 0)
			{
				if (Aappear4)
				{
					left5 -= 1;
					if (right4 <= 0)
					{
						right5 -= 1;
					}
				}
				else if (!Aappear4)
				{
					top5 = top4;
					bottom5 = bottom4;
					left5 = 30;
					right5 = 31;
					Aappear4 = true;
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
			if (Wappear2 || Aappear2 || Sappear2 || Dappear2)
			{
				for (int i = top3; i < bottom3; i++)
				{
					for (int j = left3; j < right3; j++)
					{
						nemo[i][j] = 'O';
					}
				}
			}
			if (Wappear3 || Aappear3 || Sappear3 || Dappear3)
			{
				for (int i = top4; i < bottom4; i++)
				{
					for (int j = left4; j < right4; j++)
					{
						nemo[i][j] = 'O';
					}
				}
			}
			if (Wappear4 || Aappear4 || Sappear4 || Dappear4)
			{
				for (int i = top5; i < bottom5; i++)
				{
					for (int j = left5; j < right5; j++)
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


			if (Aappear2 || Wappear2 || Dappear2)
			{
				top3 += 1;
				bottom3 += 1;
			}
			else if (bottom2 >= 30)
			{
				if (Sappear2)
				{
					bottom3 += 1;
					if (top2 >= 30)
					{
						top3 += 1;
					}
				}
				else if (!Sappear2)
				{
					left3= left2;
					right3 = right2;
					top3 = 0;
					bottom3 = 1;
					Sappear2 = true;
				}
			}

			if (Aappear3 || Wappear3 || Dappear3)
			{
				top4 += 1;
				bottom4 += 1;
			}
			else if (bottom3 >= 30)
			{
				if (Sappear3)
				{
					bottom4 += 1;
					if (top3 >= 30)
					{
						top4 += 1;
					}
				}
				else if (!Sappear3)
				{
					left4 = left3;
					right4 = right3;
					top4 = 0;
					bottom4 = 1;
					Sappear3 = true;
				}
			}

			if (Aappear4 || Wappear4 || Dappear4)
			{
				top5 += 1;
				bottom5 += 1;
			}
			else if (bottom4 >= 30)
			{
				if (Sappear4)
				{
					bottom5 += 1;
					if (top4 >= 30)
					{
						top5 += 1;
					}
				}
				else if (!Sappear4)
				{
					left5 = left4;
					right5 = right4;
					top5 = 0;
					bottom5 = 1;
					Sappear4 = true;
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
			if (Wappear2 || Aappear2 || Sappear2 || Dappear2)
			{
				for (int i = top3; i < bottom3; i++)
				{
					for (int j = left3; j < right3; j++)
					{
						nemo[i][j] = 'O';
					}
				}
			}
			if (Wappear3 || Aappear3 || Sappear3 || Dappear3)
			{
				for (int i = top4; i < bottom4; i++)
				{
					for (int j = left4; j < right4; j++)
					{
						nemo[i][j] = 'O';
					}
				}
			}
			if (Wappear4 || Aappear4 || Sappear4 || Dappear4)
			{
				for (int i = top5; i < bottom5; i++)
				{
					for (int j = left5; j < right5; j++)
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

			if (Aappear2 || Wappear2 || Sappear2)
			{
				left3+= 1;
				right3 += 1;
			}
			else if (right2 >= 30)
			{
				if (Dappear2)
				{
					right3 += 1;
					if (left2 >= 30)
					{
						left3 += 1;
					}
				}
				else if (!Dappear2)
				{
					top3 = top2;
					bottom3 = bottom2;
					left3 = 0;
					right3 = 1;
					Dappear2 = true;
				}
			}

			if (Aappear3 || Wappear3 || Sappear3)
			{
				left4 += 1;
				right4 += 1;
			}
			else if (right3 >= 30)
			{
				if (Dappear3)
				{
					right4 += 1;
					if (left3 >= 30)
					{
						left4 += 1;
					}
				}
				else if (!Dappear3)
				{
					top4 = top3;
					bottom4 = bottom3;
					left4 = 0;
					right4 = 1;
					Dappear3 = true;
				}
			}

			if (Aappear4 || Wappear4 || Sappear4)
			{
				left5 += 1;
				right5 += 1;
			}
			else if (right4 >= 30)
			{
				if (Dappear4)
				{
					right5 += 1;
					if (left4 >= 30)
					{
						left5 += 1;
					}
				}
				else if (!Dappear4)
				{
					top5 = top4;
					bottom5 = bottom4;
					left5 = 0;
					right5 = 1;
					Dappear4 = true;
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
			if (Wappear2 || Aappear2 || Sappear2 || Dappear2)
			{
				for (int i = top3; i < bottom3; i++)
				{
					for (int j = left3; j < right3; j++)
					{
						nemo[i][j] = 'O';
					}
				}
			}
			if (Wappear3 || Aappear3 || Sappear3 || Dappear3)
			{
				for (int i = top4; i < bottom4; i++)
				{
					for (int j = left4; j < right4; j++)
					{
						nemo[i][j] = 'O';
					}
				}
			}
			if (Wappear4 || Aappear4 || Sappear4 || Dappear4)
			{
				for (int i = top5; i < bottom5; i++)
				{
					for (int j = left5; j < right5; j++)
					{
						nemo[i][j] = 'O';
					}
				}
			}
			break;
		}
	}
}