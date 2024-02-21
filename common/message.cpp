#include <chrono>

#include <QDateTime>
#include <QString>

#include "message.h"
#include "participant.h"

Message::Message(QString content, std::shared_ptr<Participant> sender, QDateTime sentDateTime)
    : sentDateTime(sentDateTime), content(content), sender(sender){};

std::strong_ordering Message::operator<=>(const Message &msg) const
{
    if (sentDateTime == msg.sentDateTime)
    {
        if (sender == msg.sender)
        {
            auto string_compare = QString::compare(content, msg.content, Qt::CaseSensitive);
            if (string_compare == 0)
            {
                return std::strong_ordering::equal;
            }
            else if (string_compare < 0)
            {
                return std::strong_ordering::less;
            }
            return std::strong_ordering::greater;
        }
        else
        {
            return sender <=> msg.sender;
        }
    }
    else if (sentDateTime < msg.sentDateTime)
    {
        return std::strong_ordering::less;
    }
    return std::strong_ordering::greater;
};
QString Message::getContent() const
{
    return content;
}
QString Message::toString() const
{
    QString senderName = getSenderString();
    return sentDateTime.toString("dd.MM.yyyy hh:mm") + " " + senderName + ": " + content;
}
std::shared_ptr<Participant> Message::getSender() const
{
    return sender;
}
std::chrono::system_clock::time_point Message::getTimestamp() const
{
    return std::chrono::system_clock::time_point(std::chrono::milliseconds(sentDateTime.toMSecsSinceEpoch()));
}
QDateTime Message::getQTimestamp() const
{
    return sentDateTime;
}
QString Message::getSenderString() const
{
    return sender->getNickname();
}
