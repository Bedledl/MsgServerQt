#ifndef WORKER_H
#define WORKER_H

#include <QDataStream>
#include <QTcpSocket>
#include <QThread>

class Communicator;
class Server;

// https://doc.qt.io/qt-6/qtnetwork-fortuneclient-example.html

class Worker : public QObject
{
    Q_OBJECT
public:
    Worker(Server *server, QObject *parent) : QObject(parent), server(server){};
    ~Worker(){};

public slots:
    virtual void process() = 0;
signals:
    void finished();
    void error(QTcpSocket::SocketError socketError);

protected:
    Server *server;
    std::shared_ptr<Communicator> communicator;
};

class TCPServerWorker : public Worker
{
    Q_OBJECT
public:
    TCPServerWorker(Server *server, qintptr socketDescriptor, bool usePingCommunicator, QObject *parent);
    QString get_name() const { return name; }
public slots:
    void process() override;
    void readFromSocketAndAswer();

private:
    qintptr socketDescriptor;
    QTcpSocket *tcpSocket;
    QString name;
    QDataStream in;
    QByteArray block;
    QDataStream out{&block, QIODevice::WriteOnly};
};
#endif
