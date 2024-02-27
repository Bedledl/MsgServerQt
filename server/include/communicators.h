#ifndef SERVER_COMMUNICATORS_H
#define SERVER_COMMUNICATORS_H
#include <QString>
#include <memory>
#include <string>

#include "communicator.h"

class ClientThreadIface;
class ServerCommand;
enum ResponseCode : int;

class ServerCommunicator : public Communicator
{

public:
    std::string answerMessage(std::string) override;
    ServerCommunicator(ClientThreadIface &clientThreadIface) : clientThreadWorker(clientThreadIface) {}
    QString welcomeMessage() override;

private:
    ClientThreadIface &clientThreadWorker;
    void buildGenericResponse(ServerCommand &cmdBuf, ResponseCode code);
};

#endif
