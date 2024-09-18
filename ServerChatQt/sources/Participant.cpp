#include "Participant.h"

Participant:: Participant()
{
}

Participant::~Participant()
{
}

void Participant::setId(int id)
{
    _id = id;
}

void Participant::setLogin(std::string login)
{
	_login = login;
}

void Participant::setPassword(std::string password)
{
	_password = password;

    setPasswordHash(password);
}

void Participant::setName(std::string name)
{
	_name = name;
}

void Participant::setPasswordHash(std::string password)
{
    const char* charPassword = password.c_str();
    int length = strlen(charPassword);

    _password_hash = sha1(charPassword, (uint)length);
}

int Participant::getId() const noexcept
{
    return _id;
}

std::string Participant::getLogin() const noexcept
{
	return _login;
}

std::string Participant::getPassword() const noexcept
{
	return _password;
}

std::string Participant::getName() const noexcept
{
	return _name;
}

uint Participant::getPasswordHash() const noexcept
{
    return *_password_hash;
}
