#ifndef SERVER_INCLUDE_CLIENTTHREADWORKER
#define SERVER_INCLUDE_CLIENTTHREADWORKER

#include <QObject>
#include <QTcpSocket>

#include "chat.h"
#include "clientThreadIface.h"
#include "communicator.h"
#include "rawClientMessageProcessor.h"
#include "server.h"

class Participant;

class Worker : public QObject, public ClientThreadIface
{
    Q_OBJECT
public:
    explicit Worker(ServerIface &server, bool usePingCommunicator, QObject *parent);
public slots:
    virtual void initialize() = 0;
    virtual void sendMsgToClient(QString msg) = 0;
signals:
    void finished();
    void error(QTcpSocket::SocketError socketError);

protected:
    std::function<void(std::string)> processRawMessages;

private:
    std::unique_ptr<ServerParticipant> participantInfo;
    std::unique_ptr<PingPongCommunicator> pingPong;
    std::unique_ptr<RawClientMessageProcessor> rawClientMessageProcessor;
};

class TCPServerWorker : public Worker
{
    Q_OBJECT
public:
    TCPServerWorker(ServerIface &server, qintptr socketDescriptor, bool usePingCommunicator, QObject *parent);

public slots:
    void initialize() override;
    void readFromSocket();
    void sendMsgToClient(QString msg) override;

private:
    qintptr socketDescriptor;
    QTcpSocket *tcpSocket;
    QDataStream in;
    QByteArray block;
    QDataStream out{&block, QIODevice::WriteOnly};
};
#endif /* SERVER_INCLUDE_CLIENTTHREADWORKER */
