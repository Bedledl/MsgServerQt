#ifndef SERVER_INCLUDE_SERVERMESSAGEBUILDER
#define SERVER_INCLUDE_SERVERMESSAGEBUILDER

#include "proto/serverClientCommunication.pb.h"
#include "uniqueKeysTypedef.h"

#include "QDateTime"

class ServerMessageBuilder
{

public:
    std::string buildGenericResponse(SequenceNumber seqNr, ResponseCode response);
    std::string buildAddedToChat(SequenceNumber seqNr, ChatKey chatKey);
    std::string buildLeftChat(SequenceNumber seqNr, ChatKey chatKey);
    std::string participantAddedToChat(SequenceNumber seqNr, ChatKey chatKey, ParticipantKey partKey);
    std::string participantLeftChat(SequenceNumber seqNr, ChatKey chatKey, ParticipantKey partKey);

    std::string buildParticipantRegistered(SequenceNumber seqNr, ParticipantKey partKey);
    std::string buildParticipantUnregistered(SequenceNumber seqNr, ParticipantKey partKey);
    std::string buildSendParticipantName(SequenceNumber seqNr, ParticipantKey partKey, std::string name);
    std::string buildSendParticipantEntryDate(SequenceNumber seqNr, ParticipantKey partKey, QDateTime timestamp);

    std::string buildNewMessage(SequenceNumber seqNr, ChatKey chatKey, ParticipantKey partKey, QDateTime timestamp, std::string content);
};

#endif /* SERVER_INCLUDE_SERVERMESSAGEBUILDER */
