#include "include/buildClientCmd.h"
#include "proto/clientMsgFormats.pb.h"
#include <string>

ReferenceClientMessageBuilder::ReferenceClientMessageBuilder(SequenceNumber seqNr, ChatKey chatKey, ParticipantKey participantKey, std::string participantName, std::string testMessageContent, QDateTime timestamp)
    : seqNr(seqNr), chatKey(chatKey), participantKey(participantKey), participantName(std::move(participantName)), testMessageContent(std::move(testMessageContent)), timestamp(std::move(timestamp)) {}

std::string ReferenceClientMessageBuilder::setNickname()
{
    ClientCommand clientCmd;
    clientCmd.set_sequencenr(seqNr);
    clientCmd.set_cmd(ClientCommandId::SetNickname);
    clientCmd.set_name(participantName);
    return clientCmd.SerializeAsString();
}
std::string ReferenceClientMessageBuilder::requestNewChat() const
{
    ClientCommand clientCmd;
    clientCmd.set_sequencenr(seqNr);
    clientCmd.set_cmd(ClientCommandId::NewChat);

    return clientCmd.SerializeAsString();
}
std::string ReferenceClientMessageBuilder::requestChatParticipantKeys() const
{
    ClientCommand clientCmd;
    clientCmd.set_sequencenr(seqNr);
    clientCmd.set_cmd(ClientCommandId::ClientChatCommand);

    auto *clientChatCmd = new ClientCommand_ClientChatCommand();
    clientChatCmd->set_cmd(ClientChatCommandId::GetParticipantKeys);
    clientChatCmd->set_chatkey(chatKey);

    clientCmd.set_allocated_chatcmd(clientChatCmd);
    return clientCmd.SerializeAsString();
}
std::string ReferenceClientMessageBuilder::requestLeavingChat() const
{
    ClientCommand clientCmd;
    clientCmd.set_sequencenr(seqNr);
    clientCmd.set_cmd(ClientCommandId::ClientChatCommand);

    auto *clientChatCmd = new ClientCommand_ClientChatCommand();
    clientChatCmd->set_cmd(ClientChatCommandId::LeaveChat);
    clientChatCmd->set_chatkey(chatKey);

    clientCmd.set_allocated_chatcmd(clientChatCmd);
    return clientCmd.SerializeAsString();
}
std::string ReferenceClientMessageBuilder::requestParticipantName() const
{
    ClientCommand clientCmd;
    clientCmd.set_sequencenr(seqNr);
    clientCmd.set_cmd(ClientCommandId::ClientParticipantCommand);

    auto *clientParticipantCmd = new ClientCommand_ClientParticipantCommand();
    clientParticipantCmd->set_cmd(ClientParticipantCommandId::GetName);
    clientParticipantCmd->set_participantkey(participantKey);

    clientCmd.set_allocated_participantcmd(clientParticipantCmd);

    return clientCmd.SerializeAsString();
}
std::string ReferenceClientMessageBuilder::requestParticipantEntryDate() const
{
    ClientCommand clientCmd;
    clientCmd.set_sequencenr(seqNr);
    clientCmd.set_cmd(ClientCommandId::ClientParticipantCommand);

    auto *clientParticipantCmd = new ClientCommand_ClientParticipantCommand();
    clientParticipantCmd->set_cmd(ClientParticipantCommandId::GetEntryDate);
    clientParticipantCmd->set_participantkey(participantKey);

    clientCmd.set_allocated_participantcmd(clientParticipantCmd);
    return clientCmd.SerializeAsString();
}
std::string ReferenceClientMessageBuilder::requestAddingParticipantToChat() const
{
    ClientCommand clientCmd;
    clientCmd.set_sequencenr(seqNr);
    clientCmd.set_cmd(ClientCommandId::ClientParticipantCommand);

    auto *clientParticipantCmd = new ClientCommand_ClientParticipantCommand();
    clientParticipantCmd->set_cmd(ClientParticipantCommandId::AddToChat);
    clientParticipantCmd->set_participantkey(participantKey);
    clientParticipantCmd->set_chatkey(chatKey);

    clientCmd.set_allocated_participantcmd(clientParticipantCmd);
    return clientCmd.SerializeAsString();
}

std::string ReferenceClientMessageBuilder::sendNewMsg()
{
    ClientCommand clientCmd;
    clientCmd.set_sequencenr(seqNr);
    clientCmd.set_cmd(ClientCommandId::ClientMessageCommand);

    auto *clientMsgCmd = clientCmd.mutable_msgcmd();
    clientMsgCmd->set_chatkey(chatKey);
    clientMsgCmd->set_content(testMessageContent);

    auto *ts = clientMsgCmd->mutable_timestamp();
    ts->set_seconds(timestamp.toSecsSinceEpoch());
    return clientCmd.SerializeAsString();
}

std::string ReferenceClientMessageBuilder::genericResponseSuccess() const
{
    ClientCommand clientCmd;
    clientCmd.set_sequencenr(seqNr);
    clientCmd.set_cmd(ClientCommandId::ClientGenericResponse);
    clientCmd.set_response(ResponseCode::SUCCESS);
    return clientCmd.SerializeAsString();
}

std::string ReferenceClientMessageBuilder::genericResponseError() const
{
    ClientCommand clientCmd;
    clientCmd.set_sequencenr(seqNr);
    clientCmd.set_cmd(ClientCommandId::ClientGenericResponse);
    clientCmd.set_response(ResponseCode::ERROR);
    return clientCmd.SerializeAsString();
}

std::string ReferenceClientMessageBuilder::genericResponseMalformed() const
{
    ClientCommand clientCmd;
    clientCmd.set_sequencenr(seqNr);
    clientCmd.set_cmd(ClientCommandId::ClientGenericResponse);
    clientCmd.set_response(ResponseCode::MALFORMED_MESSAGE);
    return clientCmd.SerializeAsString();
}
