#ifndef F443BD6C_39D5_4268_B66C_A29F30009A50
#define F443BD6C_39D5_4268_B66C_A29F30009A50

#include <QDateTime>

#include "chat.h"
#include "participant.h"

class TestClientCommunicator
{
public:
    TestClientCommunicator(ChatKey chatKey, ParticipantKey participantKey, std::string participantName, std::string testMessageContent, QDateTime timestamp);
    std::string setNickname();
    std::string requestNewChat();
    std::string requestChatParticipantKeys();
    std::string requestLeavingChat();
    std::string requestParticipantName();
    std::string requestParticipantEntryDate();
    std::string requestAddingParticipantToChat();
private:
    int chatKey;
    int participantKey;
    std::string participantName;
    std::string testMessageContent;
    QDateTime timestamp;
};

#endif /* F443BD6C_39D5_4268_B66C_A29F30009A50 */
