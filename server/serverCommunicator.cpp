#include "clientMsgFormats.pb.h"
#include "communicators.h"
#include "server.h"
#include "serverMsgFormats.pb.h"
#include <QString>

QString ServerCommunicator::answerMessage(QString msg)
{
    return msg;
}

QString ServerCommunicator::welcomeMessage()
{
    std::cout << "generate welcome message\n";
    ServerCommand addedToGlobalChat;
    addedToGlobalChat.set_cmd(ServerCommandId::ServerChatCommand);

    auto *internalAddedToGlobalChat = new ServerCommand_ServerChatCommand();
    auto globalChat = server->getGlobalChat();
    internalAddedToGlobalChat->set_chatkey(int32_t(globalChat->getKey()));
    internalAddedToGlobalChat->set_cmd(ServerChatCommandId::AddedToChat);
    addedToGlobalChat.set_allocated_chatcmd(internalAddedToGlobalChat);

    return QString(addedToGlobalChat.SerializeAsString().c_str());
}
