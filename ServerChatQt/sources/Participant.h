#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include "Sha1.h"

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
	
    void setLogin(std::string login);

    void setPassword(std::string password);

    void setName(std::string name);

    void setPasswordHash(std::string password);

	// Getter
	int getId() const noexcept;
	
    std::string getLogin() const noexcept;

    std::string getPassword() const noexcept;

    std::string getName() const noexcept;

	uint getPasswordHash() const noexcept;

private:
	int _id;
	
    std::string _login, _password, _name;

	uint* _password_hash;
};
