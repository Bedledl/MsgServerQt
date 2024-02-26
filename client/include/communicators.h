#ifndef CLIENT_COMMUNICATORS_H
#define CLIENT_COMMUNICATORS_H

#include "clientIface.h"
#include "clientMsgFormats.pb.h"
#include "communicator.h"
#include "serverMsgFormats.pb.h"
#include <QDate>
#include <QString>

class Client;
#include <string>

class ClientCommunicator : public Communicator
{
public:
    ClientCommunicator(ClientIface &client) : client(client) {}
    std::string answerMessage(std::string msg) override;

    QString welcomeMessage() override;

private:
    std::string generateGenericResponseString(ResponseCode response)
    {
        ClientCommand serverCmd;
        serverCmd.set_cmd(ClientCommandId::ClientGenericResponse);
        serverCmd.set_response(response);
        return serverCmd.SerializeAsString();
    }

    ClientIface &client;
};

#endif
