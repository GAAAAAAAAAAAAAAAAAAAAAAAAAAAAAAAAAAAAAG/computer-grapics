#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <random>
#include <Windows.h>

using namespace std;

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> dis(5, 44);

struct ARR
{
	char c = '1';
	int i = 0;
	bool point = false;
};

int main()
{
	ARR arr[50][50];

	int cnt = 0;
	int I = 0, J = 0;

	int turn = dis(gen);

	bool sniffling = true;
	bool change = false;
	bool change2 = true;

	while (arr[49][49].c == '1')
	{
		arr[I][J].c = '0';
		arr[I][J].i = cnt;
		if (J == turn && change2)
		{
			change = true;
			change2 = false;
		}
		else if (change)
		{
			//하드코딩 시작!!
			J++;
			cnt++;
			arr[I][J].c = '0';
			arr[I][J].i = cnt;
			

			J++;
			cnt++;
			arr[I][J].c = '0';
			arr[I][J].i = cnt;

			I--;
			cnt++;
			arr[I][J].c = '0';
			arr[I][J].i = cnt;

			I--;
			cnt++;
			arr[I][J].c = '0';
			arr[I][J].i = cnt;

			J++;
			cnt++;
			arr[I][J].c = '0';
			arr[I][J].i = cnt;

			J++;
			cnt++;
			arr[I][J].c = '0';
			arr[I][J].i = cnt;

			J++;
			cnt++;
			arr[I][J].c = '0';
			arr[I][J].i = cnt;

			I++;
			cnt++;
			arr[I][J].c = '0';
			arr[I][J].i = cnt;

			I++;
			cnt++;
			arr[I][J].c = '0';
			arr[I][J].i = cnt;

			I++;
			cnt++;
			arr[I][J].c = '0';
			arr[I][J].i = cnt;

			J--;
			cnt++;
			arr[I][J].c = '0';
			arr[I][J].i = cnt;

			J--;
			cnt++;
			arr[I][J].c = '0';
			arr[I][J].i = cnt;

			I++;
			cnt++;
			arr[I][J].c = '0';
			arr[I][J].i = cnt;

			I++;
			cnt++;
			arr[I][J].c = '0';
			arr[I][J].i = cnt;

			change = !change;
		}
		else if (!change)
		{
			if (sniffling)
			{
				J++;
			}
			else if (!sniffling)
			{
				I++;
			}
			cnt++;
			sniffling = !sniffling;
		}
	}

	//출력
	/*for (int i = 0; i < 50; i++)
	{
		for (int j = 0; j < 50; j++)
		{
			cout << arr[i][j].c;
		}
		cout << endl;
	}*/

	int q = 1;
	char order;
	bool object = false;
	int O = 0;
	bool plus = true, minus = true;

	while (q)
	{
		system("cls");

		for (int i = 0; i < 50; i++)
		{
			for (int j = 0; j < 50; j++)
			{
				if (arr[i][j].c == '*')
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
				}
				else
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				}
				cout << arr[i][j].c;
			}
			cout << endl;
		}

		cout << "명령어 입력 : ";
		cin >> order;

		switch (order)
		{
		case 'R':
			cnt = 0;
			I = 0, J = 0;

			turn = dis(gen);

			sniffling = true;
			change = false;
			change2 = true;

			object = false;
			O = 0;

			plus = true;
			minus = true;

			while (arr[49][49].c == '1')
			{
				arr[I][J].c = '0';
				arr[I][J].i = cnt;
				if (J == turn && change2)
				{
					change = true;
					change2 = false;
				}
				else if (change)
				{
					//하드코딩 시작!!
					J++;
					cnt++;
					arr[I][J].c = '0';
					arr[I][J].i = cnt;


					J++;
					cnt++;
					arr[I][J].c = '0';
					arr[I][J].i = cnt;

					I--;
					cnt++;
					arr[I][J].c = '0';
					arr[I][J].i = cnt;

					I--;
					cnt++;
					arr[I][J].c = '0';
					arr[I][J].i = cnt;

					J++;
					cnt++;
					arr[I][J].c = '0';
					arr[I][J].i = cnt;

					J++;
					cnt++;
					arr[I][J].c = '0';
					arr[I][J].i = cnt;

					J++;
					cnt++;
					arr[I][J].c = '0';
					arr[I][J].i = cnt;

					I++;
					cnt++;
					arr[I][J].c = '0';
					arr[I][J].i = cnt;

					I++;
					cnt++;
					arr[I][J].c = '0';
					arr[I][J].i = cnt;

					I++;
					cnt++;
					arr[I][J].c = '0';
					arr[I][J].i = cnt;

					J--;
					cnt++;
					arr[I][J].c = '0';
					arr[I][J].i = cnt;

					J--;
					cnt++;
					arr[I][J].c = '0';
					arr[I][J].i = cnt;

					I++;
					cnt++;
					arr[I][J].c = '0';
					arr[I][J].i = cnt;

					I++;
					cnt++;
					arr[I][J].c = '0';
					arr[I][J].i = cnt;

					change = !change;
				}
				else if (!change)
				{
					if (sniffling)
					{
						J++;
					}
					else if (!sniffling)
					{
						I++;
					}
					cnt++;
					sniffling = !sniffling;
				}
			}
			break;
		case 'r':
			if (!object)
			{
				object = true;
			}
			arr[0][0].c = '*';
			break;
		case '+':
			if (object)
			{
				for (int i = 0; i < 50; i++)
				{
					for (int j = 0; j < 50; j++)
					{
						if (arr[i][j].c == '*')
						{
							arr[i][j].c = '0';
						}
					}
				}

				plus = true;

				for (int i = 0; i < 50; i++)
				{
					for (int j = 0; j < 50; j++)
					{
						if (arr[i][j].i == O && plus)
						{
							arr[i][j].c = '*';
							O++;
							plus = false;
						}
					}
				}
			}
			break;
		case '-':
			if (object)
			{
				for (int i = 0; i < 50; i++)
				{
					for (int j = 0; j < 50; j++)
					{
						if (arr[i][j].c == '*')
						{
							arr[i][j].c = '0';
						}
					}
				}

				minus = true;

				for (int i = 0; i < 50; i++)
				{
					for (int j = 0; j < 50; j++)
					{
						if (arr[i][j].i == O&&minus)
						{
							arr[i][j].c = '*';
							O--;
							minus = false;
						}
					}
				}
			}
			break;
		case'q':
			q = 0;
			break;
		}
	}
}