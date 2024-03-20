#ifndef A4255B2F_8114_4123_B565_3E7521DF4A15
#define A4255B2F_8114_4123_B565_3E7521DF4A15
#include <QIODevice>
#include <QObject>
#include <QTcpSocket>
#include <memory>
#include <string>

#include <functional>

QT_BEGIN_NAMESPACE
class QHostAddress;
QT_END_NAMESPACE

class ClientConnector
{
public:
    virtual void sendMsgToServer(std::string msgString) = 0;
};

class TcpClientConnector : public QObject, public ClientConnector
{
public:
    TcpClientConnector(const QHostAddress &ip, quint16 port, std::function<void(std::string)> incomingMsgCallback, QObject *parent = nullptr);
    void sendMsgToServer(std::string msgString) override;
    [[nodiscard]] std::string getRemoteIpString() const { return remoteIpString.toStdString(); }
    [[nodiscard]] uint16_t getRemotePort() const { return remotePort; }
private slots:
    void readFromSocketAndCallCb();

    std::unique_ptr<QTcpSocket> tcpSocket;
    QDataStream in;
    QByteArray block;
    QDataStream out{&block, QIODevice::WriteOnly};
    const QString remoteIpString;
    const quint16 remotePort;
    std::function<void(std::string)> incomingMsgCallback;
};

#endif /* A4255B2F_8114_4123_B565_3E7521DF4A15 */
