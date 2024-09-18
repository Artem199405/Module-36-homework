#include "Message.h"

Message::Message()
{
}

Message::~Message()
{
}

void Message::setSender(std::string sender)
{
	_sender = sender;
}

void Message::setRecipient(std::string recipient)
{
	_recipient = recipient;
}

void Message::setText(std::string text)
{
	_text = text;
}

std::string Message::getSender()
{
	return _sender;
}

std::string Message::getRecipient()
{
	return _recipient;
}

std::string Message::getText()
{
	return _text;
}
