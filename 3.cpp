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
	//	//��ġ �����ڸ� ���� ������ �ű��.
	//	in.seekg(0, ios::end);	//�� ��° ���� = ���� �� ��ġ, ù��° ���� = �� ��ġ�� ���� ��ŭ ������ �ֳ�
	//	//�׸��� �� ��ġ�� �д´�.(������ ũ�� ���)
	//	int size = in.tellg();
	//	//�� ũ���� ���ڿ��� �Ҵ��Ѵ�.
	//	s.resize(size);
	//	//��ġ �����ڸ� �ٽ� ���� �� ������ �ű��.
	//	in.seekg(0, ios::beg);
	//	//���� ��ü ������ �о ���ڿ��� �����Ѵ�.
	//	in.read(&s[0], size);
	//	cout << s << endl;
	//}
	
	ifstream file("test.txt");
	if (!file.is_open())
	{
		cerr << "������ �� �� �����ϴ�." << endl;
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
		//clear ���ڿ� ��ü�� ������ ��� �����ϰ� ���, ���ڿ� ���� = 0, string.clear()
		result.clear();

		cout << "��ɾ� �Է� : ";
		cin >> order;

		switch (order)
		{
		case 'q':
			q = 0;
			break;
		case 'd':
			result = s;
			reverse(result.begin(), result.end());	//�������� �Ųٷ� ������, rverse(begin, end) begin�� end�� ������ ���� ��Ÿ��
			cout << result << endl;
			break;
			
		case 'e':
			cout << "������ ���ڿ� ���� �Է� : ";
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