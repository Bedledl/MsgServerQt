#include <QThread>
#include <QTcpSocket>

#include "communicators.h"

//https://doc.qt.io/qt-6/qtnetwork-fortuneclient-example.html



class ClientThread : public QThread {
    Q_OBJECT

    public:
        ClientThread(qintptr socketDescriptor, QObject *parent);
        void run() override;
        QString get_name() const { return name; }
    signals:
        void error(QTcpSocket::SocketError socketError);
    private:
        qintptr socketDescriptor;
        QString name;
        Communicator communicator;
};