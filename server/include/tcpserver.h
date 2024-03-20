#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>

class Server;
class Worker;

class TCPMessageServer : public QTcpServer
{
    Q_OBJECT
public:
    TCPMessageServer(Server &server, QHostAddress ip, quint16 port, bool usePingCommunicator, QObject *parent = nullptr);
    ~TCPMessageServer(){};

protected:
    void create_new_client_thread(Worker *worker);
    bool usePingCommunicator;
    void incomingConnection(qintptr socketDescriptor) override;

private:
    Server &server;
};

#endif