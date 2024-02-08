#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>

#include "server.h"

class TCPMessageServer : public QTcpServer, public Server
{
    Q_OBJECT
public:
    TCPMessageServer(QHostAddress ip, quint16 port, bool usePingCommunicator, QObject *parent = nullptr);
    ~TCPMessageServer(){};

protected:
    bool usePingCommunicator;
    void incomingConnection(qintptr socketDescriptor) override;
};

#endif