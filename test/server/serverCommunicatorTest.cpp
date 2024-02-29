#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "buildClientCmd.h"
#include "chat.h"
#include "clientThreadIface.h"
#include "communicators.h"
#include "participant.h"
#include "serverIface.h"

#include "serverMsgFormats.pb.h"

#include <QDateTime>

class MockServer : public Server
{
public:
    MOCK_METHOD(void, removeParticipantFromChat, (ChatKey key, ParticipantKey participantKey), (const, override));
    MOCK_METHOD(void, addParticipantToChat, (std::shared_ptr<ServerChat> chat, ParticipantKey participantKey), (const, override));
    MOCK_METHOD(std::string, getParticipantName, (ParticipantKey key), (const, override));
    MOCK_METHOD(void, setParticipantName, (ParticipantKey key, std::string name), (const, override));
    MOCK_METHOD(QDateTime, getParticipantEntryDate, (ParticipantKey key), (const, override));
    MOCK_METHOD(void, setParticipantEntryDate, (ParticipantKey key, QDateTime entryDate), (const, override));
    MOCK_METHOD(void, addNewIncomingMessage, (ChatKey key, std::string content, ParticipantKey participantKey, QDateTime timestamp), (const, override));
};

class MockClientThreadWorker : public ClientThreadIface
{
public:
    MockClientThreadWorker(Server &server) : ClientThreadIface(server) {}
    MOCK_METHOD(void, setNickname, (std::string), (override));
    MOCK_METHOD(void, requestLeavingChat, (ChatKey & key), (override));
    MOCK_METHOD(std::vector<ParticipantKey>, requestChatParticipantKeys, (ChatKey & key), (override));
    MOCK_METHOD(ChatKey, requestNewChat, (), (override));
};
class ServerCommunicatorTest : public ::testing::Test
{
protected:
    ChatKey chatKey;
    ParticipantKey participantKey;
    std::string participantName;
    std::string testMessageContent;
    QDateTime timestamp = QDateTime::currentDateTime();

    MockServer server;
    MockClientThreadWorker clientThreadWorker{server};
    ServerCommunicator serverCommunicator{clientThreadWorker};
    TestClientCommunicator clientCommunicator{chatKey, participantKey, participantName, testMessageContent, timestamp};

    /**
     * @brief This is just a helper method to reduce redundant code. Gets the response code for a client message. Expects that the server communicator sends a generic response.
     *
     * @param clientMsg The client message to let the communicator answer to.
     * @return The response code that the communicator generated as response to the server's message.
     */
    ResponseCode getResponseCode(std::string clientMsg)
    {
        auto generatedResponseStr = serverCommunicator.answerMessage(clientMsg);
        ServerCommand genneratedResponse;
        genneratedResponse.ParseFromString(generatedResponseStr);
        EXPECT_EQ(genneratedResponse.cmd(), ServerCommandId::ServerGenericResponse);
        return genneratedResponse.response();
    }
};

TEST_F(ServerCommunicatorTest, setNickname)
{
    EXPECT_CALL(clientThreadWorker, setNickname(participantName));

    auto response = getResponseCode(clientCommunicator.setNickname());
    qDebug() << response;
    EXPECT_EQ(response, ResponseCode::SUCCESS);
}

TEST_F(ServerCommunicatorTest, requestNewChat)
{
    EXPECT_CALL(clientThreadWorker, requestNewChat()).Times(1);

    auto clientMsg = clientCommunicator.requestNewChat();
    auto response = serverCommunicator.answerMessage(clientMsg);
    ServerCommand generatedResponse;
    generatedResponse.ParseFromString(response);
    EXPECT_EQ(generatedResponse.cmd(), ServerCommandId::ServerChatCommand);
    EXPECT_EQ(generatedResponse.chatcmd().cmd(), ServerChatCommandId::AddedToChat);
}

TEST_F(ServerCommunicatorTest, requestChatParticipantKeys)
{
    EXPECT_CALL(clientThreadWorker, requestChatParticipantKeys(chatKey));
    auto response = getResponseCode(clientCommunicator.requestChatParticipantKeys());
    EXPECT_EQ(response, ResponseCode::SUCCESS);
    // TODO: check if the communicator will send the participant keys in further unsolicited messages
}

TEST_F(ServerCommunicatorTest, requestLeavingChat)
{

    EXPECT_CALL(clientThreadWorker, requestLeavingChat(chatKey)).Times(2);

    auto response = getResponseCode(clientCommunicator.requestLeavingChat());
    EXPECT_EQ(response, ResponseCode::SUCCESS);
}

TEST_F(ServerCommunicatorTest, requestParticipantName)
{
    EXPECT_CALL(server, getParticipantName(participantKey));

    auto clientMsg = clientCommunicator.requestParticipantName();
    auto responseStr = serverCommunicator.answerMessage(clientMsg);

    ServerCommand response;
    response.ParseFromString(responseStr);

    EXPECT_EQ(response.cmd(), ServerCommandId::ServerParticipantCommand);
    EXPECT_EQ(response.participantcmd().cmd(), ServerParticipantCommandId::SendName);
    EXPECT_EQ(response.participantcmd().participantkey(), participantKey);
    // EXPECT_EQ(response.participantcmd().name(), participantName); // this can probabyl not be checked in this way
}

TEST_F(ServerCommunicatorTest, requestParticipantEntryDate)
{
    EXPECT_CALL(server, getParticipantEntryDate(participantKey));

    auto clientMsg = clientCommunicator.requestParticipantEntryDate();
    auto responseStr = serverCommunicator.answerMessage(clientMsg);

    ServerCommand response;
    response.ParseFromString(responseStr);

    EXPECT_EQ(response.cmd(), ServerCommandId::ServerParticipantCommand);
    EXPECT_EQ(response.participantcmd().cmd(), ServerParticipantCommandId::SendEntryDate);
    EXPECT_EQ(response.participantcmd().participantkey(), participantKey);
}

TEST_F(ServerCommunicatorTest, addParticipantToChat)
{
    // TODO make comparator for chat
    // EXPECT_CALL(server, addParticipantToChat(chatKey, participantKey));

    auto response = getResponseCode(clientCommunicator.requestAddingParticipantToChat());
    EXPECT_EQ(response, ResponseCode::SUCCESS);
}