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
		//��ġ �����ڸ� ���� ������ �ű��.
		in.seekg(0, ios::end);	//�� ��° ���� = ���� �� ��ġ, ù��° ���� = �� ��ġ�� ���� ��ŭ ������ �ֳ�
		//�׸��� �� ��ġ�� �д´�.(������ ũ�� ���)
		int size = in.tellg();
		//�� ũ���� ���ڿ��� �Ҵ��Ѵ�.
		s.resize(size);
		//��ġ �����ڸ� �ٽ� ���� �� ������ �ű��.
		in.seekg(0, ios::beg);
		//���� ��ü ������ �о ���ڿ��� �����Ѵ�.
		in.read(&s[0], size);
		cout << s << endl;
	}
	else
	{
		cout << "������ ã�� �� �����ϴ�." << endl;
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
	
	cout << "�ܾ� ���� : " << count - numCount << endl;
	cout << "���� ���� : " << numCount << endl;
	cout << "�빮�� �ܾ� ���� : " << upperCount << endl;

	return 0;
}