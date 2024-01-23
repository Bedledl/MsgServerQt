#include "clientThreadWorker.h"
#include "communicators.h"
#include "moc_clientThreadWorker.cpp"
#include <iostream>

#include <unistd.h>

TCPServerWorker::TCPServerWorker(qintptr socketDescriptor, QObject *parent)
    : Worker(parent), socketDescriptor(socketDescriptor)
{
    tcpSocket = new QTcpSocket(this);
    in.setVersion(QDataStream::Qt_6_6);
    out.setVersion(QDataStream::Qt_6_6);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &TCPServerWorker::readFromSocketAndAswer);
}

void TCPServerWorker::process()
{
    communicator = std::make_unique<PingPongCommunicator>();

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

    auto answer = communicator->answerMessage(msg);

    out << answer;
    tcpSocket->write(block);
}
