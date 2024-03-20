#include "clientThreadWorker.h"
#include "chat.h"
#include "moc_clientThreadWorker.cpp"
#include "participant.h"
#include "rawClientMessageProcessor.h"
#include "server.h"
#include <iostream>

#include <unistd.h>

class Server;

Worker::Worker(ServerIface &server, bool usePingCommunicator, QObject *parent) : QObject(parent), ClientThreadIface(server)
{
    participant = std::make_unique<ServerParticipant>();

    auto sendMsgToClientCb = [this](std::string msg)
    { this->sendMsgToClient(QString::fromStdString(msg)); };

    if (usePingCommunicator)
    {
        pingPong = std::make_unique<PingPongCommunicator>(sendMsgToClientCb);
        processRawMessages = [this](std::string msg)
        { this->pingPong->processRawMessage(msg); };
    }
    else
    {
        rawClientMessageProcessor = std::make_unique<RawClientMessageProcessor>(server, *participant, sendMsgToClientCb);
        processRawMessages = [this](std::string msg)
        { this->rawClientMessageProcessor->processRawMessage(msg); };
    }
}

TCPServerWorker::TCPServerWorker(ServerIface &server, qintptr socketDescriptor, bool usePingCommunicator, QObject *parent)
    : Worker(server, usePingCommunicator, parent), socketDescriptor(socketDescriptor)
{
    tcpSocket = new QTcpSocket(this);
    in.setVersion(QDataStream::Qt_6_6);
    out.setVersion(QDataStream::Qt_6_6);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &TCPServerWorker::readFromSocket);
}

void TCPServerWorker::initialize()
{

    if (!tcpSocket->setSocketDescriptor(socketDescriptor))
    {
        std::cout << "SetSocketDescriptor error" << std::endl;
        emit error(tcpSocket->error());
        return;
    }
    in.setDevice(tcpSocket);
}

void TCPServerWorker::readFromSocket()
{
    in.startTransaction();
    if (!in.commitTransaction())
    {
        std::cout << "Error QDataStream Status: " << in.status() << std::endl;
        return;
    }
    QString msg;
    in >> msg;
    processRawMessages(msg.toStdString());
}

void TCPServerWorker::sendMsgToClient(QString msg)
{
    out << msg;
    tcpSocket->write(block);
}
