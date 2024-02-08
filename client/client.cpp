#include "include/client.h"
#include "include/communicators.h"

#include <QDebug>
#include <QIODevice>
#include <QObject>
#include <QString>
#include <QTcpSocket>

#include <iostream>

#include "moc_client.cpp"

Client::Client(QHostAddress ip, quint16 port, bool pingMode, QObject *parent)
    : QObject(parent), tcpSocket(new QTcpSocket(this))
{
    std::cout << "client constrctor" << std::endl;

    if (pingMode)
    {
        communicator = new PingPongCommunicator();
    }
    else
    {
        communicator = new ClientCommunicator(this);
    }
    tcpSocket->connectToHost(ip, port);
    in.setDevice(tcpSocket);
    in.setVersion(QDataStream::Qt_6_6);
    out.setVersion(QDataStream::Qt_6_6);
    std::cout << "QDataStream Status: " << in.status() << std::endl;
    connect(tcpSocket, &QIODevice::readyRead, this, &Client::readFromSocketAndAswer);
    connect(tcpSocket, &QAbstractSocket::errorOccurred,
            //! [3]
            [&](QAbstractSocket::SocketError socketError)
            { qDebug() << tcpSocket->errorString(); });
    out << communicator->welcomeMessage();
}

void Client::readFromSocketAndAswer()
{
    std::cout << "read from Socket begin" << std::endl;
    std::cout << "QDataStream Status: " << in.status() << std::endl;

    // auto r = tcpSocket->readAll();
    // qDebug() << QString::fromLatin1(r);

    in.startTransaction();
    if (!in.commitTransaction())
    {
        std::cout << "Error QDataStream Status: " << in.status() << std::endl;
        return;
    }
    QString msg;
    in >> msg;

    std::cout << "read from socket and aswe4r" << std::endl;

    qDebug() << msg;

    auto answer = communicator->answerMessage(msg);

    out << answer;
    auto written = tcpSocket->write(block);
    std::cout << "wrote Ping to socket" << written << std::endl;
}
