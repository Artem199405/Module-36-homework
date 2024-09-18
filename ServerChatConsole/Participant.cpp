#include "Participant.h"

Participant:: Participant() : _disabled(false), _banned(false)
{
}

Participant::~Participant()
{
}

void Participant::setId(int id)
{
    _id = id;
}

void Participant::setLogin(string login)
{
	_login = login;
}

void Participant::setPassword(string password)
{
	_password = password;

    setPasswordHash(password);
}

void Participant::setName(string name)
{
	_name = name;
}

void Participant::setPasswordHash(string password)
{
    const char* charPassword = password.c_str();
    int length = strlen(charPassword);

    _password_hash = sha1(charPassword, (uint)length);
}

void Participant::setDisabled(bool status)
{
    _disabled = status;
}

void Participant::setBanned(bool status)
{
    _banned = status;
}

int Participant::getId() const noexcept
{
    return _id;
}

string Participant::getLogin() const noexcept
{
	return _login;
}

string Participant::getPassword() const noexcept
{
	return _password;
}

string Participant::getName() const noexcept
{
	return _name;
}

uint Participant::getPasswordHash() const noexcept
{
    return *_password_hash;
}

bool Participant::getDisabled() const noexcept
{
    return _disabled;
}

bool Participant::getBanned() const noexcept
{
    return _banned;
}
