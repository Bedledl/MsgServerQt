#include "tcpserver.h"

#include <QNetworkInterface>
#include <iostream>

#include "clientThreadWorker.h"
#include "moc_tcpserver.cpp"

#include <iostream>

TCPMessageServer::TCPMessageServer(QHostAddress ip, quint16 port, bool usePingCommunicator, QObject *parent) : QTcpServer(parent), usePingCommunicator(usePingCommunicator)
{

    // copied from https://code.qt.io/cgit/qt/qtbase.git/tree/examples/network/fortuneserver/server.cpp?h=6.6
    if (!listen(ip, port))
    {
        std::cout << "Unable to start the server" << std::endl;
        close();
        throw ServerFailedToStart();
    }

    qDebug() << "Server is running on IP: " << serverAddress().toString() << " and port: " << port;
}

void TCPMessageServer::incomingConnection(qintptr socketDescriptor)
{
    std::cout << "incoming Connection" << std::endl;
    create_new_client_thread(new TCPServerWorker(*this, socketDescriptor, usePingCommunicator, this->QTcpServer::parent()));
}
