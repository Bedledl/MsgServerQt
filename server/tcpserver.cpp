#include "include/server.h"

#include <iostream>
#include <QNetworkInterface>

TCPMessageServer::TCPMessageServer(QObject *parent) : QTcpServer(parent), Server(parent) {

    // copied from https://code.qt.io/cgit/qt/qtbase.git/tree/examples/network/fortuneserver/server.cpp?h=6.6
    if (!listen()) {
        std::cout << "Unable to start the server" << std::endl;
        // QMessageBox::critical(this, tr("Messager"),
        //                       tr: %1.")
        //                       .arg(errorString()));
        close();
        return;
    }

    QString ipAddress;
    const QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (const QHostAddress &entry : ipAddressesList) {
        if (entry != QHostAddress::LocalHost && entry.toIPv4Address()) {
            ipAddress = entry.toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    // statusLabel->setText(tr("The server is running on\n\nIP: %1\nport: %2\n\n"
    //                         "Run the Fortune Client example now.")
    //                      .arg(ipAddress).arg(serverPort()));
    std::cout << "The server is running on\n\nIP: %1\nport: %2\n\n"
                             "Run the Fortune Client example now."<< std::endl;// << ipAddress << serverPort();

}

void TCPMessageServer::incomingConnection(qintptr socketDescriptor) {
    create_new_client_thread(socketDescriptor);
}


// bool TCPMessageServer::hasPendingConnections() const {
//     return true;
// }