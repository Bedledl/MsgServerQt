#ifndef SERVER_H
#define SERVER_H
#include "chat.h"
#include "clientThreadWorker.h"
#include <exception>
#include <memory>

// https://doc.qt.io/qt-6/qtnetwork-threadedfortuneserver-example.html

class ServerFailedToStart : public std::exception
{
    virtual const char *what() const throw()
    {
        return "Failed to start server.";
    }
};

class Server
{
public:
    Server()
    {
        globalChat = std::make_shared<ServerChat>();
    }
    virtual ~Server(){

    };
    std::shared_ptr<ServerChat> getGlobalChat()
    {
        return globalChat;
    }

protected:
    void create_new_client_thread(Worker *worker);
    std::shared_ptr<ServerChat> globalChat;
};

#endif