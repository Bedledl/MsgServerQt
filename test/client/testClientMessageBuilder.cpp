#include <functional>
#include <gtest/gtest.h>
#include <map>
#include <string>

#include <QDateTime>

#include "buildClientCmd.h"
#include "clientMessageBuilder.h"

class ClientMessageBuilderTest : public ::testing::TestWithParam<std::string>
{
protected:
    const SequenceNumber seqNr = 1;
    const ChatKey chatKey = 123;
    const ParticipantKey participantKey = 234;
    std::string participantName = "Romeo";
    std::string testMessageContent = "Dear Juliet. Please wait for me in the garden.";
    QDateTime timestamp = QDateTime::currentDateTime();
    std::vector<unsigned> participantKeys = {273, 48, 1235, 9};

    ReferenceClientMessageBuilder clientMsgGenerator{seqNr, chatKey, participantKey, participantName, testMessageContent, timestamp};

    std::map<std::string, std::pair<std::function<std::string()>, std::function<std::string()>>> functionPairsToTest = {
        {"setNickname", std::make_pair(
                            std::bind(&ReferenceClientMessageBuilder::setNickname, &clientMsgGenerator),
                            std::bind(&ClientMessageBuilder::getSetNewNickname, seqNr, participantName))},
        {"requestNewChat", std::make_pair(
                               std::bind(&ReferenceClientMessageBuilder::requestNewChat, &clientMsgGenerator),
                               std::bind(&ClientMessageBuilder::getRequestNewChat, seqNr))},
        {"requestChatParticipantKeys", std::make_pair(
                                           std::bind(&ReferenceClientMessageBuilder::requestChatParticipantKeys, &clientMsgGenerator),
                                           std::bind(&ClientMessageBuilder::getRequestParticipantKeys, seqNr, chatKey))},
        {"requestLeavingChat", std::make_pair(
                                   std::bind(&ReferenceClientMessageBuilder::requestLeavingChat, &clientMsgGenerator),
                                   std::bind(&ClientMessageBuilder::getRequestLeaveChat, seqNr, chatKey))},
        {"requestParticipantName", std::make_pair(
                                       std::bind(&ReferenceClientMessageBuilder::requestParticipantName, &clientMsgGenerator),
                                       std::bind(&ClientMessageBuilder::getRequestParticipantName, seqNr, participantKey))},
        {"requestParticipantEntryDate", std::make_pair(
                                            std::bind(&ReferenceClientMessageBuilder::requestParticipantEntryDate, &clientMsgGenerator),
                                            std::bind(&ClientMessageBuilder::getRequestParticipantEntryDate, seqNr, participantKey))},
        {"requestAddingParticipantToChat", std::make_pair(
                                               std::bind(&ReferenceClientMessageBuilder::requestAddingParticipantToChat, &clientMsgGenerator),
                                               std::bind(&ClientMessageBuilder::getRequestAddingParticipantToChat, seqNr, chatKey, participantKey))},
        {"sendNewMsg", std::make_pair(
                           std::bind(&ReferenceClientMessageBuilder::sendNewMsg, &clientMsgGenerator),
                           std::bind(&ClientMessageBuilder::getSendNewMsg, seqNr, chatKey, testMessageContent))},
        {"genericResponseSuccess", std::make_pair(
                                       std::bind(&ReferenceClientMessageBuilder::genericResponseSuccess, &clientMsgGenerator),
                                       std::bind(&ClientMessageBuilder::getGenericResponseSuccess, seqNr))},
        {"genericResponseError", std::make_pair(
                                     std::bind(&ReferenceClientMessageBuilder::genericResponseError, &clientMsgGenerator),
                                     std::bind(&ClientMessageBuilder::getGenericResponseError, seqNr))},
        {"genericResponseMalformed", std::make_pair(
                                         std::bind(&ReferenceClientMessageBuilder::genericResponseMalformed, &clientMsgGenerator),
                                         std::bind(&ClientMessageBuilder::getGenericResponseMalformed, seqNr))},
    };
};

TEST_P(ClientMessageBuilderTest, compareMessages)
{
    auto testFuncName = GetParam();
    auto referenceMsgBuilderFunc = functionPairsToTest[testFuncName].first;
    auto MsgBuilderFuncToTest = functionPairsToTest[testFuncName].second;
    EXPECT_EQ(referenceMsgBuilderFunc(), MsgBuilderFuncToTest());
}

INSTANTIATE_TEST_SUITE_P(CompareAllClientMessagesToRef, ClientMessageBuilderTest,
                         ::testing::Values("setNickname",
                                           "requestNewChat",
                                           "requestChatParticipantKeys",
                                           "requestLeavingChat",
                                           "requestParticipantName",
                                           "requestParticipantEntryDate",
                                           "requestAddingParticipantToChat",
                                           "sendNewMsg",
                                           "genericResponseSuccess",
                                           "genericResponseError",
                                           "genericResponseMalformed"));
