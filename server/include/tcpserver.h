#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>

#include "server.h"

//https://doc.qt.io/qt-6/qtnetwork-threadedfortuneserver-example.html

class TCPMessageServer : public QTcpServer, public Server {
    Q_OBJECT
    public:
        TCPMessageServer(QObject *parent = nullptr);
        //Server(Server &other) = delete;
        //Server(Server &&other) default;
        ~TCPMessageServer() {};
        //bool hasPendingConnections() const override;
        //QTcpSocket *nextPendingConnection() override ;
    protected:
        void incomingConnection(qintptr socketDescriptor) override;
};

#endif