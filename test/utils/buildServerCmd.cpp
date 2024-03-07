#include <QDateTime>
#include <google/protobuf/timestamp.pb.h>
#include <google/protobuf/util/time_util.h>

#include "buildServerCmd.h"
#include "proto/serverMsgFormats.pb.h"
#include "uniqueKeysTypedef.h"

#include <iostream>

ReferenceServerMessageBuilder::ReferenceServerMessageBuilder(const SequenceNumber seqNr, const ChatKey chatKey, const ParticipantKey participantKey, std::string participantName, std::string testMessageContent, QDateTime timestamp, std::vector<unsigned> participantKeys)
    : seqNr(seqNr), chatKey(chatKey), participantKey(participantKey), participantName(std::move(participantName)), testMessageContent(std::move(testMessageContent)), timestamp(std::move(timestamp)), participantKeys(std::move(participantKeys)) {}

std::string ReferenceServerMessageBuilder::getAddedToChatCmd() const
{
    ServerCommand serverCmd;
    serverCmd.set_sequencenr(seqNr);
    serverCmd.set_cmd(ServerCommandId::ServerChatCommand);

    auto *chatCmd = serverCmd.mutable_chatcmd();
    chatCmd->set_cmd(ServerChatCommandId::AddedToChat);

    chatCmd->set_chatkey(chatKey);

    return serverCmd.SerializeAsString();
}

std::string ReferenceServerMessageBuilder::getNewIncomingMessageCmd()
{
    ServerCommand serverCmd;
    serverCmd.set_sequencenr(seqNr);
    serverCmd.set_cmd(ServerCommandId::ServerMessageCommand);

    auto *msgCmd = serverCmd.mutable_msgcmd();
    msgCmd->set_chatkey(chatKey);
    msgCmd->set_participantkey(participantKey);
    msgCmd->set_content(testMessageContent);

    msgCmd->mutable_timestamp()->set_seconds(timestamp.toSecsSinceEpoch());

    return serverCmd.SerializeAsString();
}

std::string ReferenceServerMessageBuilder::getRemovedFromChatCmd() const
{
    ServerCommand serverCmd;
    serverCmd.set_sequencenr(seqNr);
    serverCmd.set_cmd(ServerCommandId::ServerChatCommand);

    auto *chatCmd = serverCmd.mutable_chatcmd();
    chatCmd->set_cmd(ServerChatCommandId::LeftChat);
    chatCmd->set_chatkey(chatKey);

    return serverCmd.SerializeAsString();
}

std::string ReferenceServerMessageBuilder::getGenericResponseCmd(ResponseCode response) const
{
    ServerCommand serverCmd;
    serverCmd.set_sequencenr(seqNr);
    serverCmd.set_cmd(ServerCommandId::ServerGenericResponse);
    serverCmd.set_response(response);
    return serverCmd.SerializeAsString();
}

std::string ReferenceServerMessageBuilder::getParticipantAddedCmd() const
{
    ServerCommand serverCmd;
    serverCmd.set_sequencenr(seqNr);
    serverCmd.set_cmd(ServerCommandId::ServerParticipantCommand);

    auto *participantCmd = serverCmd.mutable_participantcmd();
    participantCmd->set_cmd(ServerParticipantCommandId::Added);
    participantCmd->set_participantkey(participantKey);

    return serverCmd.SerializeAsString();
}

std::string ReferenceServerMessageBuilder::getParticipantRemovedCmd() const
{
    ServerCommand serverCmd;
    serverCmd.set_sequencenr(seqNr);
    serverCmd.set_cmd(ServerCommandId::ServerParticipantCommand);

    auto *participantCmd = serverCmd.mutable_participantcmd();
    participantCmd->set_cmd(ServerParticipantCommandId::Removed);
    participantCmd->set_participantkey(participantKey);

    return serverCmd.SerializeAsString();
}

std::string ReferenceServerMessageBuilder::getParticipantNameCmd()
{
    ServerCommand serverCmd;
    serverCmd.set_sequencenr(seqNr);
    serverCmd.set_cmd(ServerCommandId::ServerParticipantCommand);

    auto *participantCmd = serverCmd.mutable_participantcmd();
    participantCmd->set_cmd(ServerParticipantCommandId::SendName);
    participantCmd->set_participantkey(participantKey);
    participantCmd->set_name(participantName);

    return serverCmd.SerializeAsString();
}

std::string ReferenceServerMessageBuilder::getParticipantEntryDateCmd()
{
    ServerCommand serverCmd;
    serverCmd.set_sequencenr(seqNr);
    serverCmd.set_cmd(ServerCommandId::ServerParticipantCommand);

    auto *participantCmd = serverCmd.mutable_participantcmd();
    participantCmd->set_cmd(ServerParticipantCommandId::SendEntryDate);
    participantCmd->set_participantkey(participantKey);

    participantCmd->mutable_timestamp()->set_seconds(timestamp.toSecsSinceEpoch());

    return serverCmd.SerializeAsString();
}

std::string ReferenceServerMessageBuilder::getParticipantAddedToChatCmd() const
{
    ServerCommand serverCmd;
    serverCmd.set_sequencenr(seqNr);
    serverCmd.set_cmd(ServerCommandId::ServerChatCommand);

    auto *chatCmd = serverCmd.mutable_chatcmd();
    chatCmd->set_cmd(ServerChatCommandId::ParticipantAddedToChat);
    chatCmd->set_chatkey(chatKey);
    chatCmd->set_participantkey(participantKey);

    return serverCmd.SerializeAsString();
}
std::string ReferenceServerMessageBuilder::getParticipantLeftChatCmd() const
{
    ServerCommand serverCmd;
    serverCmd.set_sequencenr(seqNr);
    serverCmd.set_cmd(ServerCommandId::ServerChatCommand);

    auto *chatCmd = serverCmd.mutable_chatcmd();
    chatCmd->set_cmd(ServerChatCommandId::ParticipantLeftChat);
    chatCmd->set_chatkey(chatKey);
    chatCmd->set_participantkey(participantKey);

    return serverCmd.SerializeAsString();
}

std::string ReferenceServerMessageBuilder::getParticipantKeys()
{
    ServerCommand serverCmd;
    serverCmd.set_sequencenr(seqNr);
    serverCmd.set_cmd(ServerCommandId::ServerParticipantKeyListCommand);

    auto *keyList = serverCmd.mutable_participantkeylist();
    keyList->set_chatkey(chatKey);

    for (auto key : participantKeys)
    {
        keyList->add_participantkeys(key);
    }

    return serverCmd.SerializeAsString();
}

std::string ReferenceServerMessageBuilder::getMalformedMsg()
{
    ServerCommand serverCmd;
    serverCmd.set_sequencenr(seqNr);
    return serverCmd.SerializeAsString();
}

std::string ReferenceServerMessageBuilder::getMalformedChatMsg()
{
    ServerCommand serverCmd;
    serverCmd.set_sequencenr(seqNr);
    serverCmd.set_cmd(ServerCommandId::ServerChatCommand);
    return serverCmd.SerializeAsString();
}

std::string ReferenceServerMessageBuilder::getMalformedParticipantMsg()
{
    ServerCommand serverCmd;
    serverCmd.set_sequencenr(seqNr);
    serverCmd.set_cmd(ServerCommandId::ServerParticipantCommand);
    return serverCmd.SerializeAsString();
}

std::string ReferenceServerMessageBuilder::getMalformedMessageMsg()
{
    ServerCommand serverCmd;
    serverCmd.set_sequencenr(seqNr);
    serverCmd.set_cmd(ServerCommandId::ServerMessageCommand);
    return serverCmd.SerializeAsString();
}
