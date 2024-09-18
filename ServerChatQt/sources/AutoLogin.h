#pragma once

#include <string>

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
    void insert(std::string);

	// ����� ���� �� ��������
    void findPrefixWords(std::string, std::string*);
    void findPrefixWords(AutoLoginNode*, std::string, std::string, std::string*);

	// �������
	AutoLoginNode* getAutoLoginNode();

private:
	AutoLoginNode root;
};
