#include "clientThreadWorker.h"
#include "communicators.h"
#include "moc_clientThreadWorker.cpp"
#include <iostream>

#include <unistd.h>

class Server;

TCPServerWorker::TCPServerWorker(Server *server, qintptr socketDescriptor, bool usePingCommunicator, QObject *parent)
    : Worker(server, parent), socketDescriptor(socketDescriptor)
{
    tcpSocket = new QTcpSocket(this);
    in.setVersion(QDataStream::Qt_6_6);
    out.setVersion(QDataStream::Qt_6_6);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &TCPServerWorker::readFromSocketAndAswer);
    if (usePingCommunicator)
    {
        communicator = std::make_unique<PingPongCommunicator>();
    }
    else
    {
        communicator = std::make_unique<ServerCommunicator>(server);
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
    std::cout << "after writing welcome Message\n";
    tcpSocket->write(block);
    std::cout << "after sending welcome Message\n";

    // sleep(20); why does this sleep client too?
}

void TCPServerWorker::readFromSocketAndAswer()
{
    std::cout << "foobar\n";
    in.startTransaction();
    if (!in.commitTransaction())
    {
        std::cout << "Error QDataStream Status: " << in.status() << std::endl;
        return;
    }
    QString msg;
    std::cout << "1\n";
    in >> msg;
    std::cout << "2\n";

    auto answer = communicator->answerMessage(msg);
    std::cout << "3\n";

    out << answer;
    std::cout << "4\n";
    tcpSocket->write(block);
}
