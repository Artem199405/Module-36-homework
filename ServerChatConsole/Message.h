#pragma once

#include <string>

using namespace std;

class Message
{
public:
	/**
	 Constructor
	*/
	Message();

	/**
	 Destructor
	*/
	~Message();

	// Setter
	void setSender(string);

	void setRecipient(string);

	void setText(string);

	// Getter
	string getSender();

	string getRecipient();

	string getText();

private:
	string _sender, _recipient, _text;
};