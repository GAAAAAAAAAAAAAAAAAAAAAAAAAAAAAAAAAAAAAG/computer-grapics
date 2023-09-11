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
	ifstream in("test.txt");
	string s;
	string result;
	string word;

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
		s += line + '\n';
	}

	file.close();

	char order;
	int q = 1;
	char C;
	int I;
	int cnt = 0;
	char change, newC;

	string pre, suf;

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

			for (char c : s) 
			{
				//char c�� �������� ����� �ݺ� ����, �� �ݺ����� s���ڿ��� �� ���ڸ� ��ǥ, :�� ���� ��� for �������� �����̳ʿ� �ݺ� ������ �����ϴ� ������
				
				result += c;
				cnt++;
				if (cnt % I == 0)
				{
					result += C;
				}
			}
			cout << result << endl;
			break;
		case 'f':
			for (char c : s) {
				if (!isspace(c)) {
					word += c;
				}
				else if (!word.empty()) {
					reverse(word.begin(), word.end());
					cout << word << ' ';
					word.clear();
				}
			}
			reverse(word.begin(), word.end());
			cout << word << endl;
			break;
		case 'g':
			cout << "�ٲ� ���ڿ� ���ο� ���� �Է� : ";
			cin >> change >> newC;

			for (char c : s)
			{
				if (c == change)
				{
					result += newC;
				}
				else
				{
					result += c;
				}
			}
			cout << result << endl;
			break;
		case 'h':
			for (int i = 0; i < s.length() / 2; i++)
			{
				if (s[i] == s[s.length() - i - 1])
				{
					cout << s.substr(0, i + 1) << " : " << s.substr(s.length() - i - 1, i + 1) << endl;

				}
				else
				{
					break;
				}
			}
			break;
		case '+':
			for (char c : s) {
				if (isdigit(c)) {
					int digit = c - '0';
					digit = max(digit + 1, 0);  // ���� ����
					result += to_string(digit);
				}
				else {
					result += c;
				}
			}
			cout << result << endl;
			break;
		case '-':
			for (char c : s) {
				if (isdigit(c)) {
					int digit = c - '0';
					digit = max(digit - 1, 0);  // ���� ����
					result += to_string(digit);
				}
				else {
					result += c;
				}
			}
			cout << result << endl;
			break;
		}
	}

	return 0;
}