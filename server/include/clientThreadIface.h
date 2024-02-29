#ifndef D5243FA9_F759_41DC_AA08_5DC547F4B3CD
#define D5243FA9_F759_41DC_AA08_5DC547F4B3CD
#include "server.h"
#include <memory>
#include <string>

#include <QTcpSocket>

#include "chat.h"
#include "communicators.h"
class ServerParticipant;

class ClientThreadIface
{
public:
    ClientThreadIface(const Server &server) : server(server)
    {
        participant = std::make_unique<ServerParticipant>();
    }
    virtual void setNickname(std::string) = 0;
    virtual void requestLeavingChat(ChatKey &key) = 0;
    virtual std::vector<ParticipantKey> requestChatParticipantKeys(ChatKey &key) = 0;
    virtual ChatKey requestNewChat() = 0;
    const Server &getServer() const { return server; }
    ServerParticipant &getParticipant() const { return *participant; }

protected:
    std::unique_ptr<Communicator> communicator;
    std::unique_ptr<ServerParticipant> participant;

private:
    const Server &server;
};

#endif /* D5243FA9_F759_41DC_AA08_5DC547F4B3CD */
