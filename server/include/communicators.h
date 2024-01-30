#ifndef SERVER_COMMUNICATORS_H
#define SERVER_COMMUNICATORS_H
#include <QString>
#include <memory>

#include "communicator.h"

class Server;

class ServerCommunicator : public Communicator
{
    Server *server;

public:
    ServerCommunicator(Server *server) : server(server) {}
    QString answerMessage(QString msg) override;
    QString welcomeMessage() override;
};

#endif
