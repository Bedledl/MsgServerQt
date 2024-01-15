#include "server.h"
#include "clientThread.h"

#include <QObject>

void Server::create_new_client_thread(qintptr socketDescriptor) {
    ClientThread *thread = new ClientThread(socketDescriptor, this);
    QObject::connect(thread, &ClientThread::finished, thread, &ClientThread::deleteLater);
    thread->start();
}
