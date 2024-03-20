#include "tcpserver.h"
#include "server.h"

#include <QNetworkInterface>
#include <QThread>
#include <iostream>

#include "clientThreadWorker.h"
#include "moc_tcpserver.cpp"

#include <iostream>

TCPMessageServer::TCPMessageServer(Server &server, QHostAddress ip, quint16 port, bool usePingCommunicator, QObject *parent)
    : QTcpServer(parent), usePingCommunicator(usePingCommunicator), server(server)
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

void TCPMessageServer::create_new_client_thread(Worker *worker)
{
    QThread *thread = new QThread();
    worker->moveToThread(thread);

    thread->connect(thread, &QThread::started, worker, &Worker::initialize);
    thread->connect(thread, &QThread::finished, worker, &Worker::deleteLater);
    thread->connect(worker, &Worker::finished, thread, &QThread::quit);

    thread->start();
}

void TCPMessageServer::incomingConnection(qintptr socketDescriptor)
{
    std::cout << "incoming Connection" << std::endl;
    create_new_client_thread(new TCPServerWorker(server, socketDescriptor, usePingCommunicator, this->QTcpServer::parent()));
}
