#pragma once

#include <string>

#define ASCII_SIZE 256

class AutoLogin
{
public:
	struct AutoLoginNode
	{
		//Конструктор
		AutoLoginNode();

		// Деструктор
		~AutoLoginNode();

		AutoLoginNode* children[ASCII_SIZE];

		int childrenCount;

		bool isEndOfWord;
	};

	//Конструктор
	AutoLogin();

	// Деструктор
	~AutoLogin();

	// Добавление нового узла с пустыми детьми
    void insert(std::string);

	// Поиск слов по префиксу
    void findPrefixWords(std::string, std::string*);
    void findPrefixWords(AutoLoginNode*, std::string, std::string, std::string*);

	// Геттеры
	AutoLoginNode* getAutoLoginNode();

private:
	AutoLoginNode root;
};
