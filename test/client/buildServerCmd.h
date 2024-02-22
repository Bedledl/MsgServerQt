#ifndef E563A460_DD74_425F_8ADE_F1B635A275BB
#define E563A460_DD74_425F_8ADE_F1B635A275BB

#include <string>

#include "serverMsgFormats.pb.h"

class TestServerCommunicator
{
    public:
    TestServerCommunicator(int chatKey, int participantKey, std::string participantName, std::string testMessageContent, QDateTime timestamp);
    std::string getAddedToChatCmd();
    std::string getNewIncomingMessageCmd();
    std::string getRemovedFromChatCmd();
    std::string getGenericResponseCmd(ResponseCode response);
    std::string getParticipantAddedCmd();
    std::string getParticipantRemovedCmd();
    std::string getParticipantNameCmd();
    std::string getParticipantEntryDateCmd();
    std::string getParticipantAddedToChatCmd();
    std::string getParticipantLeftChatCmd();

private:
    int chatKey;
    int participantKey;
    std::string participantName;
    std::string testMessageContent;
};


#endif /* E563A460_DD74_425F_8ADE_F1B635A275BB */
