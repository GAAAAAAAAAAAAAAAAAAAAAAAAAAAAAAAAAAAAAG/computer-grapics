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

	if (in.is_open()) {
		//위치 지정자를 파일 끝으로 옮긴다.
		in.seekg(0, ios::end);	//두 번째 인자 = 파일 내 위치, 첫번째 인자 = 그 위치로 부터 얼만큼 떨어져 있냐
		//그리고 그 위치를 읽는다.(파일의 크기 계산)
		int size = in.tellg();
		//그 크기의 문자열을 할당한다.
		s.resize(size);
		//위치 지정자를 다시 파일 맨 앞으로 옮긴다.
		in.seekg(0, ios::beg);
		//파일 전체 내용을 읽어서 문자열에 저장한다.
		in.read(&s[0], size);
		cout << s << endl;
	}
	else
	{
		cout << "파일을 찾을 수 없습니다." << endl;
	}

	int count = 1;
	int numCount = 0;
	bool numCheck = false;
	int upperCount = 0;
	bool upperCheck = false;
	bool spaceCheck = false;

	for (int i = 0; i < s.length(); i++)
	{
		if (isspace(s[i])&&!spaceCheck)
		{
			count++;
			upperCheck = false;
			spaceCheck = true;
		}
		if (!isspace(s[i]))
		{
			spaceCheck = false;
		}
		if (isdigit(s[i]) && !numCheck)
		{
			numCount++;
			numCheck = true;
		}
		if (!isdigit(s[i]))
		{
			numCheck = false;
		}
		if (isupper(s[i]) && !upperCheck)
		{
			upperCount++;
			upperCheck = true;
		}
	}
	
	cout << "단어 개수 : " << count - numCount << endl;
	cout << "숫자 개수 : " << numCount << endl;
	cout << "대문자 단어 개수 : " << upperCount << endl;

	return 0;
}