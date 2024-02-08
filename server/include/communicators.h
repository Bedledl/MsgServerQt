#ifndef SERVER_COMMUNICATORS_H
#define SERVER_COMMUNICATORS_H
#include "communicator.h"
#include <QString>
#include <memory>

class ServerParticipant;
class Server;
class ServerCommand;
enum ResponseCode: int;

class ServerCommunicator : public Communicator
{

public:
    ServerCommunicator(Server *server, ServerParticipant *participant) : server(server), participant(participant) {}
    QString answerMessage(QString msg) override;
    QString welcomeMessage() override;

private:
    Server *const server;
    ServerParticipant *const participant;
    void buildGenericResponse(ServerCommand &cmdBuf, ResponseCode code);
};

#endif
