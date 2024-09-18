#pragma once

#pragma comment(lib, "ws2_32.lib")

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdio>
#include <thread>
#include <winsock2.h>
//#include <mysql.h>
#include "Participant.h"
#include "Message.h"
#include "AutoLogin.h"
#include "Logger.h"

#define port 7777

#pragma warning(disable: 4996)

class Chat
{
public:
	/**
	 Constructor
	*/
	Chat();

	/**
	 Destructor
	*/
	~Chat();

	// Establishing a connection to the database
	void connectToDatabase();

	/**
	 Restoring the chat
	*/
	void restoringChat();

	/**
	 Starting the server
	*/
	void runServer();

private:
    /**
	 * Manager for working with chat participants
	 * 
	 * @param i is the client socket
	*/
	void participantHandler(int i);

	/**
	 * Registering an account for a new chat participant
	 * 
	 * @param i is the client socket
	*/
	void registration(int i);

	/**
	 * Adding a new chat participant
	 *
	 * @param id is the ID in the chat participants database
	 * @param login is the login of the new chat participant
	 * @param password is the password of the new chat participant
	 * @param name is the name of the new chat participant
	*/
    void addParticipant(int id, std::string login, std::string password, std::string name);

	/**
	 * Adding a new chat participant to the database
	 *
	 * @param id is the ID in the chat participants database
	 * @param login is the login of the new chat participant
	 * @param password is the password of the new chat participant
	 * @param name is the name of the new chat participant
	*/
    void addParticipantToDatabase(int id, std::string login, std::string password, std::string name);

	/**
	 * Account authorization
	 * 
	 * @param i is the client socket
	*/
	void authorization(int i);

	/**
	 * Log in to your account
	 *
	 * @param login
	 * @param password
	*/
    bool signIn(std::string login, std::string password);

	/**
	 * Determining the hash of the password
	 *
	 * @param password
	*/
    uint findHash(std::string password);

	/**
	 * Entering a message
	 * 
	 * @param i is the client socket
	*/
	void enteringMessage(int i);

	/**
	 * Sending a message
	 *
	 * @param nameSender is the name of the sender
	 * @param recipientName is the name of the recipient
	 * @param text
	*/
    void sendMessage(std::string nameSender, std::string nameRecipient, std::string text);

	/**
	 * Reading the common messages
	 *
	 * @param i is the client socket
	*/
	void getChatCommonMessages(int i);

	/**
	 * Reading the private messages
	 *
	 * @param i is the client socket
	*/
	void getChatPrivateMessages(int i);

	/**
	 * Recording a new message
	 *
	 * @param nameSender, nameRecipient, text
	*/
    void recordMessage(std::string nameSender, std::string nameRecipient, std::string text);

	/**
	 * Number of messages sent
	 *
	 * @param nameSender
	*/
    int countSentMessages(std::string nameSender);
	
	/**
	 * Number of messages sent
	 *
	 * @param nameRecipient
	*/
    int countReceivedMessages(std::string nameRecipient);

	/**
	 * Comparison of existing and entered logins
	 *
	 * @param login
	*/
    bool compareLogin(std::string login);

	/**
	 * Comparing existing and entered names
	 *
	 * @param name
	*/
    bool compareName(std::string name);

	/**
	 * Get a name by login
	 *
	 * @param login
	*/
    std::string getParticipantName(std::string login);

	/**
	 * Get the index of the user in the _participants array by name
	 *
	 * @param name
	*/
    int getParticipantIndex(std::string name);

	/**
	 * Get an integer type variable from a chat participant
	 *
	 * @param i is the client socket
	*/
	int getParticipantInt(int i);

	/**
	 * Get a string variable from a chat participant
	 *
	 * @param i is the client socket
	*/
    std::string getParticipantString(int i);

	/**
	 * Send an int type variable to the chat participant
	 *
	 * @param i is the client socket
	 * @param j is a int variable
	*/
	void sendParticipantInt(int i, int j);

	// Отправить участнику чата переменную типа string
	/**
	 * Send a string variable to the chat participant
	 *
	 * @param i is the client socket
	 * @param word is a string variable
	*/
    void sendParticipantString(int i, std::string word);
	
    std::vector<Participant> _participants;
    std::vector<Message> _messages;

	SOCKET _listenSocket; // The socket that listens for client connections
    std::vector<SOCKET> _participantSocket; // Client sockets
    std::vector<std::thread> _threads;

    //MYSQL mysql; // Connection descriptor

	Logger log;
};
