#ifndef SERVER_H
#define SERVER_H
#include <QObject>

//https://doc.qt.io/qt-6/qtnetwork-threadedfortuneserver-example.html

class Server : public QObject {
    public:
        Server(QObject *parent = nullptr) : QObject(parent) {};
        virtual ~Server();
    protected:
        void create_new_client_thread(qintptr socketDescriptor);
};

    public:
    protected:
};

#endif