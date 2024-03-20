#ifndef F443BD6C_39D5_4268_B66C_A29F30009A50
#define F443BD6C_39D5_4268_B66C_A29F30009A50

#include <QDateTime>

#include "uniqueKeysTypedef.h"

class ReferenceClientMessageBuilder
{
public:
    ReferenceClientMessageBuilder(SequenceNumber seqNr, ChatKey chatKey, ParticipantKey participantKey, std::string participantName, std::string testMessageContent, QDateTime timestamp);
    [[nodiscard]] std::string setNickname();
    [[nodiscard]] std::string requestNewChat() const;
    [[nodiscard]] std::string requestChatParticipantKeys() const;
    [[nodiscard]] std::string requestLeavingChat() const;
    [[nodiscard]] std::string requestParticipantName() const;
    [[nodiscard]] std::string requestParticipantEntryDate() const;
    [[nodiscard]] std::string requestAddingParticipantToChat() const;
    [[nodiscard]] std::string sendNewMsg();
    [[nodiscard]] std::string genericResponseSuccess() const;
    [[nodiscard]] std::string genericResponseError() const;
    [[nodiscard]] std::string genericResponseMalformed() const;

private:
    const SequenceNumber seqNr;
    const ChatKey chatKey;
    const ParticipantKey participantKey;
    std::string participantName;
    std::string testMessageContent;
    QDateTime timestamp;
};

#endif /* F443BD6C_39D5_4268_B66C_A29F30009A50 */
