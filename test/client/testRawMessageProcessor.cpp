#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QString>

#include "buildServerCmd.h"
#include "chat.h"
#include "clientIface.h"
#include "participant.h"
#include "rawMessageProcessor.h"

class MockClient : public ClientOuterIface
{
public:
    MOCK_METHOD(void, addNewChat, (const SequenceNumber seqNr, const ChatKey &key), (override));
    MOCK_METHOD(void, leaveChat, (const SequenceNumber seqNr, const ChatKey &key), (override));
    MOCK_METHOD(void, addParticipantToChat, (const SequenceNumber seqNr, const ChatKey &chatKey, const ParticipantKey &participantKey), (override));
    MOCK_METHOD(void, removeParticipantFromChat, (const SequenceNumber seqNr, const ChatKey &chatKey, const ParticipantKey &participantKey), (override));
    MOCK_METHOD(void, addNewIncomingMessage, (const SequenceNumber seqNr, const ChatKey &key, const QString content, const ParticipantKey &participantKey, const QDateTime timestamp), (override));
    MOCK_METHOD(void, assignParticipantName, (const SequenceNumber seqNr, const ParticipantKey &key, const QString name), (override));
    MOCK_METHOD(void, assignParticipantEntryDate, (const SequenceNumber seqNr, const ParticipantKey &key, QDateTime entryDate), (override));
    MOCK_METHOD(void, addParticipant, (const SequenceNumber seqNr, const ParticipantKey &key), (override));
    MOCK_METHOD(void, removeParticipant, (const SequenceNumber seqNr, const ParticipantKey &key), (override));
    MOCK_METHOD(void, receivedMalformedMessage, (const SequenceNumber seqNr), (const, override));
};

class ClientRawMessageProcessorTest : public ::testing::Test
{
protected:
    MockClient client;
    RawMessageProcessor rawMessageProcessor{client, [](const std::string &) {}};
    const SequenceNumber sequenceNumber = 666;
    const ChatKey chatKey = 123;
    const ParticipantKey participantKey = 234;
    std::string participantName = "Romeo";
    std::string testMessageContent = "Dear Juliet. Please wait for me in the garden.";
    QDateTime timestamp = QDateTime::currentDateTime();
    std::vector<unsigned> participantKeys = {participantKey, participantKey, participantKey, participantKey};
    ReferenceServerMessageBuilder serverMsgGenerator{sequenceNumber, chatKey, participantKey, participantName, testMessageContent, timestamp, participantKeys};

    std::function<bool(const QDateTime)> timeStampMatch = [&](const QDateTime &ts)
    { return timestamp.toString().compare(ts.toString()) == 0; };
};

TEST_F(ClientRawMessageProcessorTest, AddNewChat)
{
    EXPECT_CALL(client, addNewChat(sequenceNumber, chatKey)).Times(1);
    rawMessageProcessor.processRawMessage(serverMsgGenerator.getAddedToChatCmd());
}
TEST_F(ClientRawMessageProcessorTest, ParticpantAddedToChat)
{
    EXPECT_CALL(client, addParticipantToChat(sequenceNumber, chatKey, participantKey)).Times(1);
    rawMessageProcessor.processRawMessage(serverMsgGenerator.getParticipantAddedToChatCmd());
}
TEST_F(ClientRawMessageProcessorTest, ParticipantLeftChat)
{
    EXPECT_CALL(client, removeParticipantFromChat(sequenceNumber, chatKey, participantKey)).Times(1);
    rawMessageProcessor.processRawMessage(serverMsgGenerator.getParticipantLeftChatCmd());
}
TEST_F(ClientRawMessageProcessorTest, LeaveChat)
{
    EXPECT_CALL(client, leaveChat(sequenceNumber, chatKey)).Times(1);
    rawMessageProcessor.processRawMessage(serverMsgGenerator.getRemovedFromChatCmd());
}
TEST_F(ClientRawMessageProcessorTest, AddNewIncomingMessage)
{
    EXPECT_CALL(client, addNewIncomingMessage(sequenceNumber, chatKey, QString::fromStdString(testMessageContent), participantKey, testing::Truly(timeStampMatch))).Times(1);
    rawMessageProcessor.processRawMessage(serverMsgGenerator.getNewIncomingMessageCmd());
}
TEST_F(ClientRawMessageProcessorTest, AddParticipant)
{
    EXPECT_CALL(client, addParticipant(sequenceNumber, participantKey)).Times(1);
    rawMessageProcessor.processRawMessage(serverMsgGenerator.getParticipantAddedCmd());
}
TEST_F(ClientRawMessageProcessorTest, RemoveParticipant)
{
    EXPECT_CALL(client, removeParticipant(sequenceNumber, participantKey)).Times(1);
    rawMessageProcessor.processRawMessage(serverMsgGenerator.getParticipantRemovedCmd());
}
TEST_F(ClientRawMessageProcessorTest, AssignParticipantName)
{
    EXPECT_CALL(client, assignParticipantName(sequenceNumber, participantKey, QString::fromStdString(participantName))).Times(1);
    rawMessageProcessor.processRawMessage(serverMsgGenerator.getParticipantNameCmd());
}
TEST_F(ClientRawMessageProcessorTest, AssignParticipantEntryDate)
{
    EXPECT_CALL(client, assignParticipantEntryDate(sequenceNumber, participantKey, testing::Truly(timeStampMatch))).Times(1);
    rawMessageProcessor.processRawMessage(serverMsgGenerator.getParticipantEntryDateCmd());
}
TEST_F(ClientRawMessageProcessorTest, RequestChatParticipantKeys)
{
    EXPECT_CALL(client, addParticipantToChat(sequenceNumber, chatKey, participantKey)).Times(participantKeys.size());
    rawMessageProcessor.processRawMessage(serverMsgGenerator.getParticipantKeys());
}
TEST_F(ClientRawMessageProcessorTest, MalformedMessage)
{
    EXPECT_CALL(client, receivedMalformedMessage(sequenceNumber)).Times(1);
    rawMessageProcessor.processRawMessage(serverMsgGenerator.getMalformedMsg());
}
TEST_F(ClientRawMessageProcessorTest, MalformedChatMessage)
{
    EXPECT_CALL(client, receivedMalformedMessage(sequenceNumber)).Times(1);
    rawMessageProcessor.processRawMessage(serverMsgGenerator.getMalformedChatMsg());
}
TEST_F(ClientRawMessageProcessorTest, MalformedParticipantMessage)
{
    EXPECT_CALL(client, receivedMalformedMessage(sequenceNumber)).Times(1);
    rawMessageProcessor.processRawMessage(serverMsgGenerator.getMalformedParticipantMsg());
}
TEST_F(ClientRawMessageProcessorTest, MalformedMessageMessage)
{
    EXPECT_CALL(client, receivedMalformedMessage(sequenceNumber)).Times(1);
    rawMessageProcessor.processRawMessage(serverMsgGenerator.getMalformedMessageMsg());
}
