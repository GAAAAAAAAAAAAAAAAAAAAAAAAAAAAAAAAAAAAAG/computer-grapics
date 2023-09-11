#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
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
	ifstream in("test.txt");
	string s;
	string result;

	//if (in.is_open()) {
	//	//위치 지정자를 파일 끝으로 옮긴다.
	//	in.seekg(0, ios::end);	//두 번째 인자 = 파일 내 위치, 첫번째 인자 = 그 위치로 부터 얼만큼 떨어져 있냐
	//	//그리고 그 위치를 읽는다.(파일의 크기 계산)
	//	int size = in.tellg();
	//	//그 크기의 문자열을 할당한다.
	//	s.resize(size);
	//	//위치 지정자를 다시 파일 맨 앞으로 옮긴다.
	//	in.seekg(0, ios::beg);
	//	//파일 전체 내용을 읽어서 문자열에 저장한다.
	//	in.read(&s[0], size);
	//	cout << s << endl;
	//}
	
	ifstream file("test.txt");
	if (!file.is_open())
	{
		cerr << "파일을 열 수 없습니다." << endl;
		return 1;
	}
	
	string line;
	while (getline(file, line))
	{
		s += line +'\n';
	}
	file.close();

	char order;
	int q = 1;
	char C;
	int I;

	while (q)
	{
		//clear 문자열 객체의 내용을 모두 제거하고 비움, 문자열 길이 = 0, string.clear()
		result.clear();

		cout << "명령어 입력 : ";
		cin >> order;

		switch (order)
		{
		case 'q':
			q = 0;
			break;
		case 'd':
			result = s;
			reverse(result.begin(), result.end());	//시퀀스르 거꾸로 뒤집음, rverse(begin, end) begin과 end는 뒤집을 범위 나타냄
			cout << result << endl;
			break;
			
		case 'e':
			cout << "삽입할 문자와 간격 입력 : ";
			cin >> C >> I;
			
			for (const string& sentence : s)
			{

			}
		
			cout << result << endl;
			break;
		case 'f':
			break;
		case 'g':
			break;
		case 'h':
			break;
		case '+':
			break;
		case '-':
			break;
		}
	}

	return 0;
}