#ifndef CB79325A_D6E7_4A7F_A89B_BB74D5F74941
#define CB79325A_D6E7_4A7F_A89B_BB74D5F74941
#include <gtest/gtest.h>
#include "participant.h"

TEST(ParticipantTest, Getters)
{
    ParticipantKey key = 123;
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString name = "Claudia Musterfrau";

    Participant participant(key, name, currentDateTime);

    EXPECT_EQ(participant.getKey(), key);
    EXPECT_TRUE(participant.getNickname() == name);
    EXPECT_EQ(participant.getEntryDate(), currentDateTime);
}

TEST(ParticipantTest, Setters)
{
    ParticipantKey key = 123;
    Participant participant(key);

    QString nickname = "John";
    participant.setNickname(nickname);
    EXPECT_TRUE(participant.getNickname() == nickname);

    QDateTime entryDate = QDateTime::currentDateTime();
    participant.setEntryDate(entryDate);
    EXPECT_EQ(participant.getEntryDate(), entryDate);
}


#endif /* CB79325A_D6E7_4A7F_A89B_BB74D5F74941 */
