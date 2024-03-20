#include "tcpClientConnector.h"

#include <QObject>
#include <QTcpSocket>

#include <QDebug>

TcpClientConnector::TcpClientConnector(const QHostAddress &ip, quint16 port, std::function<void(std::string)> incomingMsgCallback, QObject *parent)
    : QObject(parent), tcpSocket(std::make_unique<QTcpSocket>(this)), remoteIpString(ip.toString()), remotePort(port), incomingMsgCallback(incomingMsgCallback)
{
    tcpSocket->connectToHost(ip, port);
    in.setDevice(tcpSocket.get());
    in.setVersion(QDataStream::Qt_6_6);
    connect(tcpSocket.get(), &QIODevice::readyRead, this, &TcpClientConnector::readFromSocketAndCallCb);
    connect(tcpSocket.get(), &QAbstractSocket::errorOccurred,
            [&](QAbstractSocket::SocketError /*socketError*/)
            { qDebug() << "TcpClientConnector.tcpSocket.errorOccurred: " << tcpSocket->errorString(); });

    out.setVersion(QDataStream::Qt_6_6);
}

void TcpClientConnector::sendMsgToServer(std::string msgString)
{
    out << QString::fromStdString(msgString);
    auto written = tcpSocket->write(block);
    qInfo() << "TcpClientConnector.sendMsgToServer: wrote " << written << " bytes to socket";
    if (written == -1)
    {
        qCritical() << "TcpClientConnector.sendMsgToServer: failed to write to socket";
    }
    if (written != msgString.length())
    {
        qWarning() << "TcpClientConnector.sendMsgToServer: wrote " << written << " bytes to socket, but message length is " << msgString.length();
    }
}

void TcpClientConnector::readFromSocketAndCallCb()
{
    in.startTransaction();
    QString msg;
    in >> msg;
    if (!in.commitTransaction())
    {
        qCritical() << "TcpClientConnector.readFromSocketAndAswer: failed to commit transaction";
        return;
    }

    if (msg.length() == 0)
    {
        qDebug() << "TcpClientConnector: readFromSocketAndAnswer: Received empty message, won't process it.";
        return;
    }

    incomingMsgCallback(msg.toStdString());
}
