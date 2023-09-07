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
			//행렬1 숫자 곱
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr1[i][j] = arr1[i][j] * 1;
				}
			}
			//행렬2 숫자 곱
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr2[i][j] = arr2[i][j] * 1;
				}
			}

			//출력
			cout << "행렬1" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr1[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			cout << "행렬2" << endl;
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
			//행렬1 숫자 곱
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr1[i][j] = arr1[i][j] * 2;
				}
			}
			//행렬2 숫자 곱
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr2[i][j] = arr2[i][j] * 2;
				}
			}

			//출력
			cout << "행렬1" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr1[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			cout << "행렬2" << endl;
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
			//행렬1 숫자 곱
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr1[i][j] = arr1[i][j] * 3;
				}
			}
			//행렬2 숫자 곱
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr2[i][j] = arr2[i][j] * 3;
				}
			}

			//출력
			cout << "행렬1" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr1[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			cout << "행렬2" << endl;
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
			//행렬1 숫자 곱
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr1[i][j] = arr1[i][j] * 4;
				}
			}
			//행렬2 숫자 곱
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr2[i][j] = arr2[i][j] * 4;
				}
			}

			//출력
			cout << "행렬1" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr1[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			cout << "행렬2" << endl;
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
			//행렬1 숫자 곱
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr1[i][j] = arr1[i][j] * 5;
				}
			}
			//행렬2 숫자 곱
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr2[i][j] = arr2[i][j] * 5;
				}
			}

			//출력
			cout << "행렬1" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr1[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			cout << "행렬2" << endl;
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
			//행렬1 숫자 곱
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr1[i][j] = arr1[i][j] * 6;
				}
			}
			//행렬2 숫자 곱
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr2[i][j] = arr2[i][j] * 6;
				}
			}

			//출력
			cout << "행렬1" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr1[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			cout << "행렬2" << endl;
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
			//행렬1 숫자 곱
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr1[i][j] = arr1[i][j] * 7;
				}
			}
			//행렬2 숫자 곱
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr2[i][j] = arr2[i][j] * 7;
				}
			}

			//출력
			cout << "행렬1" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr1[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			cout << "행렬2" << endl;
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
			//행렬1 숫자 곱
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr1[i][j] = arr1[i][j] * 8;
				}
			}
			//행렬2 숫자 곱
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr2[i][j] = arr2[i][j] * 8;
				}
			}

			//출력
			cout << "행렬1" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr1[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			cout << "행렬2" << endl;
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
			//행렬1 숫자 곱
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr1[i][j] = arr1[i][j] * 9;
				}
			}
			//행렬2 숫자 곱
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr2[i][j] = arr2[i][j] * 9;
				}
			}

			//출력
			cout << "행렬1" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr1[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			cout << "행렬2" << endl;
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
			//값 넣기
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

			//값 넣기
			Arr1 = ((arr1_4[0][0] * arr1_4[1][1] * arr1_4[2][2] * arr1_4[3][3]) + (arr1_4[0][1] * arr1_4[1][2] * arr1_4[2][3] * arr1_4[3][0]) + (arr1_4[0][2] * arr1_4[1][3] * arr1_4[2][0] * arr1_4[3][1]) + (arr1_4[0][3] * arr1_4[1][0] * arr1_4[2][1] * arr1_4[3][2])) - ((arr1_4[0][3] * arr1_4[1][2] * arr1_4[2][1] * arr1_4[3][0]) + (arr1_4[1][3] * arr1_4[2][2] * arr1_4[3][1] * arr1_4[0][0]) + (arr1_4[2][3] * arr1_4[3][2] * arr1_4[0][1] * arr1_4[1][0]) + (arr1_4[3][3] * arr1_4[0][2] * arr1_4[1][1] * arr1_4[2][0]));
			//값 넣기
			Arr2 = ((arr2_4[0][0] * arr2_4[1][1] * arr2_4[2][2] * arr2_4[3][3]) + (arr2_4[0][1] * arr2_4[1][2] * arr2_4[2][3] * arr2_4[3][0]) + (arr2_4[0][2] * arr2_4[1][3] * arr2_4[2][0] * arr2_4[3][1]) + (arr2_4[0][3] * arr2_4[1][0] * arr2_4[2][1] * arr2_4[3][2])) - ((arr2_4[0][3] * arr2_4[1][2] * arr2_4[2][1] * arr2_4[3][0]) + (arr2_4[1][3] * arr2_4[2][2] * arr2_4[3][1] * arr2_4[0][0]) + (arr2_4[2][3] * arr2_4[3][2] * arr2_4[0][1] * arr2_4[1][0]) + (arr2_4[3][3] * arr2_4[0][2] * arr2_4[1][1] * arr2_4[2][0]));

			//출력
			cout << "행렬1" << endl;
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					cout << arr1_4[i][j] << ' ';
				}
				cout << endl;
			}
			cout << "행렬 1의 값 : " << Arr1 << endl;
			cout << "행렬2" << endl;
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					cout << arr2_4[i][j] << ' ';
				}
				cout << endl;
			}
			cout << "행렬 2의 값 : " << Arr2 << endl;


			break;
		case 't':
			//출력
			cout << "행렬1" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr1[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			cout << "행렬2" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr2[i][j] << ' ';
				}
				cout << endl;
			}
			//값 초기화
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					copyArr1[i][j] = 0;
					copyArr2[i][j] = 0;
				}
			}
			//값 복사
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					copyArr1[i][j] = arr1[j][i];
					copyArr2[i][j] = arr2[j][i];
				}
			}
			//값 넣기
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr1[i][j] = copyArr1[i][j];
					arr2[i][j] = copyArr2[i][j];
				}
			}
			//전치행렬 출력
			cout << "전치행렬로 바꾼 행렬1" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr1[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			cout << "전치행렬로 바꾼 행렬2" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr2[i][j] << ' ';
				}
				cout << endl;
			}
			//값 초기화
			Arr1 = 0;
			Arr2 = 0;

			//전치행렬 값
			Arr1 = ((arr1[0][0] * arr1[1][1] * arr1[2][2]) + (arr1[0][1] * arr1[1][2] * arr1[2][0]) + (arr1[0][2] * arr1[1][0] * arr1[2][1])) - ((arr1[0][2] * arr1[1][1] * arr1[2][0]) + (arr1[0][0] * arr1[1][2] * arr1[2][1]) + (arr1[0][1] * arr1[1][0] * arr1[2][2]));
			Arr2 = ((arr2[0][0] * arr2[1][1] * arr2[2][2]) + (arr2[0][1] * arr2[1][2] * arr2[2][0]) + (arr2[0][2] * arr2[1][0] * arr2[2][1])) - ((arr2[0][2] * arr2[1][1] * arr2[2][0]) + (arr2[0][0] * arr2[1][2] * arr2[2][1]) + (arr2[0][1] * arr2[1][0] * arr2[2][2]));

			//출력
			cout << "전치행렬 1의 값 : " << Arr1 << endl;
			cout << "전치행렬 2의 값 : " << Arr2 << endl;
			break;
		case 'm':
			//행렬3 초기화
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr3[i][j] = 0;
				}
			}
			//행렬 곱셈
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr3[i][j] = (arr1[i][0] * arr2[0][j]) + (arr1[i][1] * arr2[1][j]) + (arr1[i][2] * arr2[2][j]);
				}
			}
			//출력
			cout << "행렬1" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr1[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			cout << "행렬2" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr2[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			cout << "행렬1과 행렬2 곱셈 결과" << endl;
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
			//값 초기화
			Arr1 = 0;
			Arr2 = 0;

			//값 넣기
			Arr1 = ((arr1[0][0] * arr1[1][1] * arr1[2][2]) + (arr1[0][1] * arr1[1][2] * arr1[2][0]) + (arr1[0][2] * arr1[1][0] * arr1[2][1])) - ((arr1[0][2] * arr1[1][1] * arr1[2][0]) + (arr1[0][0] * arr1[1][2] * arr1[2][1]) + (arr1[0][1] * arr1[1][0] * arr1[2][2]));
			Arr2 = ((arr2[0][0] * arr2[1][1] * arr2[2][2]) + (arr2[0][1] * arr2[1][2] * arr2[2][0]) + (arr2[0][2] * arr2[1][0] * arr2[2][1])) - ((arr2[0][2] * arr2[1][1] * arr2[2][0]) + (arr2[0][0] * arr2[1][2] * arr2[2][1]) + (arr2[0][1] * arr2[1][0] * arr2[2][2]));

			//출력
			cout << "행렬1" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr1[i][j] << ' ';
				}
				cout << endl;
			}
			cout << "행렬 1의 값 : " << Arr1 << endl;
			cout << "행렬2" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr2[i][j] << ' ';
				}
				cout << endl;
			}
			cout << "행렬 2의 값 : " << Arr2 << endl;

			break;
		case 'a':
			//행렬3 초기화
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr3[i][j] = 0;
				}
			}
			//행렬 덧셈
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr3[i][j] = arr1[i][j] + arr2[i][j];
				}
			}
			//출력
			cout << "행렬1" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr1[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			cout << "행렬2" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr2[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			cout << "행렬1과 행렬2 덧셈 결과" << endl;
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
			//행렬3 초기화
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr3[i][j] = 0;
				}
			}
			//행렬 뺄셈
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr3[i][j] = arr1[i][j] - arr2[i][j];
				}
			}
			//출력
			cout << "행렬1" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr1[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			cout << "행렬2" << endl;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cout << arr2[i][j] << ' ';
				}
				cout << endl;
			}
			cout << endl;
			cout << "행렬1과 행렬2 덧셈 결과" << endl;
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
			//랜덤 리셋
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					arr1[i][j] = dis(gen);
					arr2[i][j] = dis(gen);
				}
			}

			//출력
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