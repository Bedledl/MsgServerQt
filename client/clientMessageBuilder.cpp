#include <QDateTime>
#include <google/protobuf/util/time_util.h>

#include "clientMessageBuilder.h"
#include "proto/clientMsgFormats.pb.h"

std::string ClientMessageBuilder::getSendNewMsg(SequenceNumber seqNr, ChatKey chatKey, std::string msg)
{
    ClientCommand ccc;
    ccc.set_sequencenr(seqNr);
    ccc.set_cmd(ClientCommandId::ClientMessageCommand);

    auto *newMsg = ccc.mutable_msgcmd();
    newMsg->set_chatkey(chatKey);
    newMsg->set_content(msg);

    QDateTime datetime = QDateTime::currentDateTime();
    newMsg->mutable_timestamp()->set_seconds(datetime.toSecsSinceEpoch());

    return ccc.SerializeAsString();
}
std::string ClientMessageBuilder::getRequestNewChat(SequenceNumber seqNr)
{
    ClientCommand ccc;
    ccc.set_sequencenr(seqNr);
    ccc.set_cmd(ClientCommandId::NewChat);
    return ccc.SerializeAsString();
}
std::string ClientMessageBuilder::getRequestParticipantKeys(SequenceNumber seqNr, ChatKey chatKey)
{
    ClientCommand clientCmd;
    clientCmd.set_sequencenr(seqNr);
    clientCmd.set_cmd(ClientCommandId::ClientChatCommand);

    auto *clientChatCmd = clientCmd.mutable_chatcmd();
    clientChatCmd->set_cmd(ClientChatCommandId::GetParticipantKeys);
    clientChatCmd->set_chatkey(chatKey);
    return clientCmd.SerializeAsString();
}
std::string ClientMessageBuilder::getSetNewNickname(SequenceNumber seqNr, std::string newName)
{
    ClientCommand ccc;
    ccc.set_sequencenr(seqNr);
    ccc.set_cmd(ClientCommandId::SetNickname);
    ccc.set_name(newName);
    return ccc.SerializeAsString();
}
std::string ClientMessageBuilder::getRequestAddingParticipantToChat(SequenceNumber seqNr, ChatKey chatKey, ParticipantKey participantKey)
{
    ClientCommand ccc;
    ccc.set_sequencenr(seqNr);
    ccc.set_cmd(ClientCommandId::ClientParticipantCommand);

    auto *participantCmd = ccc.mutable_participantcmd();
    participantCmd->set_cmd(ClientParticipantCommandId::AddToChat);
    participantCmd->set_chatkey(chatKey);
    participantCmd->set_participantkey(participantKey);
    return ccc.SerializeAsString();
}
std::string ClientMessageBuilder::getRequestLeaveChat(SequenceNumber seqNr, ChatKey chatKey)
{
    ClientCommand clientCmd;
    clientCmd.set_sequencenr(seqNr);
    clientCmd.set_cmd(ClientCommandId::ClientChatCommand);

    auto *clientChatCmd = clientCmd.mutable_chatcmd();
    clientChatCmd->set_cmd(ClientChatCommandId::LeaveChat);
    clientChatCmd->set_chatkey(chatKey);
    return clientCmd.SerializeAsString();
}
std::string ClientMessageBuilder::getRequestParticipantName(SequenceNumber seqNr, ParticipantKey participantKey)
{
    ClientCommand clientCmd;
    clientCmd.set_sequencenr(seqNr);
    clientCmd.set_cmd(ClientCommandId::ClientParticipantCommand);

    auto *clientParticipantCmd = clientCmd.mutable_participantcmd();
    clientParticipantCmd->set_cmd(ClientParticipantCommandId::GetName);
    clientParticipantCmd->set_participantkey(participantKey);

    return clientCmd.SerializeAsString();
}
std::string ClientMessageBuilder::getRequestParticipantEntryDate(SequenceNumber seqNr, ParticipantKey participantKey)
{
    ClientCommand clientCmd;
    clientCmd.set_sequencenr(seqNr);
    clientCmd.set_cmd(ClientCommandId::ClientParticipantCommand);

    auto *clientParticipantCmd = clientCmd.mutable_participantcmd();
    clientParticipantCmd->set_cmd(ClientParticipantCommandId::GetEntryDate);
    clientParticipantCmd->set_participantkey(participantKey);
    return clientCmd.SerializeAsString();
}
std::string ClientMessageBuilder::getGenericResponseSuccess(SequenceNumber seqNr)
{
    ClientCommand ccc;
    ccc.set_sequencenr(seqNr);
    ccc.set_cmd(ClientCommandId::ClientGenericResponse);
    ccc.set_response(ResponseCode::SUCCESS);
    return ccc.SerializeAsString();
}
std::string ClientMessageBuilder::getGenericResponseError(SequenceNumber seqNr)
{
    ClientCommand ccc;
    ccc.set_sequencenr(seqNr);
    ccc.set_cmd(ClientCommandId::ClientGenericResponse);
    ccc.set_response(ResponseCode::ERROR);
    return ccc.SerializeAsString();
}
std::string ClientMessageBuilder::getGenericResponseMalformed(SequenceNumber seqNr)
{
    ClientCommand ccc;
    ccc.set_sequencenr(seqNr);
    ccc.set_cmd(ClientCommandId::ClientGenericResponse);
    ccc.set_response(ResponseCode::MALFORMED_MESSAGE);
    return ccc.SerializeAsString();
}
