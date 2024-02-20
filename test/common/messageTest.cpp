#include <gtest/gtest.h>
#include <memory>

#include "message.h"
#include "participant.h"

class MessageTest : public testing::Test
{
protected:
    std::shared_ptr<Participant> testParticipant = std::make_shared<Participant>(123, "Thomas");
};

TEST_F(MessageTest, GetContent)
{
    // Create a message object
    Message message("Hello, world!", testParticipant);

    // Test the getContent() function
    EXPECT_EQ(message.getContent(), "Hello, world!");
}

TEST_F(MessageTest, GetContentEmpty)
{
    // Create an empty message object
    Message message("", testParticipant);

    // Test the getContent() function for an empty message
    EXPECT_EQ(message.getContent(), "");
}

TEST_F(MessageTest, StrongOrdering)
{
    auto timepoint1 = QDateTime(QDate(2024, 2, 20), QTime(10, 40));
    auto timepoint2 = QDateTime(QDate(2024, 2, 20), QTime(10, 41));

    auto content1 = QString("abc");
    auto content2 = QString("bcd");

    auto testParticipant2 = std::make_shared<Participant>(123, "Thomas");

    Message msg1{content1, testParticipant, timepoint1};
    Message msg2{content2, testParticipant, timepoint1};
    Message msg3{content2, testParticipant, timepoint2};
    Message msg4{content2, testParticipant, timepoint2};
    Message msg5{content2, testParticipant2, timepoint2};

    EXPECT_LE(msg1, msg2);
    EXPECT_LE(msg1, msg3);
    EXPECT_LE(msg2, msg3);
    EXPECT_TRUE((msg3 <=> msg4) == 0);

    EXPECT_EQ((testParticipant < testParticipant2), (msg4 < msg5));
}

TEST_F(MessageTest, GetSenderName)
{
    Message msg{"", testParticipant};
    EXPECT_EQ(msg.getSenderString(), testParticipant->getNickname());
}

TEST_F(MessageTest, getTimestamps)
{
    auto timepoint = QDateTime(QDate(2024, 2, 20), QTime(10, 40));
    Message msg{"", testParticipant, timepoint};
    auto qTimestamp = msg.getQTimestamp();
    auto chronoTimestamp = msg.getTimestamp();

    auto chronoSec = std::chrono::duration_cast<std::chrono::seconds>(
                         chronoTimestamp.time_since_epoch())
                         .count();
    auto qSec = qTimestamp.toSecsSinceEpoch();

    EXPECT_EQ(qSec, chronoSec);
}
