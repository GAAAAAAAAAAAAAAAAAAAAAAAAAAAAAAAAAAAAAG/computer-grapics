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

	while (q)
	{
		for (int i = 0; i < 10; i++)
		{
			cout << line[i] << endl;
		}
		for (int i = 0; i < 10; i++)
		{
			result[i].clear();
		}
		
		cout << "명령어 입력 : ";
		cin >> order;

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
			cout << "삽입할 문자와 간격 입력 : ";
		
			cin >> insertC >> I;
			cnt = 0;

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
			break;
		case 'f':
			break;
		case 'q':
			q = 0;
			break;
		}
	}
	//file.close();

	//char order;
	//int q = 1;
	//char C;
	//int I;
	//int cnt = 0;
	//char change, newC;

	//string pre, suf;

	//while (q)
	//{
	//	//clear 문자열 객체의 내용을 모두 제거하고 비움, 문자열 길이 = 0, string.clear()
	//	result.clear();

	//	cout << "명령어 입력 : ";
	//	cin >> order;

	//	switch (order)
	//	{
	//	case 'q':
	//		q = 0;
	//		break;
	//	case 'd':
	//		result = s;
	//		reverse(result.begin(), result.end());	//시퀀스르 거꾸로 뒤집음, rverse(begin, end) begin과 end는 뒤집을 범위 나타냄
	//		cout << result << endl;
	//		break;
	//		
	//	case 'e':
	//		cout << "삽입할 문자와 간격 입력 : ";
	//		cin >> C >> I;

	//		for (char c : s) 
	//		{
	//			//char c는 루프에서 사용할 반복 변수, 각 반복에서 s문자열의 한 문자를 대표, :는 범위 기반 for 루프에서 컨테이너와 반복 변수를 구분하는 연산자
	//			
	//			result += c;
	//			cnt++;
	//			if (cnt % I == 0)
	//			{
	//				result += C;
	//			}
	//		}
	//		cout << result << endl;
	//		break;
	//	case 'f':
	//		for (char c : s) {
	//			if (!isspace(c)) {
	//				word += c;
	//			}
	//			else if (!word.empty()) {
	//				reverse(word.begin(), word.end());
	//				cout << word << ' ';
	//				word.clear();
	//			}
	//		}
	//		reverse(word.begin(), word.end());
	//		cout << word << endl;
	//		break;
	//	case 'g':
	//		cout << "바꿀 문자와 새로운 문자 입력 : ";
	//		cin >> change >> newC;

	//		for (char c : s)
	//		{
	//			if (c == change)
	//			{
	//				result += newC;
	//			}
	//			else
	//			{
	//				result += c;
	//			}
	//		}
	//		cout << result << endl;
	//		break;
	//	case 'h':
	//		for (int i = 0; i < s.length() / 2; i++)
	//		{
	//			if (s[i] == s[s.length() - i - 1])
	//			{
	//				cout << s.substr(0, i + 1) << " : " << s.substr(s.length() - i - 1, i + 1) << endl;

	//			}
	//			else
	//			{
	//				break;
	//			}
	//		}
	//		break;
	//	case '+':
	//		for (char c : s)
	//		{
	//			if (isdigit(c))
	//			{
	//				s[c] += 1;
	//			}
	//		}
	//		cout << s;
	//		//for (char c : s) {
	//		//	if (isdigit(c)) {
	//		//		int digit = c - '0';
	//		//		digit = max(digit + 1, 0);  // 음수 방지
	//		//		result += to_string(digit);
	//		//	}
	//		//	else {
	//		//		result += c;
	//		//	}
	//		//}
	//		//cout << result << endl;
	//		break;
	//	case '-':
	//		//for (char c : s) {
	//		//	if (isdigit(c)) {
	//		//		int digit = c - '0';
	//		//		digit = max(digit - 1, 0);  // 음수 방지
	//		//		result += to_string(digit);
	//		//	}
	//		//	else {
	//		//		result += c;
	//		//	}
	//		//}
	//		//cout << result << endl;
	//		break;
	//	}
	//}

	//return 0;
}