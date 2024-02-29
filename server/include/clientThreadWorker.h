#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QTcpSocket>

#include "chat.h"
#include "clientThreadIface.h"

class Server;

class Worker : public QObject, public ClientThreadIface
{
    Q_OBJECT
public:
    Worker(const Server &server, QObject *parent) : QObject(parent), ClientThreadIface(server) {}

    void setNickname(std::string newName) override
    {
        participant->setNickname(QString::fromStdString(newName));
    }
    void requestLeavingChat(ChatKey &key) override
    {
        participant->leaveChat(key);
    }
    std::vector<ParticipantKey> requestChatParticipantKeys(ChatKey &key) override
    {
        qDebug() << "requestChatParticipantKeys: Not implemented yet";
        return participant->getParticipantKeys(key);
    }
    ChatKey requestNewChat() override
    {
        return participant->newChat();
    }
public slots:
    virtual void process() = 0;
signals:
    void finished();
    void error(QTcpSocket::SocketError socketError);
};

class TCPServerWorker : public Worker
{
    Q_OBJECT
public:
    TCPServerWorker(const Server &server, qintptr socketDescriptor, bool usePingCommunicator, QObject *parent);

public slots:
    void process() override;
    void readFromSocketAndAswer();

private:
    qintptr socketDescriptor;
    QTcpSocket *tcpSocket;
    QDataStream in;
    QByteArray block;
    QDataStream out{&block, QIODevice::WriteOnly};
};
#endif
