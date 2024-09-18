#pragma once

#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdio>

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

    /**
     Establish a connection with the server
    */
    void connectServer();

    /**
     Registering an account for a new chat participant
     *
     * @param login, password, name
    */
    int registration(std::string login, std::string password, std::string name);

    /**
     Account authorization
     *
     * @param login, password
    */
    int authorization(std::string login, std::string password);

    /**
     Send a message
     *
     * @param nameRecipient, text
    */
    int sendMessage(std::string nameRecipient, std::string text);

    /**
     Reading common messages
    */
    std::vector<std::string> getChatCommonMessages();

    /**
     Reading private messages
    */
    std::vector<std::string>getChatPrivateMessage();

private:
    /**
     Get an integer type variable from the server
    */
    int getServerInt();

    /**
     Get a string variable from the server
    */
    std::string getServerString();

    /**
     * Send an integer type variable to the server
     *
     * @param action
    */
    void sendServerInt(int action);

    /**
     * Send a string variable to the server
     *
     * @param message
    */
    void sendServerString(std::string message);

    SOCKET connectSocket; // Socket for connecting to the server
};
