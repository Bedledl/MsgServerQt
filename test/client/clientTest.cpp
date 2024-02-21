#include <gtest/gtest.h>

#include <client.h>

class ClientTest : public testing::Test
{
protected:
    std::shared_ptr<Participant> participant1 = std::make_shared<Participant>(123);
    std::shared_ptr<Participant> participant2 = std::make_shared<Participant>(234);
};

TEST_F(ClientTest, ChatManagementTest) {
    // test adding of new chat
    // test removing of chat

}

TEST_F(ClientTest, ParticipantMangementTest) {
    // test adding of new participant
    // test removing of participant
    // test setting of participant's nickname
    // test setting of participant's entry date
    // test adding of participant to chat
    // test removing of participant from chat

}

TEST_F(ClientTest, MessagingTest) {
    // test sending of own message in chat
    // test receving of remote message in chat
}

TEST_F(ClientTest, ConnectionTest) {
    // test connection to server
    // test disconnection from server
    // test reconnection to server
}

TEST_F(ClientTest, selectedChatTest) {}

TEST_F(ClientTest, chatPreviewListTest) {}
