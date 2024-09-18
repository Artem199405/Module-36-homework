#include "chat.h"

Chat::Chat()
{
}

Chat::~Chat()
{
    closesocket(connectSocket);
}

void Chat::connectServer()
{
    // WSAStartup
    WSAData wsaData;
    // The requested version of the Winsock library
    WORD DLLVersion = MAKEWORD(2, 1);

    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(DLLVersion, &wsaData);
    if (iResult != 0)
    {
        std::cout << ("WSAStartup failed with error: %d\n", iResult);
        system("pause");
    }

    // Socket address
    SOCKADDR_IN address;
    int sizeAddress = sizeof(address);
    // Set the IP number for communication
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    // Set the port number for communication
    address.sin_port = htons(port);
    // Using IPv4
    address.sin_family = AF_INET;

    // Creating a socket to connect to the server
    connectSocket = socket(AF_INET, SOCK_STREAM, NULL);
    if (connectSocket == INVALID_SOCKET)
    {
        std::cout << ("Socket failed with error: ", WSAGetLastError());
        system("pause");
    }

    // Connecting to the server
    if (connect(connectSocket, (SOCKADDR*)&address, sizeAddress) != 0)
    {
        std::cout << "Error: failed connect to server.";
        system("pause");
    }
    else
        std::cout << "The connection to the server is established." << std::endl;

}

int Chat::registration(std::string login, std::string password, std::string name)
{
    int action = 1;

    sendServerInt(action);
    sendServerString(login);
    sendServerString(password);
    sendServerString(name);

    int key = getServerInt();

    return key;
}

int Chat::authorization(std::string login, std::string password)
{
    int action = 2;

    sendServerInt(action);
    sendServerString(login);
    sendServerString(password);

    int key = getServerInt();

    return key;
}

int Chat::sendMessage(std::string nameRecipient, std::string text)
{
    int action = 3;

    sendServerInt(action);
    sendServerString(nameRecipient);
    sendServerString(text);

    int key = getServerInt();

    return key;
}

std::vector<std::string> Chat::getChatCommonMessages()
{
    std::vector<std::string> strings;

    int action = 4;

    sendServerInt(action);

    size_t messageCount = getServerInt();

    for (size_t m = 0; m < messageCount; ++m)
    {
        std::string sender = getServerString();
        std::string text = getServerString();

        strings.push_back("<" + sender + ">: " + text);
    }

    return strings;
}

std::vector<std::string> Chat::getChatPrivateMessage()
{
    std::vector<std::string> strings;

    int action = 5;

    sendServerInt(action);

    size_t messageCount = getServerInt();

    for (size_t m = 0; m < messageCount; ++m)
    {
        std::string sender = getServerString();
        std::string recipient = getServerString();
        std::string text = getServerString();

        if (recipient != "all")
            strings.push_back("<Sender: " + sender + "; Recipient: " + recipient + ">: " + text);
    }

    return strings;
}

int Chat::getServerInt()
{
    int i;

    recv(connectSocket, (char*)&i, sizeof(int), NULL);

    return i;
}

std::string Chat::getServerString()
{
    int sizeWord;

    recv(connectSocket, (char*)&sizeWord, sizeof(int), NULL);

    char* word = new char[sizeWord + 1];
    word[sizeWord] = '\0';

    recv(connectSocket, word, sizeWord, NULL);

    return word;
}

void Chat::sendServerInt(int action)
{
    send(connectSocket, (char*)&action, sizeof(int), NULL);
}

void Chat::sendServerString(std::string message)
{
    int sizeMessage = message.size();

    send(connectSocket, (char*)&sizeMessage, sizeof(int), NULL);
    send(connectSocket, message.c_str(), sizeMessage, NULL);
}
