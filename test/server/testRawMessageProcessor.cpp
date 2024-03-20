#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "buildClientCmd.h"
#include "buildServerCmd.h"
#include "chat.h"
#include "clientThreadIface.h"
#include "memory"
#include "participant.h"
#include "rawClientMessageProcessor.h"
#include "serverIface.h"
#include "uniqueKeysTypedef.h"

#include "proto/serverMsgFormats.pb.h"

#include <QDateTime>

class MockServer : public ServerIface
{
public:
    MOCK_METHOD(void, removeParticipantFromChat, (ChatKey key, ParticipantKey participantKey), (const, override));
    MOCK_METHOD(void, addParticipantToChat, (std::shared_ptr<ServerChat> chat, ParticipantKey participantKey), (const, override));
    MOCK_METHOD(std::string, getParticipantName, (ParticipantKey key), (const, override));
    MOCK_METHOD(void, setParticipantName, (ParticipantKey key, std::string name), (const, override));
    MOCK_METHOD(QDateTime, getParticipantEntryDate, (ParticipantKey key), (const, override));
    MOCK_METHOD(void, setParticipantEntryDate, (ParticipantKey key, QDateTime entryDate), (const, override));
    MOCK_METHOD(void, registerParticipant, (std::shared_ptr<ServerParticipant> participant), (override));
    MOCK_METHOD(void, unregisterParticipant, (ParticipantKey key), (override));
};

class MockServerParticipant : public ServerParticipant
{
public:
    MockServerParticipant(std::shared_ptr<ServerChat> chatPtr)
    {
        ON_CALL(*this, getChat(testing::_)).WillByDefault(testing::Return(chatPtr));
    }
    MOCK_METHOD(void, joinChat, (std::shared_ptr<ServerChat> chat), (override));
    MOCK_METHOD(std::shared_ptr<ServerChat>, getChat, (ChatKey chatKey), (const, override));
    MOCK_METHOD(QString, getNickname, (), (const, override));
    MOCK_METHOD(QDateTime, getEntryDate, (), (const, override));
    MOCK_METHOD(void, setNickname, (QString name), (override));
    MOCK_METHOD(void, setEntryDate, (QDateTime entryDate), (override));
    MOCK_METHOD(void, leaveChat, (ChatKey chatKey), (override));
    MOCK_METHOD(void, newMessage, (ChatKey chatKey, std::string content, QDateTime timestamp), (override));
    MOCK_METHOD(ChatKey, newChat, (), (override));
    MOCK_METHOD(std::vector<ChatKey>, getChatKeys, (), (const, override));
    MOCK_METHOD(std::vector<ChatKey>, getParticipantKeys, (ChatKey chatKey), (const, override));
};

class MockClientThreadWorker : public ClientThreadIface
{
public:
    MockClientThreadWorker(ServerIface &server) : ClientThreadIface(server) {}
};
class ServerCommunicatorTest : public ::testing::Test
{
public:
    MOCK_METHOD(void, sendMsgToClient, (std::string));

protected:
    SequenceNumber seqNr;
    ChatKey chatKey;
    ParticipantKey participantKey;
    std::string participantName;
    std::string testMessageContent;
    std::shared_ptr<ServerChat> chatPtr = std::make_shared<ServerChat>();
    QDateTime timestamp = QDateTime::currentDateTime();
    std::vector<ParticipantKey> keys = {4352, 12738, 34};

    MockServer server;
    MockServerParticipant selfParticipant{chatPtr};

    MockClientThreadWorker clientThreadWorker{server};
    RawClientMessageProcessor serverCommunicator{server, selfParticipant, [this](std::string msg)
                                                 { sendMsgToClient(msg); }};
    ReferenceClientMessageBuilder clientMsgBuilder{seqNr, chatKey, participantKey, participantName, testMessageContent, timestamp};
    ReferenceServerMessageBuilder serverMsgBuilder{seqNr, chatKey, participantKey, participantName, testMessageContent, timestamp, keys};

    std::function<bool(const QDateTime)> timeStampMatch = [&](const QDateTime &ts)
    { return timestamp.toString().compare(ts.toString()) == 0; };
};

TEST_F(ServerCommunicatorTest, requestNewChat)
{
    EXPECT_CALL(selfParticipant, newChat()).Times(1);
    auto clientMsg = clientMsgBuilder.requestNewChat();
    serverCommunicator.processRawMessage(clientMsg);
}

TEST_F(ServerCommunicatorTest, setNickname)
{
    auto serverResponse = serverMsgBuilder.getGenericResponseCmd(ResponseCode::SUCCESS);

    EXPECT_CALL(selfParticipant, setNickname(QString::fromStdString(participantName))).Times(1);
    // EXPECT_CALL(*this, sendMsgToClient(serverResponse)).Times(1);

    auto clientMsg = clientMsgBuilder.setNickname();
    serverCommunicator.processRawMessage(clientMsg);
}

TEST_F(ServerCommunicatorTest, requestChatParticipantKeys)
{
    EXPECT_CALL(selfParticipant, getParticipantKeys(chatKey)).Times(1);
    auto clientMsg = clientMsgBuilder.requestChatParticipantKeys();
    serverCommunicator.processRawMessage(clientMsg);
}

TEST_F(ServerCommunicatorTest, requestLeavingChat)
{
    EXPECT_CALL(selfParticipant, leaveChat(chatKey)).Times(1);
    auto clientMsg = clientMsgBuilder.requestLeavingChat();
    serverCommunicator.processRawMessage(clientMsg);
}

TEST_F(ServerCommunicatorTest, requestParticipantName)
{
    EXPECT_CALL(server, getParticipantName(participantKey));
    auto clientMsg = clientMsgBuilder.requestParticipantName();
    serverCommunicator.processRawMessage(clientMsg);
}

TEST_F(ServerCommunicatorTest, requestParticipantEntryDate)
{
    EXPECT_CALL(server, getParticipantEntryDate(participantKey));
    auto clientMsg = clientMsgBuilder.requestParticipantEntryDate();
    serverCommunicator.processRawMessage(clientMsg);
}

TEST_F(ServerCommunicatorTest, addParticipantToChat)
{
    EXPECT_CALL(server, addParticipantToChat(chatPtr, participantKey));
    EXPECT_CALL(selfParticipant, getChat(chatKey)).Times(1);
    auto clientMsg = clientMsgBuilder.requestAddingParticipantToChat();
    serverCommunicator.processRawMessage(clientMsg);
}

TEST_F(ServerCommunicatorTest, sendNewMsg)
{
    EXPECT_CALL(selfParticipant, newMessage(chatKey, testMessageContent, testing::Truly(timeStampMatch)));
    auto clientMsg = clientMsgBuilder.sendNewMsg();
    serverCommunicator.processRawMessage(clientMsg);
}
TEST_F(ServerCommunicatorTest, genericResponseSuccess)
{
    // TODO EXPECT_CALL(server, getParticipantEntryDate(participantKey));
    auto clientMsg = clientMsgBuilder.genericResponseSuccess();
    serverCommunicator.processRawMessage(clientMsg);
}
TEST_F(ServerCommunicatorTest, genericResponseError)
{
    // TODO EXPECT_CALL(server, getParticipantEntryDate(participantKey));
    auto clientMsg = clientMsgBuilder.genericResponseError();
    serverCommunicator.processRawMessage(clientMsg);
}

TEST_F(ServerCommunicatorTest, genericResponseMalformed)
{
    // TODO EXPECT_CALL(server, getParticipantEntryDate(participantKey));
    auto clientMsg = clientMsgBuilder.genericResponseMalformed();
    serverCommunicator.processRawMessage(clientMsg);
}