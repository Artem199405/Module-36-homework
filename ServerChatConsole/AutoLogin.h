#pragma once

#include <string>

using namespace std;

#define ASCII_SIZE 256

class AutoLogin
{
public:
	struct AutoLoginNode
	{
		//�����������
		AutoLoginNode();

		// ����������
		~AutoLoginNode();

		AutoLoginNode* children[ASCII_SIZE];

		int childrenCount;

		bool isEndOfWord;
	};

	//�����������
	AutoLogin();

	// ����������
	~AutoLogin();

	// ���������� ������ ���� � ������� ������
	void insert(string);

	// ����� ���� �� ��������
	void findPrefixWords(string, string*);
	void findPrefixWords(AutoLoginNode*, string, string, string*);

	// �������
	AutoLoginNode* getAutoLoginNode();

private:
	AutoLoginNode root;
};