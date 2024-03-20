#ifndef E563A460_DD74_425F_8ADE_F1B635A275BB
#define E563A460_DD74_425F_8ADE_F1B635A275BB
#include <QDateTime>
#include <string>

#include "proto/serverClientCommunication.pb.h"
#include "uniqueKeysTypedef.h"

class ReferenceServerMessageBuilder
{
public:
    ReferenceServerMessageBuilder(SequenceNumber seqNr, ChatKey chatKey, ParticipantKey participantKey, std::string participantName, std::string testMessageContent, QDateTime timestamp, std::vector<unsigned> participantKeys);
    std::string getAddedToChatCmd() const;
    std::string getNewIncomingMessageCmd();
    std::string getRemovedFromChatCmd() const;
    std::string getGenericResponseCmd(ResponseCode response) const;
    std::string getParticipantAddedCmd() const;
    std::string getParticipantRemovedCmd() const;
    std::string getParticipantNameCmd();
    std::string getParticipantEntryDateCmd();
    std::string getParticipantAddedToChatCmd() const;
    std::string getParticipantLeftChatCmd() const;
    std::string getParticipantKeys();
    std::string getMalformedMsg();
    std::string getMalformedChatMsg();
    std::string getMalformedParticipantMsg();
    std::string getMalformedMessageMsg();

private:
    const SequenceNumber seqNr;
    const ChatKey chatKey;
    const ParticipantKey participantKey;
    const std::string participantName;
    const std::string testMessageContent;
    const QDateTime timestamp;
    const std::vector<unsigned> participantKeys;
};

#endif /* E563A460_DD74_425F_8ADE_F1B635A275BB */
