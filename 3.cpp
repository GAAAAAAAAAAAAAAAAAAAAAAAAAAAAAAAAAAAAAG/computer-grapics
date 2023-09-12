#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <algorithm>
#include <stdio.h>
#include <iostream>
#include <random>
#include <string>
#include <ctype.h>

using namespace std;

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> dis(0, 3);

int main()
{
	ifstream file("test.txt");
	if (!file.is_open())
	{
		cerr << "파일을 열 수 없습니다." << endl;
		return 1;
	}

	string line[10];
	string result[10];
	string Ebefore[10];

	for (int i = 0; i < 10; i++)
	{
		getline(file, line[i]);
	}

	file.close();

	char order;
	int q = 1;

	char insertC;
	int I;
	int cnt = 0;

	char oldC;
	char newC;

	bool E = false;
	bool H = false;

	while (q)
	{
		if (!H)
		{
			for (int i = 0; i < 10; i++)
			{
				cout << line[i] << endl;
			}
		}

		H = false;

		for (int i = 0; i < 10; i++)
		{
			result[i].clear();
		}

		cout << "명령어 입력 : ";
		std::cin >> order;

		system("cls");

		switch (order)
		{
		case 'd':
			for (int i = 0; i < 10; i++)
			{
				reverse(line[i].begin(), line[i].end());
			}
			break;
		case 'e':
			cnt = 0;
			E = !E;

			if (E)
			{
				cout << "삽입할 문자와 간격 입력 : ";

				std::cin >> insertC >> I;

				for (int i = 0; i < 10; i++)
				{
					Ebefore[i] = line[i];
				}

				for (int i = 0; i < 10; i++)
				{
					for (char c : line[i])
					{
						result[i] += c;
						cnt++;
						if (cnt % I == 0)
						{
							result[i] += insertC;
						}
					}
					line[i] = result[i];
					cnt = 0;
				}
			}
			else
			{
				for (int i = 0; i < 10; i++)
				{
					line[i] = Ebefore[i];
				}
				E = !E;
			}

			break;
		case 'f':
			for (int i = 0; i < 10; i++) {
				string reversedLine = line[i]; // 원본 문자열을 복사

				int start = 0;
				int end = line[i].find(' ');

				while (end != string::npos) {
					// 공백을 기준으로 단어를 추출하고 뒤집어서 다시 추가
					string word = line[i].substr(start, end - start);
					reverse(word.begin(), word.end());
					line[i].replace(start, end - start, word);

					// 다음 공백을 찾을 준비
					start = end + 1;
					end = line[i].find(' ', start);
				}

				// 마지막 단어 처리
				string lastWord = line[i].substr(start);
				reverse(lastWord.begin(), lastWord.end());
				line[i].replace(start, string::npos, lastWord);
			}
			break;
		case 'g':
			cout << "바꿀 문자와 새로운 문자 입력 : ";
			std::cin >> oldC >> newC;

			for (int i = 0; i < 10; i++)
			{
				for (char c : line[i])
				{
					if (c == oldC)
					{
						result[i] += newC;
					}
					else
					{
						result[i] += c;
					}
				}
				line[i] = result[i];
			}
			break;
		case 'h':
			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < line[i].length() / 2; j++)
				{
					if (line[i][j] == line[i][line[i].length() - j - 1])
					{
						cout << line[i] << " : " << line[i].substr(line[i].length() - i - 1, i + 1) << endl;
						break;
					}
					else
					{
						cout << line[i] << " : " << '0' << endl;
						break;
					}
				}
			}
			H = true;

			//substr(start, count) : 시작위치부터 문자 개수만큼, substr(start) : 해당 위치부터 문자열 끝까지 <특정 범위의 문자열 추출>

			break;
		case '+':
			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < line[i].length(); j++)
				{
					if (isdigit(line[i][j]))
					{
						int num = line[i][j] - '0';
						num = (num + 1) % 10;
						line[i][j] = num + '0';
					}
				}
			}
			break;
		case '-':
			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < line[i].length(); j++)
				{
					if (isdigit(line[i][j]))
					{
						int num = line[i][j] - '0';
						num = (num - 1 + 10) % 10;
						line[i][j] = num + '0';
					}
				}
			}
			break;
		case 'q':
			q = 0;
			break;
		}
	}
}