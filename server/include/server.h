#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QObject>

//https://doc.qt.io/qt-6/qtnetwork-threadedfortuneserver-example.html

class Server : public QObject {
    public:
        Server(QObject *parent = nullptr) : QObject(parent) {};
        virtual ~Server();
    protected:
        void create_new_client_thread(qintptr socketDescriptor);
};

class TCPMessageServer : public QTcpServer, Server {
    Q_OBJECT
    public:
        TCPMessageServer(QObject *parent = nullptr);
        //Server(Server &other) = delete;
        //Server(Server &&other) default;
        //~TCPMessageServer() override {};
        bool hasPendingConnections() const override;
        QTcpSocket *nextPendingConnection() override ;
    protected:
        void incomingConnection(qintptr socketDescriptor) override;
};

#endif