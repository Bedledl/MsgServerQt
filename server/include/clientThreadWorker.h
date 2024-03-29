#ifndef WORKER_H
#define WORKER_H

#include <QDataStream>
#include <QTcpSocket>
#include <QThread>

#include "communicators.h"

// https://doc.qt.io/qt-6/qtnetwork-fortuneclient-example.html

class Worker : public QObject
{
    Q_OBJECT
public:
    Worker(QObject *parent) : QObject(parent){};
    ~Worker(){};

public slots:
    virtual void process() = 0;
signals:
    void finished();
    void error(QTcpSocket::SocketError socketError);
};

class TCPServerWorker : public Worker
{
    Q_OBJECT
public:
    TCPServerWorker(qintptr socketDescriptor, QObject *parent);
    QString get_name() const { return name; }
public slots:
    void process() override;
    void readFromSocketAndAswer();

private:
    qintptr socketDescriptor;
    QTcpSocket *tcpSocket;
    QString name;
    std::unique_ptr<Communicator> communicator;
    QDataStream in;
    QByteArray block;
    QDataStream out{&block, QIODevice::WriteOnly};
};
#endif
