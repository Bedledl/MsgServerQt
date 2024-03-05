#ifndef CLIENT_INCLUDE_CLIENTMESSAGEBUILDER
#define CLIENT_INCLUDE_CLIENTMESSAGEBUILDER
#include "uniqueKeysTypedef.h"
#include <string>

namespace ClientMessageBuilder
{
    std::string getSendNewMsg(SequenceNumber, ChatKey, std::string msg);
    std::string getRequestNewChat(SequenceNumber);
    std::string getRequestParticipantKeys(SequenceNumber, ChatKey);
    std::string getSetNewNickname(SequenceNumber, std::string newName);
    std::string getRequestAddingParticipantToChat(SequenceNumber, ChatKey, ParticipantKey);
    std::string getRequestLeaveChat(SequenceNumber, ChatKey);
    std::string getRequestParticipantName(SequenceNumber, ParticipantKey);
    std::string getRequestParticipantEntryDate(SequenceNumber, ParticipantKey);
    std::string getGenericResponseSuccess(SequenceNumber);
    std::string getGenericResponseError(SequenceNumber);
    std::string getGenericResponseMalformed(SequenceNumber);
};

#endif /* CLIENT_INCLUDE_CLIENTMESSAGEBUILDER */
