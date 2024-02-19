#include "include/client.h"
#include "include/chatPreviewModel.h"
#include "include/communicators.h"

#include <QDebug>
#include <QIODevice>
#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <qqml.h>
#include <memory>

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

    std::cout << "connectToHost\n";

    tcpSocket->connectToHost(ip, port);

    std::cout << "SetDevice\n";
    in.setDevice(tcpSocket);
    std::cout << "set versions\n";
    in.setVersion(QDataStream::Qt_6_6);
    out.setVersion(QDataStream::Qt_6_6);
    std::cout << "QDataStream Status: " << in.status() << std::endl;
    connect(tcpSocket, &QIODevice::readyRead, this, &Client::readFromSocketAndAswer);
    connect(tcpSocket, &QAbstractSocket::errorOccurred,
            //! [3]
            [&](QAbstractSocket::SocketError socketError)
            { qDebug() << tcpSocket->errorString(); });
    std::cout << "Write welcome Message:"
              << "\n";
    out << communicator->welcomeMessage();
    nickname = "Juliet";
    remoteIpString = ip.toString();
    remotePort = port;
    chatPreviewListModel = std::make_unique<ChatPreviewListModel>(&chats, this);
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
    std::cout << "Answer: " << answer.toStdString() << std::endl;

    out << answer;
    auto written = tcpSocket->write(block);
    std::cout << "wrote Ping to socket" << written << std::endl;
}

inline QString Client::getNickname() const { return nickname; }
inline int Client::getPort() const { return remotePort; }
inline QString Client::getIp() const { return remoteIpString; }
