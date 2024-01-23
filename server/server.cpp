#include "server.h"
#include "clientThreadWorker.h"

void Server::create_new_client_thread(Worker *worker)
{
    QThread *thread = new QThread();
    worker->moveToThread(thread);

    thread->connect(thread, &QThread::started, worker, &Worker::process);
    thread->connect(thread, &QThread::finished, worker, &Worker::deleteLater);
    thread->connect(worker, &Worker::finished, thread, &QThread::quit);

    thread->start();
}
