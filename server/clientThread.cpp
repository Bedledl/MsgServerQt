#include "clientThread.h"

#include <iostream>

ClientThread::ClientThread(qintptr socketDescriptor, QObject *parent)
: QThread(parent), socketDescriptor(socketDescriptor) {
    std::cout << "Xlient Thread" << std::endl;
}

void ClientThread::run() {
    QTcpSocket tcpSocket;
    if (!tcpSocket.setSocketDescriptor(socketDescriptor)) {
        emit error(tcpSocket.error());
        return;
    }
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);
    out << "Ping";

    tcpSocket.write("Ping");
    char buffer[64];
    auto len = tcpSocket.readLine(buffer, 64);

    out.setVersion(QDataStream::Qt_6_5);
    out << communicator.answerMessage(QString(buffer)); // may access unitilialized memory because of maybe missing \0
    tcpSocket.write("Ping");
//    tcpSocket.disconnectFromHost();
//    tcpSocket.waitForDisconnected();
}
