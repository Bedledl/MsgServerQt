#include "clientThreadWorker.h"
#include "communicators.h"
#include <iostream>
#include "moc_clientThreadWorker.cpp"

TCPServerWorker::TCPServerWorker(qintptr socketDescriptor, QObject *parent)
: Worker(parent), socketDescriptor(socketDescriptor) {
    std::cout << "Client Thread Worker Init" << std::endl;
}

void TCPServerWorker::process() {
    communicator = std::make_unique<PingPongCommunicator>();

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
    out << communicator->answerMessage(QString(buffer)); // may access unitilialized memory because of maybe missing \0
    tcpSocket.write("Ping");
//    tcpSocket.disconnectFromHost();
//    tcpSocket.waitForDisconnected();

    emit finished();
}
