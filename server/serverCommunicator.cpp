#include "clientMsgFormats.pb.h"
#include "communicators.h"
#include "server.h"
#include "serverMsgFormats.pb.h"
#include <QString>

QString ServerCommunicator::answerMessage(QString msg)
{
    ClientCommand incomingCmd;
    incomingCmd.ParseFromString(msg.toStdString());

    std::cout << "Incoming Client Message: ";

    switch (incomingCmd.cmd())
    {

    case ClientCommandId::SetNickname:
    {
        // TODO
        std::cout << "Client Set Nickname Cmd\n";
        break;
    }

    case ClientCommandId::NewChat:
    {
        // TODO
        std::cout << "Client New Chat Cmd\n";
        break;
    }

    case ClientCommandId::GetChatKeys:
    {
        // TODO
        std::cout << "Client Get Chat Keys Cmd\n";
        break;
    }

    case ClientCommandId::ClientGenericResponse:
    {
        auto inResponseCode = incomingCmd.data().response();
        std::cout << "Client Generic Response: ";
        switch (inResponseCode)
        {
        case ResponseCode::SUCCESS:
            std::cout << "Success\n";
            break;

        case ResponseCode::ERROR:
            std::cout << "Error\n";
            break;

        case ResponseCode::MALFORMED_MESSAGE:
            std::cout << "Malformed\n";
            break;

        default:
            std::cout << "Unknown Response Code\n";
        }
        // TODO
    }
    break;

    case ClientCommandId::ClientChatCommand:
    {
        auto chatCmd = incomingCmd.chatcmd();
        switch (chatCmd.cmd())
        {

        case ClientChatCommandId::NewClientMessage:
        {
            std::cout << "New Client Message\n";
            // TODO
            break;
        }

        case ClientChatCommandId::GetParticipantKeys:
        {
            std::cout << "Get Participant Keys\n";
            // TODO
            break;
        }

        case ClientChatCommandId::LeaveChat:
        {
            std::cout << "Leave Chat\n";
            // TODO
            break;
        }

        default:
        {
            std::cout << "Unknown Message\n";
            // TODO
        }
        }
    }
    break;

    case ClientCommandId::ClientParticipantCommand:
    {
        auto partCmd = incomingCmd.participantcmd();
        switch (partCmd.cmd())
        {

        case ClientParticipantCommandId::GetName:
        {
            std::cout << "Get Participant Name\n";
            // TODO
            break;
        }

        case ClientParticipantCommandId::GetEntryDate:
        {
            std::cout << "Get Participant Entry Date\n";
            // TODO
            break;
        }

        case ClientParticipantCommandId::AddToChat:
        {
            std::cout << "Add Participant To Chat\n";
            // TODO
            break;
        }

        default:
        {
            std::cout << "Unknown Message\n";
            break;
            // TODO
        }
        }
    }

    default:
    {
        std::cout << "Unknown Message\n";
        // TODO
    }
    }

    ServerCommand outgoing;
    outgoing.set_cmd(ServerCommandId::ServerGenericResponse);
    return QString(outgoing.SerializeAsString().c_str());
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
