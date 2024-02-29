#include "clientThreadWorker.h"
#include "chat.h"
#include "communicators.h"
#include "moc_clientThreadWorker.cpp"
#include "participant.h"
#include <iostream>

#include <unistd.h>

class Server;

TCPServerWorker::TCPServerWorker(const Server &server, qintptr socketDescriptor, bool usePingCommunicator, QObject *parent)
    : Worker(server, parent), socketDescriptor(socketDescriptor)
{
    tcpSocket = new QTcpSocket(this);
    in.setVersion(QDataStream::Qt_6_6);
    out.setVersion(QDataStream::Qt_6_6);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &TCPServerWorker::readFromSocketAndAswer);
    if (usePingCommunicator)
    {
        qDebug() << "Creating PingPongCommunicator";
        communicator = std::make_unique<PingPongCommunicator>();
    }
    else
    {
        qDebug() << "Creating ServerCommunicator";
        communicator = std::make_unique<ServerCommunicator>(*this);
    }
}

void TCPServerWorker::process()
{

    if (!tcpSocket->setSocketDescriptor(socketDescriptor))
    {
        std::cout << "SetSocketDescriptor error" << std::endl;
        emit error(tcpSocket->error());
        return;
    }
    in.setDevice(tcpSocket);

    out << communicator->welcomeMessage();
    tcpSocket->write(block);

    // sleep(20); why does this sleep client too?
}

void TCPServerWorker::readFromSocketAndAswer()
{
    in.startTransaction();
    if (!in.commitTransaction())
    {
        std::cout << "Error QDataStream Status: " << in.status() << std::endl;
        return;
    }
    QString msg;
    in >> msg;

    if (msg.length() == 0)
    {
        std::cout << "Read nothing from socket." << std::endl;
        return;
    }

    auto answer = communicator->answerMessage(msg);

    if (answer.length() == 0)
    {
        std::cout << "Generated answer is empty. Won't send it." << std::endl;
        return;
    }

    out << answer;

    tcpSocket->write(block);
}
