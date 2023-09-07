#include <stdio.h>
#include <random>
#include <iostream>

using namespace std;
random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> dis(0,1);

int arr1[3][3];
int arr2[3][3];
int arr3[3][3];

int arr1_4[4][4];
int arr2_4[4][4];

int main()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			arr1[i][j] = dis(gen);
			arr2[i][j] = dis(gen);
		}
	}

	int q = 1;
	char order = ' ';
	int Arr1 = 0;
	int Arr2 = 0;

	int copyArr1[3][3];
	int copyArr2[3][3];

	while (q)
	{
		cin >> order;
		switch (order)
		{
		case '1':
			//���1 ���� ��
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr1[i][j] = arr1[i][j] * 1;
				}
			}
			//���2 ���� ��
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr2[i][j] = arr2[i][j] * 1;
				}
			}

			//���
			cout << "���1" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr1[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			cout << "���2" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr2[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			break;
		case '2':
			//���1 ���� ��
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr1[i][j] = arr1[i][j] * 2;
				}
			}
			//���2 ���� ��
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr2[i][j] = arr2[i][j] * 2;
				}
			}

			//���
			cout << "���1" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr1[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			cout << "���2" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr2[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			break;
		case '3':
			//���1 ���� ��
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr1[i][j] = arr1[i][j] * 3;
				}
			}
			//���2 ���� ��
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr2[i][j] = arr2[i][j] * 3;
				}
			}

			//���
			cout << "���1" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr1[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			cout << "���2" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr2[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			break;
		case '4':
			//���1 ���� ��
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr1[i][j] = arr1[i][j] * 4;
				}
			}
			//���2 ���� ��
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr2[i][j] = arr2[i][j] * 4;
				}
			}

			//���
			cout << "���1" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr1[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			cout << "���2" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr2[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			break;
		case '5':
			//���1 ���� ��
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr1[i][j] = arr1[i][j] * 5;
				}
			}
			//���2 ���� ��
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr2[i][j] = arr2[i][j] * 5;
				}
			}

			//���
			cout << "���1" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr1[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			cout << "���2" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr2[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			break;
		case '6':
			//���1 ���� ��
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr1[i][j] = arr1[i][j] * 6;
				}
			}
			//���2 ���� ��
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr2[i][j] = arr2[i][j] * 6;
				}
			}

			//���
			cout << "���1" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr1[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			cout << "���2" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr2[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			break;
		case '7':
			//���1 ���� ��
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr1[i][j] = arr1[i][j] * 7;
				}
			}
			//���2 ���� ��
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr2[i][j] = arr2[i][j] * 7;
				}
			}

			//���
			cout << "���1" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr1[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			cout << "���2" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr2[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			break;
		case '8':
			//���1 ���� ��
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr1[i][j] = arr1[i][j] * 8;
				}
			}
			//���2 ���� ��
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr2[i][j] = arr2[i][j] * 8;
				}
			}

			//���
			cout << "���1" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr1[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			cout << "���2" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr2[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			break;
		case '9':
			//���1 ���� ��
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr1[i][j] = arr1[i][j] * 9;
				}
			}
			//���2 ���� ��
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr2[i][j] = arr2[i][j] * 9;
				}
			}

			//���
			cout << "���1" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr1[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			cout << "���2" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr2[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			break;
			//---------------------
		case 'h':
			//�� �ֱ�
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr1_4[i][j] = arr1[i][j];
					arr2_4[i][j] = arr2[i][j];
				}
			}
			arr1_4[0][3] = 0;
			arr1_4[1][3] = 0;
			arr1_4[2][3] = 0;
			arr1_4[3][3] = 1;
			arr1_4[3][0] = 0;
			arr1_4[3][1] = 0;
			arr1_4[3][2] = 0;
			
			arr2_4[0][3] = 0;
			arr2_4[1][3] = 0;
			arr2_4[2][3] = 0;
			arr2_4[3][3] = 1;
			arr2_4[3][0] = 0;
			arr2_4[3][1] = 0;
			arr2_4[3][2] = 0;

			//�� �ֱ�
			Arr1 = ((arr1_4[0][0] * arr1_4[1][1] * arr1_4[2][2] * arr1_4[3][3]) + (arr1_4[0][1] * arr1_4[1][2] * arr1_4[2][3] * arr1_4[3][0]) + (arr1_4[0][2] * arr1_4[1][3] * arr1_4[2][0] * arr1_4[3][1]) + (arr1_4[0][3] * arr1_4[1][0] * arr1_4[2][1] * arr1_4[3][2])) - ((arr1_4[0][3] * arr1_4[1][2] * arr1_4[2][1] * arr1_4[3][0]) + (arr1_4[1][3] * arr1_4[2][2] * arr1_4[3][1] * arr1_4[0][0]) + (arr1_4[2][3] * arr1_4[3][2] * arr1_4[0][1] * arr1_4[1][0]) + (arr1_4[3][3] * arr1_4[0][2] * arr1_4[1][1] * arr1_4[2][0]));
			//�� �ֱ�
			Arr2 = ((arr2_4[0][0] * arr2_4[1][1] * arr2_4[2][2] * arr2_4[3][3]) + (arr2_4[0][1] * arr2_4[1][2] * arr2_4[2][3] * arr2_4[3][0]) + (arr2_4[0][2] * arr2_4[1][3] * arr2_4[2][0] * arr2_4[3][1]) + (arr2_4[0][3] * arr2_4[1][0] * arr2_4[2][1] * arr2_4[3][2])) - ((arr2_4[0][3] * arr2_4[1][2] * arr2_4[2][1] * arr2_4[3][0]) + (arr2_4[1][3] * arr2_4[2][2] * arr2_4[3][1] * arr2_4[0][0]) + (arr2_4[2][3] * arr2_4[3][2] * arr2_4[0][1] * arr2_4[1][0]) + (arr2_4[3][3] * arr2_4[0][2] * arr2_4[1][1] * arr2_4[2][0]));

			//���
			cout << "���1" << endl;
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					cout << arr1_4[i][j] << ' ';
				}
				cout << endl;
			}
			cout << "��� 1�� �� : " << Arr1 << endl;
			cout << "���2" << endl;
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					cout << arr2_4[i][j] << ' ';
				}
				cout << endl;
			}
			cout << "��� 2�� �� : " << Arr2 << endl;


			break;
		case 't':
			//���
			cout << "���1" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr1[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			cout << "���2" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr2[i][j] << ' ';
				}
				cout << endl;
			}
			//�� �ʱ�ȭ
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					copyArr1[i][j] = 0;
					copyArr2[i][j] = 0;
				}
			}
			//�� ����
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					copyArr1[i][j] = arr1[j][i];
					copyArr2[i][j] = arr2[j][i];
				}
			}
			//�� �ֱ�
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr1[i][j] = copyArr1[i][j];
					arr2[i][j] = copyArr2[i][j];
				}
			}
			//��ġ��� ���
			cout << "��ġ��ķ� �ٲ� ���1" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr1[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			cout << "��ġ��ķ� �ٲ� ���2" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr2[i][j] << ' ';
				}
				cout << endl;
			}
			//�� �ʱ�ȭ
			Arr1 = 0;
			Arr2 = 0;

			//��ġ��� ��
			Arr1 = ((arr1[0][0] * arr1[1][1] * arr1[2][2]) + (arr1[0][1] * arr1[1][2] * arr1[2][0]) + (arr1[0][2] * arr1[1][0] * arr1[2][1])) - ((arr1[0][2] * arr1[1][1] * arr1[2][0]) + (arr1[0][0] * arr1[1][2] * arr1[2][1]) + (arr1[0][1] * arr1[1][0] * arr1[2][2]));
			Arr2 = ((arr2[0][0] * arr2[1][1] * arr2[2][2]) + (arr2[0][1] * arr2[1][2] * arr2[2][0]) + (arr2[0][2] * arr2[1][0] * arr2[2][1])) - ((arr2[0][2] * arr2[1][1] * arr2[2][0]) + (arr2[0][0] * arr2[1][2] * arr2[2][1]) + (arr2[0][1] * arr2[1][0] * arr2[2][2]));

			//���
			cout << "��ġ��� 1�� �� : " << Arr1 << endl;
			cout << "��ġ��� 2�� �� : " << Arr2 << endl;
			break;
		case 'm':
			//���3 �ʱ�ȭ
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr3[i][j] = 0;
				}
			}
			//��� ����
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr3[i][j] = (arr1[i][0] * arr2[0][j]) + (arr1[i][1] * arr2[1][j]) + (arr1[i][2] * arr2[2][j]);
				}
			}
			//���
			cout << "���1" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr1[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			cout << "���2" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr2[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			cout << "���1�� ���2 ���� ���" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr3[i][j] << ' ';
				}
				cout << endl;
			}
			break;
		case 'r':
			//�� �ʱ�ȭ
			Arr1 = 0;
			Arr2 = 0;

			//�� �ֱ�
			Arr1 = ((arr1[0][0] * arr1[1][1] * arr1[2][2]) + (arr1[0][1] * arr1[1][2] * arr1[2][0]) + (arr1[0][2] * arr1[1][0] * arr1[2][1])) - ((arr1[0][2] * arr1[1][1] * arr1[2][0]) + (arr1[0][0] * arr1[1][2] * arr1[2][1]) + (arr1[0][1] * arr1[1][0] * arr1[2][2]));
			Arr2 = ((arr2[0][0] * arr2[1][1] * arr2[2][2]) + (arr2[0][1] * arr2[1][2] * arr2[2][0]) + (arr2[0][2] * arr2[1][0] * arr2[2][1])) - ((arr2[0][2] * arr2[1][1] * arr2[2][0]) + (arr2[0][0] * arr2[1][2] * arr2[2][1]) + (arr2[0][1] * arr2[1][0] * arr2[2][2]));

			//���
			cout << "���1" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr1[i][j] << ' ';
				}
				cout << endl;
			}
			cout << "��� 1�� �� : " << Arr1 << endl;
			cout << "���2" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr2[i][j] << ' ';
				}
				cout << endl;
			}
			cout << "��� 2�� �� : " << Arr2 << endl;

			break;
		case 'a':
			//���3 �ʱ�ȭ
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr3[i][j] = 0;
				}
			}
			//��� ����
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr3[i][j] = arr1[i][j] + arr2[i][j];
				}
			}
			//���
			cout << "���1" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr1[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			cout << "���2" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr2[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			cout << "���1�� ���2 ���� ���" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr3[i][j] << ' ';
				}
				cout << endl;
			}
			break;
		case 'd':
			//���3 �ʱ�ȭ
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr3[i][j] = 0;
				}
			}
			//��� ����
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr3[i][j] = arr1[i][j] - arr2[i][j];
				}
			}
			//���
			cout << "���1" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr1[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			cout << "���2" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr2[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			cout << "���1�� ���2 ���� ���" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr3[i][j] << ' ';
				}
				cout << endl;
			}
			break;
		case 's':
			//���� ����
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr1[i][j] = dis(gen);
					arr2[i][j] = dis(gen);
				}
			}

			//���
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr1[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr2[i][j] << ' ';
				}
				cout << endl;
			}

			break;
		case 'q':
			q = 0;
			break;
		}
	}
}