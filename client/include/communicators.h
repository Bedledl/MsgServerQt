#ifndef CLIENT_COMMUNICATORS_H
#define CLIENT_COMMUNICATORS_H

#include "clientMsgFormats.pb.h"
#include "communicator.h"
#include "serverMsgFormats.pb.h"
#include <QDate>
#include <QString>

class Client;

class ClientCommunicator : public Communicator
{
public:
    ClientCommunicator(Client &client) : client(client) {}
    QString answerMessage(QString msg);

    QString welcomeMessage() override;

private:
    std::string generateGenericResponseString(ResponseCode response)
    {
        ClientCommand serverCmd;
        serverCmd.set_cmd(ClientCommandId::ClientGenericResponse);
        auto data = new Data();
        data->set_response(response);
        serverCmd.set_allocated_data(data);
        return serverCmd.SerializeAsString();
    }

    Client &client;
};

#endif
