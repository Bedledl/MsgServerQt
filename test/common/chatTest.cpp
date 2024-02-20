#include <gtest/gtest.h>
#include <stdexcept>

#include <QDate>
#include <QDateTime>
#include <QTime>

#include "chat.h"
#include "message.h"
#include "participant.h"

class ChatTest : public testing::Test
{
protected:
    std::shared_ptr<Participant> participant1 = std::make_shared<Participant>(123);
    std::shared_ptr<Participant> participant2 = std::make_shared<Participant>(234);
};

TEST_F(ChatTest, ParticipantsTest)
{
    Chat chat_(1);

    Message msg{"", participant1};

    EXPECT_THROW({ chat_.addMessage(msg); }, SenderNotChatParticipant);

    chat_.addParticipant(participant1);

    EXPECT_NO_THROW({ chat_.addMessage(msg); });

    chat_.removeParticipant(participant1);

    EXPECT_THROW({ chat_.addMessage(msg); }, SenderNotChatParticipant);
};

TEST_F(ChatTest, GetMessageAt)
{
    Chat chat_(1);
    chat_.addParticipant(participant1);
    chat_.addParticipant(participant2);

    Message message1("Hello", participant1, QDateTime::currentDateTime());
    Message message2("Hi", participant2, QDateTime::currentDateTime().addSecs(1));

    chat_.addMessage(message2);
    chat_.addMessage(message1);

    ASSERT_NO_THROW({ chat_.getMessageAt(0); });
    ASSERT_NO_THROW({ chat_.getMessageAt(1); });

    auto result1 = chat_.getMessageAt(0);
    auto result2 = chat_.getMessageAt(1);

    EXPECT_EQ(result1.getContent(), "Hello");
    EXPECT_EQ(result2.getContent(), "Hi");
};

TEST_F(ChatTest, GetMessageAt_InvalidIndex)
{
    Chat chat_(1);
    chat_.addParticipant(participant1);

    EXPECT_THROW({ chat_.getMessageAt(0); }, std::out_of_range);
};

TEST_F(ChatTest, MessageOrdering)
{
    Chat chat_(1);
    chat_.addParticipant(participant1);
    chat_.addParticipant(participant2);

    QDateTime timepoint1{QDate(2024, 2, 20), QTime(10, 40)};
    QDateTime timepoint2{QDate(2024, 2, 20), QTime(10, 41)};

    QString content1{"abc"};
    QString content2{"bcd"};

    Message msg1{content1, participant1, timepoint1};
    Message msg2{content2, participant2, timepoint1};
    Message msg3{content2, participant2, timepoint2};
    Message msg4{content2, participant2, timepoint2};

    chat_.addMessage(msg4);
    chat_.addMessage(msg3);
    chat_.addMessage(msg2);
    chat_.addMessage(msg1);

    // because message 3 and 4 are equal in the Message's ordering, only one of the objects is added to the messages list
    EXPECT_THROW({ chat_.getMessageAt(3); }, std::out_of_range);

    auto msg0_ = chat_.getMessageAt(0);
    auto msg1_ = chat_.getMessageAt(1);
    auto msg2_ = chat_.getMessageAt(2);

    EXPECT_EQ(msg0_.getContent(), content1);
    EXPECT_EQ(msg1_.getContent(), content2);
    EXPECT_EQ(msg2_.getContent(), content2);

    EXPECT_EQ(msg0_.getQTimestamp(), timepoint1);
    EXPECT_EQ(msg1_.getQTimestamp(), timepoint1);
    EXPECT_EQ(msg2_.getQTimestamp(), timepoint2);
};