#include "clientThreadWorker.h"
#include "communicator.h"
#include "moc_clientThreadWorker.cpp"
#include <iostream>

#include <unistd.h>

TCPServerWorker::TCPServerWorker(qintptr socketDescriptor, QObject *parent)
    : Worker(parent), socketDescriptor(socketDescriptor), tcpSocket(new QTcpSocket(this))
{

    in.setVersion(QDataStream::Qt_6_6);
    out.setVersion(QDataStream::Qt_6_6);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &TCPServerWorker::readFromSocket);
}

void TCPServerWorker::initialize()
{
    communicator = std::make_unique<PingPongCommunicator>([](const std::string &)
                                                          { qDebug() << "dummy function"; });

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
    communicator->processRawMessage(msg);
}

void TCPServerWorker::sendMsgToClient(QString msg)
{
    out << msg;
    tcpSocket->write(block);
}
