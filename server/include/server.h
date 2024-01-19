#ifndef SERVER_H
#define SERVER_H
#include <QObject>
#include <exception>

//https://doc.qt.io/qt-6/qtnetwork-threadedfortuneserver-example.html

class Server : public QObject {
    public:
        Server(QObject *parent = nullptr) : QObject(parent) {};
        virtual ~Server();
    protected:
        void create_new_client_thread(qintptr socketDescriptor);
class ServerFailedToStart : public std::exception
{
    virtual const char *what() const throw()
    {
        return "Failed to start server.";
    }
};

    public:
    protected:
};

#endif