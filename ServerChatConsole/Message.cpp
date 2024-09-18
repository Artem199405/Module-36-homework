#include "Message.h"

Message::Message()
{
}

Message::~Message()
{
}

void Message::setSender(string sender)
{
	_sender = sender;
}

void Message::setRecipient(string recipient)
{
	_recipient = recipient;
}

void Message::setText(string text)
{
	_text = text;
}

string Message::getSender()
{
	return _sender;
}

string Message::getRecipient()
{
	return _recipient;
}

string Message::getText()
{
	return _text;
}