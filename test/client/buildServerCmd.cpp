#include <QDateTime>
#include <google/protobuf/timestamp.pb.h> // Include the header file for the Timestamp class
#include <google/protobuf/util/time_util.h>

#include "buildServerCmd.h"

#include <iostream>

TestServerCommunicator::TestServerCommunicator(int chatKey, int participantKey, std::string participantName, std::string testMessageContent, QDateTime currentDateTime)
    : chatKey(chatKey), participantKey(participantKey), participantName(participantName), testMessageContent(testMessageContent) {}

std::string TestServerCommunicator::getAddedToChatCmd()
{
    ServerCommand serverCmd;
    serverCmd.set_cmd(ServerCommandId::ServerChatCommand);

    auto chatCmd = new ServerCommand_ServerChatCommand();
    chatCmd->set_cmd(ServerChatCommandId::AddedToChat);

    chatCmd->set_chatkey(chatKey);

    serverCmd.set_allocated_chatcmd(chatCmd);
    return serverCmd.SerializeAsString();
}

std::string TestServerCommunicator::getNewIncomingMessageCmd()
{
    ServerCommand serverCmd;
    serverCmd.set_cmd(ServerCommandId::ServerMessageCommand);

    auto msgCmd = new ServerCommand_ServerMessageCommand();
    msgCmd->set_chatkey(chatKey);
    msgCmd->set_participantkey(participantKey);
    msgCmd->set_content(testMessageContent);

    auto ts = new google::protobuf::Timestamp(); // Create an instance of the Timestamp class
    ts->set_seconds(timestamp.toSecsSinceEpoch());
    msgCmd->set_allocated_timestamp(ts);

    serverCmd.set_allocated_msgcmd(msgCmd);

    return serverCmd.SerializeAsString();
}

std::string TestServerCommunicator::getRemovedFromChatCmd()
{
    ServerCommand serverCmd;
    serverCmd.set_cmd(ServerCommandId::ServerChatCommand);

    auto chatCmd = new ServerCommand_ServerChatCommand();
    chatCmd->set_cmd(ServerChatCommandId::LeftChat);

    chatCmd->set_chatkey(chatKey);

    serverCmd.set_allocated_chatcmd(chatCmd);
    return serverCmd.SerializeAsString();
}

std::string TestServerCommunicator::getGenericResponseCmd(ResponseCode response)
{
    ServerCommand serverCmd;
    serverCmd.set_cmd(ServerCommandId::ServerGenericResponse);
    serverCmd.set_response(response);
    return serverCmd.SerializeAsString();
}

std::string TestServerCommunicator::getParticipantAddedCmd()
{
    ServerCommand serverCmd;
    serverCmd.set_cmd(ServerCommandId::ServerParticipantCommand);

    auto participantCmd = new ServerCommand_ServerParticipantCommand();
    participantCmd->set_cmd(ServerParticipantCommandId::Added);
    participantCmd->set_participantkey(participantKey);

    serverCmd.set_allocated_participantcmd(participantCmd);
    return serverCmd.SerializeAsString();
}

std::string TestServerCommunicator::getParticipantRemovedCmd()
{
    ServerCommand serverCmd;
    serverCmd.set_cmd(ServerCommandId::ServerParticipantCommand);

    auto participantCmd = new ServerCommand_ServerParticipantCommand();
    participantCmd->set_cmd(ServerParticipantCommandId::Removed);
    participantCmd->set_participantkey(participantKey);

    serverCmd.set_allocated_participantcmd(participantCmd);
    return serverCmd.SerializeAsString();
}

std::string TestServerCommunicator::getParticipantNameCmd()
{
    ServerCommand serverCmd;
    serverCmd.set_cmd(ServerCommandId::ServerParticipantCommand);

    auto participantCmd = new ServerCommand_ServerParticipantCommand();
    participantCmd->set_cmd(ServerParticipantCommandId::SendName);
    participantCmd->set_participantkey(participantKey);
    participantCmd->set_name(participantName);

    serverCmd.set_allocated_participantcmd(participantCmd);
    return serverCmd.SerializeAsString();
}

std::string TestServerCommunicator::getParticipantEntryDateCmd()
{
    ServerCommand serverCmd;
    serverCmd.set_cmd(ServerCommandId::ServerParticipantCommand);

    auto participantCmd = new ServerCommand_ServerParticipantCommand();
    participantCmd->set_cmd(ServerParticipantCommandId::SendName);
    participantCmd->set_participantkey(participantKey);

    auto ts = new google::protobuf::Timestamp();
    ts->set_seconds(QDateTime::currentDateTime().toSecsSinceEpoch());
    participantCmd->set_allocated_timestamp(ts);

    serverCmd.set_allocated_participantcmd(participantCmd);
    return serverCmd.SerializeAsString();
}

std::string TestServerCommunicator::getParticipantAddedToChatCmd()
{
    ServerCommand serverCmd;
    serverCmd.set_cmd(ServerCommandId::ServerChatCommand);

    auto chatCmd = new ServerCommand_ServerChatCommand();
    chatCmd->set_cmd(ServerChatCommandId::ParticipantAddedToChat);

    chatCmd->set_chatkey(chatKey);
    chatCmd->set_participantkey(participantKey);

    serverCmd.set_allocated_chatcmd(chatCmd);
    return serverCmd.SerializeAsString();
}
std::string TestServerCommunicator::getParticipantLeftChatCmd()
{
    ServerCommand serverCmd;
    serverCmd.set_cmd(ServerCommandId::ServerChatCommand);

    auto chatCmd = new ServerCommand_ServerChatCommand();
    chatCmd->set_cmd(ServerChatCommandId::ParticipantLeftChat);

    chatCmd->set_chatkey(chatKey);
    chatCmd->set_participantkey(participantKey);

    serverCmd.set_allocated_chatcmd(chatCmd);
    return serverCmd.SerializeAsString();
}
