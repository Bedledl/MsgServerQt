#include <functional>
#include <gtest/gtest.h>
#include <map>
#include <string>

#include <QDateTime>

#include "buildServerCmd.h"
#include "serverMessageBuilder.h"

using functionPairToCompare = std::pair<std::function<std::string()>, std::function<std::string()>>;

class ServerMessageBuilderTest : public ::testing::TestWithParam<std::string>
{

public:
    static void SetUpTestSuite()
    {
        functionPairsToTest.emplace(std::string("genericResponseSuccess"), std::make_pair(
                                                                               std::bind(&ReferenceServerMessageBuilder::getGenericResponseCmd, &refServerMsgGenerator, ResponseCode::SUCCESS),
                                                                               std::bind(&ServerMessageBuilder::buildGenericResponse, &messageBuilder, seqNr, ResponseCode::SUCCESS)));
        functionPairsToTest.emplace("genericResponseError", std::make_pair(
                                                                std::bind(&ReferenceServerMessageBuilder::getGenericResponseCmd, &refServerMsgGenerator, ResponseCode::ERROR),
                                                                std::bind(&ServerMessageBuilder::buildGenericResponse, &messageBuilder, seqNr, ResponseCode::ERROR)));
        functionPairsToTest.emplace("genericResponseMalformed", std::make_pair(
                                                                    std::bind(&ReferenceServerMessageBuilder::getGenericResponseCmd, &refServerMsgGenerator, ResponseCode::MALFORMED_MESSAGE),
                                                                    std::bind(&ServerMessageBuilder::buildGenericResponse, &messageBuilder, seqNr, ResponseCode::MALFORMED_MESSAGE)));
        functionPairsToTest.emplace("buildAddedToChat", std::make_pair(
                                                            std::bind(&ReferenceServerMessageBuilder::getAddedToChatCmd, &refServerMsgGenerator),
                                                            std::bind(&ServerMessageBuilder::buildAddedToChat, &messageBuilder, seqNr, chatKey)));
        functionPairsToTest.emplace("buildLeftChat", std::make_pair(
                                                         std::bind(&ReferenceServerMessageBuilder::getRemovedFromChatCmd, &refServerMsgGenerator),
                                                         std::bind(&ServerMessageBuilder::buildLeftChat, &messageBuilder, seqNr, chatKey)));
        functionPairsToTest.emplace("participantAddedToChat", std::make_pair(
                                                                  std::bind(&ReferenceServerMessageBuilder::getParticipantAddedToChatCmd, &refServerMsgGenerator),
                                                                  std::bind(&ServerMessageBuilder::participantAddedToChat, &messageBuilder, seqNr, chatKey, participantKey)));
        functionPairsToTest.emplace("participantRemovedFromChat", std::make_pair(
                                                                      std::bind(&ReferenceServerMessageBuilder::getParticipantLeftChatCmd, &refServerMsgGenerator),
                                                                      std::bind(&ServerMessageBuilder::participantLeftChat, &messageBuilder, seqNr, chatKey, participantKey)));
        functionPairsToTest.emplace("participantRegistered", std::make_pair(
                                                                 std::bind(&ReferenceServerMessageBuilder::getParticipantAddedCmd, &refServerMsgGenerator),
                                                                 std::bind(&ServerMessageBuilder::buildParticipantRegistered, &messageBuilder, seqNr, participantKey)));
        functionPairsToTest.emplace("participantUnregistered", std::make_pair(
                                                                   std::bind(&ReferenceServerMessageBuilder::getParticipantRemovedCmd, &refServerMsgGenerator),
                                                                   std::bind(&ServerMessageBuilder::buildParticipantUnregistered, &messageBuilder, seqNr, participantKey)));
        functionPairsToTest.emplace("requestParticipantName", std::make_pair(
                                                                  std::bind(&ReferenceServerMessageBuilder::getParticipantNameCmd, &refServerMsgGenerator),
                                                                  std::bind(&ServerMessageBuilder::buildSendParticipantName, &messageBuilder, seqNr, participantKey, participantName)));
        functionPairsToTest.emplace("requestParticipantEntryDate", std::make_pair(
                                                                       std::bind(&ReferenceServerMessageBuilder::getParticipantEntryDateCmd, &refServerMsgGenerator),
                                                                       std::bind(&ServerMessageBuilder::buildSendParticipantEntryDate, &messageBuilder, seqNr, participantKey, timestamp)));
        functionPairsToTest.emplace("sendNewMsg", std::make_pair(
                                                      std::bind(&ReferenceServerMessageBuilder::getNewIncomingMessageCmd, &refServerMsgGenerator),
                                                      std::bind(&ServerMessageBuilder::buildNewMessage, &messageBuilder, seqNr, chatKey, participantKey, timestamp, testMessageContent)));
    }

public: // eigentlich protected
    static const SequenceNumber seqNr;
    static const ChatKey chatKey;
    static const ParticipantKey participantKey;
    static const std::string participantName;
    static const std::string testMessageContent;
    static const QDateTime timestamp;
    static const std::vector<unsigned> participantKeys;

    static ServerMessageBuilder messageBuilder;
    static ReferenceServerMessageBuilder refServerMsgGenerator;
    static std::map<std::string, functionPairToCompare> functionPairsToTest;
};
const SequenceNumber ServerMessageBuilderTest::seqNr = 1;
const ChatKey ServerMessageBuilderTest::chatKey = 123;
const ParticipantKey ServerMessageBuilderTest::participantKey = 234;

const std::string ServerMessageBuilderTest::participantName = "Romeo";
const std::string ServerMessageBuilderTest::testMessageContent = "Dear Juliet. Please wait for me in the garden.";
const QDateTime ServerMessageBuilderTest::timestamp = QDateTime::currentDateTime();
const std::vector<unsigned> ServerMessageBuilderTest::participantKeys = {273, 48, 1235, 9};

ServerMessageBuilder ServerMessageBuilderTest::messageBuilder{};
ReferenceServerMessageBuilder ServerMessageBuilderTest::refServerMsgGenerator{seqNr, chatKey, participantKey, participantName, testMessageContent, timestamp, participantKeys};
std::map<std::string, functionPairToCompare> ServerMessageBuilderTest::functionPairsToTest = {};

TEST_P(ServerMessageBuilderTest, compareMessages)
{
    auto testFuncName = GetParam();
    auto referenceMsgBuilderFunc = functionPairsToTest[testFuncName].first;
    auto MsgBuilderFuncToTest = functionPairsToTest[testFuncName].second;
    EXPECT_EQ(referenceMsgBuilderFunc(), MsgBuilderFuncToTest());
}

INSTANTIATE_TEST_SUITE_P(CompareAllServerMessagesToRef, ServerMessageBuilderTest,
                         ::testing::Values("genericResponseSuccess",
                                           "genericResponseError",
                                           "genericResponseMalformed",
                                           "buildAddedToChat",
                                           "buildLeftChat",
                                           "participantAddedToChat",
                                           "participantRemovedFromChat",
                                           "participantRegistered",
                                           "participantUnregistered",
                                           "requestParticipantName",
                                           "requestParticipantEntryDate",
                                           "sendNewMsg"));
