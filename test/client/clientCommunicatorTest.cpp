#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QString>

#include <client.h>
#include <

class ChatKey;
class ParticipantKey;

class MockClient : public Client
{
public:
    MOCK_METHOD(void, addNewChat, (const ChatKey &key), ());
    MOCK_METHOD(void, leaveChat, (const ChatKey &key), ());
    MOCK_METHOD(void, addNewIncomingMessage, (const ChatKey &key, const QString &content, const ParticipantKey &participantKey, const QDateTime &timestamp), ());
    MOCK_METHOD(void, assignParticipantName, (const ParticipantKey &key, const QString &name), ());
    MOCK_METHOD(void, assignParticipantEntryDate, (const ParticipantKey &key, const QDateTime &entryDate), ());
    MOCK_METHOD(QString, getNickname, (), ());
    MOCK_METHOD(int, getPort, (), ());
    MOCK_METHOD(QString, getIp, (), ());
    MOCK_METHOD(ChatPreviewListModel *, getChatPreviewListModel, (), ());
    MOCK_METHOD(int, selectedChat, (), ());
    MOCK_METHOD(void, setSelectedChat, (int selectedChat), ());
};

class ClientCommunicatorTest : public ::testing::Test
{
protected:
    MockClient client;
    ClientCommunicator
};
