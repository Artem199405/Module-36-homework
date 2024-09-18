#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include "Sha1.h"

using namespace std;

class Participant
{
public:
	/**
	 Constructor
	*/
	Participant();

	/**
	 Destructor
	*/
	~Participant();

	// Setter
	void setId(int id);
	
	void setLogin(string login);

	void setPassword(string password);

	void setName(string name);

	void setPasswordHash(string password);

	void setDisabled(bool status);

	void setBanned(bool status);

	// Getter
	int getId() const noexcept;
	
	string getLogin() const noexcept;

	string getPassword() const noexcept;

	string getName() const noexcept;

	uint getPasswordHash() const noexcept;

	bool getDisabled() const noexcept;

	bool getBanned() const noexcept;

private:
	int _id;
	
	string _login, _password, _name;

	bool _disabled, _banned;

	uint* _password_hash;
};