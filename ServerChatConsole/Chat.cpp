#include "Chat.h"

Chat::Chat()
{
    log.writeLogFile("Welcome to the chat server!");
}

Chat::~Chat()
{
    for (auto& t : _threads)
        t.join();
    
    closesocket(_listenSocket);
}

void Chat::connectToDatabase()
{
    // Get a connection descriptor
    mysql_init(&mysql);

    // If the descriptor is not received, we output an error message
    if (&mysql == nullptr)
        cout << "Error: can't create MySQL-descriptor" << endl;

    // Connecting to the server (if it is not possible to establish
    //  a connection with the database, we display an error message)
    if (!mysql_real_connect(&mysql, "localhost", "root", "1234", "chatdb", NULL, NULL, 0))
    {
        cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
        log.writeLogFile("Error: can't connect to database!");
    }
    else
    {
        cout << "The connection to the database has been successfully established!" << endl;
        log.writeLogFile("The connection to the database has been successfully established!");
    }

    // Creating a database chat
    mysql_query(&mysql, "CREATE DATABASE IF NOT EXISTS chat");
    mysql_query(&mysql, "USE chat");

    // Creating a table with the data of chat participants
    mysql_query(&mysql, "CREATE TABLE IF NOT EXISTS participants("
                        "id INT AUTO_INCREMENT PRIMARY KEY,"
                        "login VARCHAR(255),"
                        "password VARCHAR(255),"
                        "name VARCHAR(255))");

    // Creating admin participant
    mysql_query(&mysql, "INSERT IGNORE INTO participants(id, login, password, name)"
                        "values(1, 'admin', 'admin', 'admin')");

    // Creating 'all' participant
    mysql_query(&mysql, "INSERT IGNORE INTO participants(id, login, password, name)"
                        "values(2, 'all', 'all', 'all')");

    // Creating a table with a message history
    mysql_query(&mysql, "CREATE TABLE IF NOT EXISTS messages("
                        "id INT AUTO_INCREMENT PRIMARY KEY,"
                        "id_recipient INT,"
                        "id_sender INT,"
                        "text VARCHAR(255))");
}

void Chat::restoringChat()
{
    MYSQL_RES* res;
    MYSQL_ROW row;

    mysql_query(&mysql, "SELECT * FROM participants");
    
    if ( res = mysql_store_result(&mysql))
    {
        while (row = mysql_fetch_row(res))
        {
            int id = atoi(row[0]);
            string login = row[1], password = row[2], name = row[3];

            if (login != "" && password != "" && name != "")
                addParticipant(id, login, password, name);
        }
    }
    else
        cout << "MySQL error number " << mysql_error(&mysql);

    mysql_query(&mysql, "SELECT * FROM messages");

    if (res = mysql_store_result(&mysql))
    {
        while (row = mysql_fetch_row(res))
        {
            int idRecipient = atoi(row[1]), idSender = atoi(row[2]);
            string text = row[3];
            string nameSender, nameRecipient;

            for (size_t i = 0; i < _participants.size(); ++i)
            {
                if (idSender == _participants[i].getId())
                    nameSender = _participants[i].getName();

                if (idRecipient == _participants[i].getId())
                    nameRecipient = _participants[i].getName();
            }

            recordMessage(nameSender, nameRecipient, text);
        }
    }
    else
        cout << "MySQL error number " << mysql_error(&mysql);

    int action;

    log.writeLogFile("The chat was restored successfully!");
    do
    {
        cout << "Do you want to read log.txt file? (1 - Yes; 2 - Not): ";
        cin >> action;

        if (action == 1)
        {
            // Reading log file
            log.readLogFile();
            action = 2;
        }
        else if (action == 2)
        {
        }
        else
        {
            cout << "Invalid operator, repeat the input" << endl;
        }
    } while (action != 2);
}

void Chat::runServer()
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
        cout << ("WSAStartup failed with error: ", iResult);
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

    // Creating a socket for the server that will listen for client connections
    _listenSocket = socket(AF_INET, SOCK_STREAM, NULL);
    if (_listenSocket == INVALID_SOCKET)
    {
        cout << ("Socket failed with error: ", WSAGetLastError());
        system("pause");
    }
    // Configuring a TCP listening socket
    iResult = bind(_listenSocket, (SOCKADDR*)&address, sizeAddress);
    if (iResult == SOCKET_ERROR)
    {
        cout << ("Bind failed with error: ", WSAGetLastError());
        system("pause");
    }
    // Let's set the server to receive data
    iResult = listen(_listenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR)
    {
        cout << ("Listen failed with error: ", WSAGetLastError());
        system("pause");
    }

    std::thread adminThread(&Chat::adminHandler, this);

    int iConnection = 0;
    SOCKET connection;
    while (true)
    {
        // Accept the client socket
        connection = accept(_listenSocket, (SOCKADDR*)&address, &sizeAddress);

        if (connection == INVALID_SOCKET)
        {
            std::cout << ("accept failed with error: ", WSAGetLastError());
            system("pause");
        }
        else
        {
            //std::cout << "Client Connected!" << endl;
            log.writeLogFile("Client Connected!");

            iConnection++;
            _participantSocket.push_back(connection);

            _threads.push_back(std::thread(&Chat::participantHandler, this, iConnection - 1));
        }
    }

    adminThread.join();
}

void Chat::adminHandler()
{
    int action = 0;

    while (action != 100)
    {
        cout << "Select an action (1 or 2 - abling/disabling the participant; \n" <<
            "3 or 4 - ban/unban of the participant; 5 - exit the application): ";
        cin >> action;

        switch (action)
        {
        // Abling the participant
        case 1:
            ablingParticipant();
            break;
        // Disabling the participant
        case 2:
            disablingParticipant();
            break;
        // Ban of the participant
        case 3:
            banParticipant();
            break;
        // Unban of the participant
        case 4:
            unbanParticipant();
            break;
        default:
            action = 100;
        }
    }
}

void Chat::ablingParticipant()
{
    string name;

    cout << "Enter the name of the participant who will be disabled from the chat: ";
    cin >> name;

    int iName = getParticipantIndex(name);

    _participants[iName].setDisabled(false);
}

void Chat::disablingParticipant()
{
    string name;
    
    cout << "Enter the name of the participant who will be disabled from the chat: ";
    cin >> name;

    int iName = getParticipantIndex(name);

    _participants[iName].setDisabled(true);
}

void Chat::banParticipant()
{
    string name;
    
    cout << "Enter the name of the participant who will be banned from the chat: ";
    cin >> name;

    int iName = getParticipantIndex(name);

    _participants[iName].setBanned(true);
}

void Chat::unbanParticipant()
{
    string name;

    cout << "Enter the name of the participant who will be banned from the chat: ";
    cin >> name;

    int iName = getParticipantIndex(name);

    _participants[iName].setBanned(false);
}

void Chat::participantHandler(int i)
{
    int action = 0;
    
    while (action != 100)
    {
        action = getParticipantInt(i);

        switch (action)
        {
        // Account Registration
        case 1:
            registration(i);
            break;
        // Log in to your account
        case 2:
            authorization(i);
            break;
        // Entering a message
        case 3:
            enteringMessage(i);
            break;
        // Reading the common messages
        case 4:
            getChatCommonMessages(i);
            break;
        // Reading the private messages
        case 5:
            getChatPrivateMessages(i);
            break;
        default:
            action = 100;
        }
    }
}

void Chat::registration(int i)
{
    string login = getParticipantString(i);
    string password = getParticipantString(i);
    string name = getParticipantString(i);

    bool key = compareLogin(login);
    if (key == true)
    {
        sendParticipantInt(i, -1);
        return;
    }

    key = compareName(name);
    if (key == true)
    {
        sendParticipantInt(i, -2);
        return;
    }

    sendParticipantInt(i, 0);

    int id = 0;

    addParticipantToDatabase(id, login, password, name);
}

void Chat::addParticipant(int id, string login, string password, string name)
{
    Participant participant;

    participant.setId(id);
    participant.setLogin(login);
    participant.setPassword(password);
    participant.setName(name);

    _participants.push_back(participant);
    log.writeLogFile("Client " + name + " was added to the chat successfully!");
}

void Chat::addParticipantToDatabase(int id, string login, string password, string name)
{
    string str;
    
    str.append("INSERT INTO participants(id, login, password, name) values(default, '")
        .append(login).append("', '").append(password).append("', '").append(name).append("')");
    mysql_query(&mysql, str.c_str());
    
    str.clear();
    
    str.append("SELECT * FROM participants WHERE login = '").append(login).append("'");
    mysql_query(&mysql, str.c_str());

    if (MYSQL_RES* res = mysql_store_result(&mysql))
    {
        MYSQL_ROW row = mysql_fetch_row(res);

        id = atoi(row[0]);
    }

    addParticipant(id, login, password, name);
}

void Chat::authorization(int i)
{
    string login = getParticipantString(i);
    string password = getParticipantString(i);
    string name = getParticipantName(login);
    int iName = getParticipantIndex(name);

    bool key = signIn(login, password);
    if (key == false)
    {
        sendParticipantInt(i, -1);
        return;
    }

    key = _participants[iName].getDisabled();
    if (key)
    {
        sendParticipantInt(i, -2);
        return;
    }

    sendParticipantInt(i, 0);
    log.writeLogFile("The client logged in to the chat. Login: " + login);

    _participantNameSocket.push_back(name);
}

bool Chat::signIn(string login, string password)
{
    uint hash = findHash(password);

    for (size_t i = 0; i < _participants.size(); ++i)
    {
        if (login == _participants[i].getLogin())
        {
            // The first option
            /*if (password == _participants[i].getPassword())
            {
                return true;
            }
            else
            {
                return false;
            }*/

            // The second option
            if (hash == _participants[i].getPasswordHash())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    return false;
}

uint Chat::findHash(string password)
{
    const char* charPassword = password.c_str();
    int length = strlen(charPassword);

    uint* hash = sha1(charPassword, (uint)length);

    return *hash;
}

void Chat::enteringMessage(int i)
{
    string nameSender = _participantNameSocket[i];
    string nameRecipient = getParticipantString(i);
    string text = getParticipantString(i);
    int iSender = getParticipantIndex(nameSender);

    bool key = compareName(nameRecipient);
    if (key == false)
    {
        sendParticipantInt(i, -1);
        return;
    }

    key = _participants[iSender].getBanned();
    if (key == true)
    {
        sendParticipantInt(i, -2);
        return;
    }

    sendParticipantInt(i, 0);
    sendMessage(nameSender, nameRecipient, text);
}

void Chat::sendMessage(string nameSender, string nameRecipient, string text)
{
    int iSender = getParticipantIndex(nameSender);
    int idSender = _participants[iSender].getId();
    int idRecipient;
    string str;

    for (size_t i = 0; i < _participants.size(); ++i)
        if (nameRecipient == _participants[i].getName())
        {
            recordMessage(nameSender, nameRecipient, text);

            idRecipient = _participants[i].getId();

            str.append("INSERT INTO messages(id, id_recipient, id_sender, text) values(default, ")
                .append(to_string(idRecipient)).append(", ").append(to_string(idSender)).append(", '")
                .append(text).append("')");
            mysql_query(&mysql, str.c_str());

            break;
        }

    log.writeLogFile("The message was sent. Sender: " + nameSender + "; Recipient: " + nameRecipient);
}

void Chat::getChatCommonMessages(int i)
{
    string nameRecipient = "all";
    int countMessages = countReceivedMessages(nameRecipient);

    sendParticipantInt(i, countMessages);

    for (size_t m = 0; m < _messages.size(); ++m)
    {
        if (nameRecipient == _messages[m].getRecipient())
        {
            sendParticipantString(i, _messages[m].getSender());
            sendParticipantString(i, _messages[m].getText());
        }
    }
}

void Chat::getChatPrivateMessages(int i)
{
    string name = _participantNameSocket[i];
    int countMessages = countSentMessages(name) + countReceivedMessages(name);

    sendParticipantInt(i, countMessages);

    for (size_t m = 0; m < _messages.size(); ++m)
    {
        if (name == _messages[m].getSender() || name == _messages[m].getRecipient())
        {
            sendParticipantString(i, _messages[m].getSender());
            sendParticipantString(i, _messages[m].getRecipient());
            sendParticipantString(i, _messages[m].getText());
        }
    }
}

void Chat::recordMessage(string nameSender, string nameRecipient, string text)
{
    Message message;

    message.setSender(nameSender);
    message.setRecipient(nameRecipient);
    message.setText(text);

    _messages.push_back(message);
}

int Chat::countSentMessages(string nameSender)
{
    int count = 0;

    for (size_t m = 0; m < _messages.size(); ++m)
        if (nameSender == _messages[m].getSender() && nameSender != _messages[m].getRecipient())
            count++;

    return count;
}

int Chat::countReceivedMessages(string nameRecipient)
{
    int count = 0;

    for (size_t m = 0; m < _messages.size(); ++m)
        if (nameRecipient == _messages[m].getRecipient())
            count++;
    
    return count;
}

bool Chat::compareLogin(string login)
{
    for (size_t i = 0; i < _participants.size(); ++i)
    {
        if (login == _participants[i].getLogin())
            return true;
    }

    return false;
}

bool Chat::compareName(string name)
{
    if (name == "all")
        return true;

    for (size_t i = 0; i < _participants.size(); ++i)
    {
        if (name == _participants[i].getName())
            return true;
    }

    return false;
}

string Chat::getParticipantName(string login)
{
    for (size_t i = 0; i < _participants.size(); ++i)
        if (login == _participants[i].getLogin())
            return _participants[i].getName();
}

int Chat::getParticipantIndex(string name)
{
    for (size_t i = 0; i < _participants.size(); ++i)
        if (name == _participants[i].getName())
            return i;
}

int Chat::getParticipantInt(int i)
{
    int j;
    
    recv(_participantSocket[i], (char*)&j, sizeof(int), NULL);

    return j;
}

string Chat::getParticipantString(int i)
{
    int sizeWord;

    recv(_participantSocket[i], (char*)&sizeWord, sizeof(int), NULL);

    char* word = new char[sizeWord + 1];
    word[sizeWord] = '\0';

    recv(_participantSocket[i], word, sizeWord, NULL);

    return word;
}

void Chat::sendParticipantInt(int i, int j)
{
    send(_participantSocket[i], (char*)&j, sizeof(int), NULL);
}

void Chat::sendParticipantString(int i, string word)
{
    int sizeWord = word.size();

    send(_participantSocket[i], (char*)&sizeWord, sizeof(int), NULL);
    send(_participantSocket[i], word.c_str(), sizeWord, NULL);
}