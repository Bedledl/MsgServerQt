#include "server.h"
#include "clientThreadWorker.h"

void Server::create_new_client_thread(Worker *worker)
{
    auto *thread = new QThread();
    worker->moveToThread(thread);

    QThread::connect(thread, &QThread::started, worker, &Worker::initialize);
    QThread::connect(thread, &QThread::finished, worker, &Worker::deleteLater);
    QThread::connect(worker, &Worker::finished, thread, &QThread::quit);

    thread->start();
}
