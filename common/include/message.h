#ifndef MESSAGE_H
#define MESSAGE_H

#include <chrono>
#include <compare>
#include <string>

/// @brief  forward declaration of Participant
class Participant;

class Message
{

public:
    Message(QString content, Participant *sender = nullptr);
    std::strong_ordering operator<=>(const Message &msg) const;
    QString getContent() const;
    QString toString() const;
    Participant const *getSender() const;
    std::chrono::time_point<std::chrono::system_clock> getTimestamp() const;
    QDateTime getQTimestamp() const;
    QString getSenderString() const;

private:
    Participant const *sender;
    std::chrono::time_point<std::chrono::system_clock> date;
    QString content;
};

#endif