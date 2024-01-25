#ifndef CLIENT_H
#define CLIENT_H

#include <QAbstractSocket>
#include <QObject>

#include "communicators.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QTcpSocket;
QT_END_NAMESPACE

class ClientFailedToConnect : public std::exception
{
    virtual const char *what() const throw()
    {
        return "Failed to connect.";
    }
};

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QHostAddress ip, quint16 port, QObject *parent = nullptr);
private slots:
    void readFromSocketAndAswer();

private:
    QLabel *statusLabel = nullptr;
    QTcpSocket *tcpSocket = nullptr;
    QDataStream in;
    QByteArray block;
    QDataStream out{&block, QIODevice::WriteOnly};
    Communicator *communicator;
};

#endif