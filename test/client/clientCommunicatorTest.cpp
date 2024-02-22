#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QString>

#include "../client/include/communicators.h"
#include "buildServerCmd.h"
#include "chat.h"
#include "clientIface.h"
#include "clientMsgFormats.pb.h"
#include "serverMsgFormats.pb.h"

class TestClient : public ClientIface
{
public:
    virtual QString getNickname() const override
    {
        return QString("John");
    };
    virtual void addNewChat(const ChatKey &key) override
    {
        if (std::find(chatKeys.begin(), chatKeys.end(), key) != chatKeys.end())
        {
            throw ChatAlreadyExists();
        }
        chatKeys.push_back(key);
    }
    virtual void leaveChat(const ChatKey &key) override
    {
        auto countDeleted = std::remove(chatKeys.begin(), chatKeys.end(), key);
        if (countDeleted == chatKeys.end())
        {
            throw ChatNotFound();
        }
    }
    virtual void addNewIncomingMessage(const ChatKey &key, [[maybe_unused]] const QString content, const ParticipantKey &participantKey, [[maybe_unused]] const QDateTime timestamp) override
    {
        if (std::find(chatKeys.begin(), chatKeys.end(), key) == chatKeys.end())
        {
            throw ChatNotFound();
        }
    }
    virtual void assignParticipantName(const ParticipantKey &key, [[maybe_unused]] const QString name) override
    {
    }
    virtual void assignParticipantEntryDate(const ParticipantKey &key, [[maybe_unused]] const QDateTime entryDate) override
    {
    virtual void addParticipant(const ParticipantKey &key)
    {
        if (std::find(participantKeys.begin(), participantKeys.end(), key) == participantKeys.end())
        {
            participantKeys.push_back(key);
            return;
        }
        throw ParticipantAlreadyExists();
    }
    virtual void removeParticipant(const ParticipantKey &key)
    {
        auto countDeleted = std::remove(participantKeys.begin(), participantKeys.end(), key);
        if (countDeleted == chatKeys.end())
        {
            throw ParticipantNotFound();
        }
    }
    virtual void addParticipantToChat(const ChatKey &chatKey, const ParticipantKey &participantKey)
    {
        if (std::find(chatKeys.begin(), chatKeys.end(), chatKey) == chatKeys.end())
        {
            throw ChatNotFound();
        }
        if (std::find(participantKeys.begin(), participantKeys.end(), participantKey) == participantKeys.end())
        {
            throw ParticipantNotFound();
        }
        if (std::find(chatParticipants[chatKey].begin(), chatParticipants[chatKey].end(), participantKey) != chatParticipants[chatKey].end())
        {
            throw ParticipantAlreadyExists();
        }
        chatParticipants[chatKey].push_back(participantKey);
    };
    virtual void removeParticipantFromChat(const ChatKey &chatKey, const ParticipantKey &participantKey)
    {
        if (std::find(chatKeys.begin(), chatKeys.end(), chatKey) == chatKeys.end())
        {
            throw ChatNotFound();
        }
        if (std::find(participantKeys.begin(), participantKeys.end(), participantKey) == participantKeys.end())
        {
            throw ParticipantNotFound();
        }
        auto countDeleted = std::remove(chatParticipants[chatKey].begin(), chatParticipants[chatKey].end(), participantKey);
        if (countDeleted == chatParticipants[chatKey].end())
        {
            throw ParticipantNotFound();
        }
    };

private:
    std::vector<ChatKey> chatKeys = {};
    std::vector<ParticipantKey> participantKeys = {};
    std::map<ChatKey, std::vector<ParticipantKey>> chatParticipants = {};
};

class MockClient : public ClientIface
{
public:
    MockClient()
    {
        DelegateToFake();
    }
    MOCK_METHOD(QString, getNickname, (), (const, override));
    MOCK_METHOD(void, addNewChat, (const ChatKey &key), (override));
    MOCK_METHOD(void, leaveChat, (const ChatKey &key), (override));
    MOCK_METHOD(void, addParticipantToChat, (const ChatKey &chatKey, const ParticipantKey &participantKey), (override));
    MOCK_METHOD(void, removeParticipantFromChat, (const ChatKey &chatKey, const ParticipantKey &participantKey), (override));
    MOCK_METHOD(void, addNewIncomingMessage, (const ChatKey &key, const QString content, const ParticipantKey &participantKey, const QDateTime timestamp), (override));
    MOCK_METHOD(void, assignParticipantName, (const ParticipantKey &key, const QString name), (override));
    MOCK_METHOD(void, assignParticipantEntryDate, (const ParticipantKey &key, QDateTime entryDate), (override));
    MOCK_METHOD(void, addParticipant, (const ParticipantKey &key), (override));
    MOCK_METHOD(void, removeParticipant, (const ParticipantKey &key), (override));

    void DelegateToFake()
    {
        ON_CALL(*this, getNickname).WillByDefault([this]()
                                                  { return fake_.getNickname(); });
        ON_CALL(*this, addNewChat).WillByDefault([this](const ChatKey &key)
                                                 { return fake_.addNewChat(key); });
        ON_CALL(*this, leaveChat).WillByDefault([this](const ChatKey &key)
                                                { return fake_.leaveChat(key); });
        ON_CALL(*this, addNewIncomingMessage).WillByDefault([this](const ChatKey &key, const QString content, const ParticipantKey &participantKey, const QDateTime timestamp)
                                                            { return fake_.addNewIncomingMessage(key, content, participantKey, timestamp); });
        ON_CALL(*this, assignParticipantName).WillByDefault([this](const ParticipantKey &key, const QString name)
                                                            { return fake_.assignParticipantName(key, name); });
        ON_CALL(*this, assignParticipantEntryDate).WillByDefault([this](const ParticipantKey &key, QDateTime entryDate)
                                                                 { return fake_.assignParticipantEntryDate(key, entryDate); });
        ON_CALL(*this, addParticipant).WillByDefault([this](const ParticipantKey &key)
                                                                 { return fake_.addParticipant(key); });
        ON_CALL(*this, removeParticipant).WillByDefault([this](const ParticipantKey &key)
                                                                 { return fake_.removeParticipant(key); });
        ON_CALL(*this, addParticipantToChat).WillByDefault([this](const ChatKey &chatKey, const ParticipantKey &participantKey)
                                                           { return fake_.addParticipantToChat(chatKey, participantKey); });
        ON_CALL(*this, removeParticipantFromChat).WillByDefault([this](const ChatKey &chatKey, const ParticipantKey &participantKey)
                                                                { return fake_.removeParticipantFromChat(chatKey, participantKey); });
    }

private:
    TestClient fake_;
};

class ClientCommunicatorTest : public ::testing::Test
{
protected:
    MockClient client;
    ClientCommunicator communicator{client};
    const ChatKey chatKey = 123;
    const ParticipantKey participantKey = 234;
    std::string participantName = "Romeo";
    std::string testMessageContent = "Dear Juliet. Please wait for me in the garden.";
    QDateTime timestamp = QDateTime::currentDateTime();
    TestServerCommunicator serverMsgGenerator{chatKey, participantKey, participantName, testMessageContent, timestamp};

    /**
     * @brief This is just a helper method to reduce redundant code. Gets the response code for a server message. Expeccts that the client communicator sends a generic response.
     *
     * @param serverMsg The server message to let the communicator answer to.
     * @return The response code that the communicator generated as response to the server's message.
     */
    ResponseCode getResponseCode(std::string serverMsg)
    {
        auto generatedResponseStr = communicator.answerMessage(serverMsg);
        ClientCommand genneratedResponse;
        genneratedResponse.ParseFromString(generatedResponseStr);
        EXPECT_EQ(genneratedResponse.cmd(), ClientCommandId::ClientGenericResponse);
        return genneratedResponse.response();
    }
};

TEST_F(ClientCommunicatorTest, WelcomeMessage)
{
    EXPECT_CALL(client, getNickname).Times(1);
    auto cmdString = communicator.welcomeMessage();

    ClientCommand expectedSetNicknameCmd;
    expectedSetNicknameCmd.ParseFromString(cmdString.toStdString());

    EXPECT_EQ(expectedSetNicknameCmd.cmd(), ClientCommandId::SetNickname);
    EXPECT_EQ(expectedSetNicknameCmd.name(), "John");
}

TEST_F(ClientCommunicatorTest, AddNewChat)
{
    EXPECT_CALL(client, addNewChat(chatKey)).Times(1);

    auto reponse = getResponseCode(serverMsgGenerator.getAddedToChatCmd());
    EXPECT_EQ(reponse, ResponseCode::SUCCESS);
}

TEST_F(ClientCommunicatorTest, AddNewChatTwice)
{
    EXPECT_CALL(client, addNewChat(chatKey)).Times(2);

    auto reponse = getResponseCode(serverMsgGenerator.getAddedToChatCmd());
    EXPECT_EQ(reponse, ResponseCode::SUCCESS);

    reponse = getResponseCode(serverMsgGenerator.getAddedToChatCmd());
    EXPECT_EQ(reponse, ResponseCode::ERROR);
}

TEST_F(ClientCommunicatorTest, ParticpantAddedToChat)
{
    EXPECT_CALL(client, addNewChat(chatKey)).Times(1);
    EXPECT_CALL(client, addParticipant(participantKey)).Times(1);
    EXPECT_CALL(client, addParticipantToChat(chatKey, participantKey)).Times(3);

    auto response = getResponseCode(serverMsgGenerator.getParticipantAddedToChatCmd());
    EXPECT_EQ(response, ResponseCode::ERROR);

    getResponseCode(serverMsgGenerator.getAddedToChatCmd());

    response = getResponseCode(serverMsgGenerator.getParticipantAddedToChatCmd());
    EXPECT_EQ(response, ResponseCode::ERROR);

    getResponseCode(serverMsgGenerator.getParticipantAddedCmd());

    response = getResponseCode(serverMsgGenerator.getParticipantAddedToChatCmd());
    EXPECT_EQ(response, ResponseCode::SUCCESS);
}

TEST_F(ClientCommunicatorTest, ParticipantLeftChat)
{
    EXPECT_CALL(client, addNewChat(chatKey)).Times(1);
    EXPECT_CALL(client, addParticipant(participantKey)).Times(1);
    EXPECT_CALL(client, addParticipantToChat(chatKey, participantKey)).Times(1);
    EXPECT_CALL(client, removeParticipantFromChat(chatKey, participantKey)).Times(3);

    auto response = getResponseCode(serverMsgGenerator.getParticipantLeftChatCmd());
    EXPECT_EQ(response, ResponseCode::ERROR);

    getResponseCode(serverMsgGenerator.getAddedToChatCmd());
    getResponseCode(serverMsgGenerator.getParticipantAddedCmd());

    response = getResponseCode(serverMsgGenerator.getParticipantLeftChatCmd());
    EXPECT_EQ(response, ResponseCode::ERROR);

    getResponseCode(serverMsgGenerator.getParticipantAddedToChatCmd());

    response = getResponseCode(serverMsgGenerator.getParticipantLeftChatCmd());
    EXPECT_EQ(response, ResponseCode::SUCCESS);
}

TEST_F(ClientCommunicatorTest, LeaveChat)
{
    EXPECT_CALL(client, addNewChat(chatKey)).Times(1);
    EXPECT_CALL(client, leaveChat(chatKey)).Times(1);

    auto reponse = getResponseCode(serverMsgGenerator.getAddedToChatCmd());
    EXPECT_EQ(reponse, ResponseCode::SUCCESS);

    reponse = getResponseCode(serverMsgGenerator.getRemovedFromChatCmd());
    EXPECT_EQ(reponse, ResponseCode::SUCCESS);
}

TEST_F(ClientCommunicatorTest, LeaveUnkownChat)
{
    EXPECT_CALL(client, leaveChat(chatKey)).Times(1);

    auto reponse = getResponseCode(serverMsgGenerator.getRemovedFromChatCmd());
    EXPECT_EQ(reponse, ResponseCode::ERROR);
}

TEST_F(ClientCommunicatorTest, AddNewIncomingMessage)
{
    EXPECT_CALL(client, addNewChat(chatKey)).Times(1);
    EXPECT_CALL(client, addNewIncomingMessage(chatKey, QString(testMessageContent.c_str()), participantKey, timestamp)).Times(1);

    getResponseCode(serverMsgGenerator.getAddedToChatCmd());

    auto reponse = getResponseCode(serverMsgGenerator.getNewIncomingMessageCmd());
    EXPECT_EQ(reponse, ResponseCode::SUCCESS);
}

TEST_F(ClientCommunicatorTest, AddNewIncomingMessageUnknownChat)
{
    EXPECT_CALL(client, addNewIncomingMessage(chatKey, QString(testMessageContent.c_str()), participantKey, timestamp)).Times(1);

    auto reponse = getResponseCode(serverMsgGenerator.getNewIncomingMessageCmd());
    EXPECT_EQ(reponse, ResponseCode::ERROR);
}

// TODO: This test is not possible to implement because the client does currently not have its own managed chat participant list
// TEST_F(ClientCommunicatorTest, AddNewIncomingMessageUnknownParticipant)
// {
//     EXPECT_CALL(client, addNewIncomingMessage(chatKey, testMessageContent, participantKey, timestamp)).Times(1);

//     auto reponse = getResponseCode(serverMsgGenerator.getRemovedFromChatCmd());
//     EXPECT_EQ(reponse, ResponseCode::ERROR);
// }

TEST_F(ClientCommunicatorTest, AddParticipant)
{
    EXPECT_CALL(client, addParticipant(participantKey)).Times(2);

    auto response = getResponseCode(serverMsgGenerator.getParticipantAddedCmd());
    EXPECT_EQ(response, ResponseCode::SUCCESS);

    response = getResponseCode(serverMsgGenerator.getParticipantAddedCmd());
    EXPECT_EQ(response, ResponseCode::ERROR);
}

TEST_F(ClientCommunicatorTest, RemoveParticipant)
{
    EXPECT_CALL(client, removeParticipant(participantKey)).Times(2);
    EXPECT_CALL(client, addParticipant(participantKey)).Times(1);

    auto response = getResponseCode(serverMsgGenerator.getParticipantRemovedCmd());
    EXPECT_EQ(response, ResponseCode::ERROR);

    getResponseCode(serverMsgGenerator.getParticipantAddedCmd());

    response = getResponseCode(serverMsgGenerator.getParticipantRemovedCmd());
    EXPECT_EQ(response, ResponseCode::SUCCESS);
}

TEST_F(ClientCommunicatorTest, AssignParticipantName)
{
    ParticipantKey key = 123;
    QString name = "John";
    EXPECT_CALL(client, assignParticipantName(key, name)).Times(1);
}

TEST_F(ClientCommunicatorTest, AssignParticipantEntryDate)
{
    ParticipantKey key = 123;
    QDateTime entryDate = QDateTime::currentDateTime();
    EXPECT_CALL(client, assignParticipantEntryDate(key, entryDate)).Times(1);
}
