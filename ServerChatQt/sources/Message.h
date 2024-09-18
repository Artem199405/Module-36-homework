#pragma once

#include <string>

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
    void setSender(std::string);

    void setRecipient(std::string);

    void setText(std::string);

	// Getter
    std::string getSender();

    std::string getRecipient();

    std::string getText();

private:
    std::string _sender, _recipient, _text;
};
