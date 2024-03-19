#include "serverMessageBuilder.h"
#include "QDateTime"
#include "proto/serverClientCommunication.pb.h"
#include "proto/serverMsgFormats.pb.h"
#include <string>

std::string ServerMessageBuilder::buildGenericResponse(SequenceNumber seqNr, ResponseCode response)
{
    ServerCommand sc;
    sc.set_sequencenr(seqNr);
    sc.set_cmd(ServerCommandId::ServerGenericResponse);
    sc.set_response(response);
    return sc.SerializeAsString();
}

std::string ServerMessageBuilder::buildAddedToChat(SequenceNumber seqNr, ChatKey chatKey)
{
    ServerCommand sc;
    sc.set_sequencenr(seqNr);
    sc.set_cmd(ServerCommandId::ServerChatCommand);
    sc.mutable_chatcmd()->set_cmd(ServerChatCommandId::AddedToChat);
    sc.mutable_chatcmd()->set_chatkey(chatKey);
    return sc.SerializeAsString();
}
std::string ServerMessageBuilder::buildLeftChat(SequenceNumber seqNr, ChatKey chatKey)
{
    ServerCommand sc;
    sc.set_sequencenr(seqNr);
    sc.set_cmd(ServerCommandId::ServerChatCommand);
    sc.mutable_chatcmd()->set_cmd(ServerChatCommandId::LeftChat);
    sc.mutable_chatcmd()->set_chatkey(chatKey);
    return sc.SerializeAsString();
}
std::string ServerMessageBuilder::participantAddedToChat(SequenceNumber seqNr, ChatKey chatKey, ParticipantKey partKey)
{
    ServerCommand sc;
    sc.set_sequencenr(seqNr);
    sc.set_cmd(ServerCommandId::ServerChatCommand);
    sc.mutable_chatcmd()->set_cmd(ServerChatCommandId::ParticipantAddedToChat);
    sc.mutable_chatcmd()->set_chatkey(chatKey);
    sc.mutable_chatcmd()->set_participantkey(partKey);
    return sc.SerializeAsString();
}
std::string ServerMessageBuilder::participantLeftChat(SequenceNumber seqNr, ChatKey chatKey, ParticipantKey partKey)
{
    ServerCommand sc;
    sc.set_sequencenr(seqNr);
    sc.set_cmd(ServerCommandId::ServerChatCommand);
    sc.mutable_chatcmd()->set_cmd(ServerChatCommandId::ParticipantLeftChat);
    sc.mutable_chatcmd()->set_chatkey(chatKey);
    sc.mutable_chatcmd()->set_participantkey(partKey);
    return sc.SerializeAsString();
}
std::string ServerMessageBuilder::buildParticipantRegistered(SequenceNumber seqNr, ParticipantKey partKey)
{
    ServerCommand sc;
    sc.set_sequencenr(seqNr);
    sc.set_cmd(ServerCommandId::ServerParticipantCommand);
    sc.mutable_participantcmd()->set_cmd(ServerParticipantCommandId::Added);
    sc.mutable_participantcmd()->set_participantkey(partKey);
    return sc.SerializeAsString();
}
std::string ServerMessageBuilder::buildParticipantUnregistered(SequenceNumber seqNr, ParticipantKey partKey)
{
    ServerCommand sc;
    sc.set_sequencenr(seqNr);
    sc.set_cmd(ServerCommandId::ServerParticipantCommand);
    sc.mutable_participantcmd()->set_cmd(ServerParticipantCommandId::Removed);
    sc.mutable_participantcmd()->set_participantkey(partKey);
    return sc.SerializeAsString();
}
std::string ServerMessageBuilder::buildSendParticipantName(SequenceNumber seqNr, ParticipantKey partKey, std::string name)
{
    ServerCommand sc;
    sc.set_sequencenr(seqNr);
    sc.set_cmd(ServerCommandId::ServerParticipantCommand);
    sc.mutable_participantcmd()->set_cmd(ServerParticipantCommandId::SendName);
    sc.mutable_participantcmd()->set_participantkey(partKey);
    sc.mutable_participantcmd()->set_name(name);
    return sc.SerializeAsString();
}
std::string ServerMessageBuilder::buildSendParticipantEntryDate(SequenceNumber seqNr, ParticipantKey partKey, QDateTime timestamp)
{
    ServerCommand sc;
    sc.set_sequencenr(seqNr);
    sc.set_cmd(ServerCommandId::ServerParticipantCommand);
    sc.mutable_participantcmd()->set_cmd(ServerParticipantCommandId::SendEntryDate);
    sc.mutable_participantcmd()->set_participantkey(partKey);
    sc.mutable_participantcmd()->mutable_timestamp()->set_seconds(timestamp.toSecsSinceEpoch());
    return sc.SerializeAsString();
}
std::string ServerMessageBuilder::buildNewMessage(SequenceNumber seqNr, ChatKey chatKey, ParticipantKey partKey, QDateTime timestamp, std::string content)
{
    ServerCommand sc;
    sc.set_sequencenr(seqNr);
    sc.set_cmd(ServerCommandId::ServerMessageCommand);
    sc.mutable_msgcmd()->set_chatkey(chatKey);
    sc.mutable_msgcmd()->set_participantkey(partKey);
    sc.mutable_msgcmd()->mutable_timestamp()->set_seconds(timestamp.toSecsSinceEpoch());
    sc.mutable_msgcmd()->set_content(content);
    return sc.SerializeAsString();
}
