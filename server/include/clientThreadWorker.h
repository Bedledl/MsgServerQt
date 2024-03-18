#ifndef WORKER_H
#define WORKER_H

#include <QDataStream>
#include <QTcpSocket>
#include <QThread>

#include "communicator.h"

// https://doc.qt.io/qt-6/qtnetwork-fortuneclient-example.html

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent) : QObject(parent){};
    ~Worker() override = default;
    ;

public slots:
    virtual void initialize() = 0;
    virtual void sendMsgToClient(QString msg) = 0;
signals:
    void finished();
    void error(QTcpSocket::SocketError socketError);
};

class TCPServerWorker : public Worker
{
    Q_OBJECT
public:
    TCPServerWorker(qintptr socketDescriptor, QObject *parent);
    [[nodiscard]] QString get_name() const { return name; }

public slots:
    void initialize() override;
    void readFromSocket();
    void sendMsgToClient(QString msg) override;

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
