#ifndef SERVER_H
#define SERVER_H
#include "clientThreadWorker.h"
#include <exception>

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
    virtual ~Server(){};

protected:
    void create_new_client_thread(Worker *worker);
};

#endif