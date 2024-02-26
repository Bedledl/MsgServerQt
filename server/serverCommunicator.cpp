#include "clientMsgFormats.pb.h"
#include "communicators.h"
#include "participant.h"
#include "server.h"
#include "serverMsgFormats.pb.h"
#include <QString>

void ServerCommunicator::buildGenericResponse(ServerCommand &cmdBuf, ResponseCode code)
{
    cmdBuf.set_cmd(ServerCommandId::ServerGenericResponse);
    cmdBuf.set_response(code);
}

std::string ServerCommunicator::answerMessage(std::string msg)
{
    ClientCommand incomingCmd;
    incomingCmd.ParseFromString(msg);

    ServerCommand outgoingCmd;

    std::cout << "Incoming Client Message: ";

    switch (incomingCmd.cmd())
    {

    case ClientCommandId::SetNickname:
    {
        auto name = incomingCmd.msgcmd().content();
        participant->setNickname(QString::fromStdString(name));
        buildGenericResponse(outgoingCmd, ResponseCode::SUCCESS);
        std::cout << "Client Set Nickname Cmd\n";
        break;
    }

    case ClientCommandId::NewChat:
    {
        auto newChatKey = participant->newChat();

        outgoingCmd.set_cmd(ServerCommandId::ServerChatCommand);
        auto chatCmd = new ServerCommand_ServerChatCommand();

        chatCmd->set_cmd(ServerChatCommandId::AddedToChat);
        chatCmd->set_chatkey(newChatKey);

        outgoingCmd.set_allocated_chatcmd(chatCmd);

        std::cout << "Client New Chat Cmd\n";
        break;
    }

        // case ClientCommandId::GetChatKeys:
        // {
        //     // TODO
        //     std::cout << "Client Get Chat Keys Cmd\n";
        //     std::cout << "Acually, i don't want to use this method, because i want to prevent sending multiple chat keys at once?\n";

        //     break;
        // }

    case ClientCommandId::ClientGenericResponse:
    {
        auto inResponseCode = incomingCmd.response();
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
        auto chatKey = chatCmd.chatkey();
        switch (chatCmd.cmd())
        {

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
            // TODO send malformed apckages
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
        }
        break;

        default:
        {
            std::cout << "Unknown Message\n";
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
    sleep(10);
    return outgoing.SerializeAsString();
}

QString ServerCommunicator::welcomeMessage()
{
    ServerCommand addedToGlobalChat;
    addedToGlobalChat.set_cmd(ServerCommandId::ServerChatCommand);

    auto *internalAddedToGlobalChat = new ServerCommand_ServerChatCommand();
    auto globalChat = server->getGlobalChat();
    internalAddedToGlobalChat->set_chatkey(int32_t(globalChat->getKey()));
    internalAddedToGlobalChat->set_cmd(ServerChatCommandId::AddedToChat);
    addedToGlobalChat.set_allocated_chatcmd(internalAddedToGlobalChat);

    return QString::fromStdString(addedToGlobalChat.SerializeAsString());
}
