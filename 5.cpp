#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>

using namespace std;

struct storeList
{
	bool full = false;
	int x, y, z;
};

storeList List[10];
storeList copyList[10];
storeList sList[10];

int main()
{
	char order;
	int X, Y, Z;
	int q=1;
	int cnt = 0;
	int s = 0;
	bool Select = false;

	while (q)
	{
		printf("명령어를 입력하시오. : ");
		scanf("%s", &order);

		switch (order)
		{
		case '.':
			for (int i = 9; i >= 0; i--)
			{
				if (List[i].full)
				{
					printf("%d번째 칸 : %d %d %d\n", i, List[i].x, List[i].y, List[i].z);
				}
				else
				{
					printf("%d번째 칸 :      \n", i);
				}
			}
			break;
		case '+':
			scanf("%d %d %d",&X, &Y, &Z);

			for (int i = 9; i >= 0; i--)
			{
				if (List[9].full)
				{
					for (int i = 0; i < 10; i++)
					{
						if (!List[i].full)
						{
							List[i].full = true;
							List[i].x = X;
							List[i].y = Y;
							List[i].z = Z;
							break;
						}
					}
					break;
				}
				else if (List[i].full)
				{
					List[i + 1].full = true;
					List[i + 1].x = X;
					List[i + 1].y = Y;
					List[i + 1].z = Z;
					break;
				}
				else if(!List[0].full)
				{
					List[0].full = true;
					List[0].x = X;
					List[0].y = Y;
					List[0].z = Z;
					break;
				}
			}
			break;
		case '-':
			for (int i = 9; i >= 0; i--)
			{
				if (List[i].full)
				{
					List[i].full = false;
					List[i].x = 0;
					List[i].y = 0;
					List[i].z = 0;
					break;
				}
			}
			break;
		case 'e':
			scanf("%d %d %d", &X, &Y, &Z);
			
			for (int i = 0; i < 10; i++)
			{
				copyList[i] = List[i];
			}

			for (int i = 0; i < 9; i++)
			{
				List[i+1] = copyList[i];
			}

			List[0].full = true;
			List[0].x = X;
			List[0].y = Y;
			List[0].z = Z;

			break;
		case 'd':
			for (int i = 0; i < 10; i++)
			{
				if (List[i].full)
				{
					List[i].full = false;
					List[i].x = 0;
					List[i].y = 0;
					List[i].z = 0;
					break;
				}
			}
			break;
		case 'l':
			cnt = 0;

			for (int i = 0; i < 10; i++)
			{
				if (List[i].full)
				{
					cnt++;
				}
			}
			printf("리스트 길이 : %d\n", cnt);
			break;
		case 'c':
			for (int i = 0; i < 10; i++)
			{
				List[i].full = false;
				List[i].x = 0;
				List[i].y = 0;
				List[i].z = 0;
			}
			break;
		case 'm':
			for (int i = 9; i >= 0; i--)
			{
				if (List[i].full)
				{
					printf("(%d %d %d)\n", List[i].x, List[i].y, List[i].z);
					break;
				}
			}
			break;
		case 'n':
			for (int i = 0; i < 10; i++)
			{
				if (List[i].full)
				{
					printf("(%d %d %d)\n", List[i].x, List[i].y, List[i].z);
					break;
				}
			}
			break;
		case 's':
			s = 0;
			Select = !Select;

			if (Select)
			{
				for (int i = 0; i < 10; i++)
				{
					copyList[i] = List[i];
				}

				for (int i = 0; i < 10; i++)
				{
					List[i].full = false;
					List[i].x = 0;
					List[i].y = 0;
					List[i].z = 0;
				}

				for (int i = 0; i < 10; i++)
				{
					if (s > 9)
					{
						break;
					}
					else if (copyList[s].full)
					{
						List[i] = copyList[s];
						s++;
					}
					else if (!copyList[s].full)
					{
						i--;
						s++;
					}
				}
			}

			else
			{
				for (int i = 0; i < 10; i++)
				{
					List[i] = copyList[i];
				}
			}
			
			break;
		case'q':
			q = 0;
			break;
		}
	}
}