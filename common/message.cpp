#include <QDateTime>
#include <QString>

#include "message.h"
#include "participant.h"

Message::Message(QString content, Participant *sender) : date(std::chrono::system_clock::now()), content(content), sender(sender){};

std::strong_ordering Message::operator<=>(const Message &msg) const
{
    if (date == msg.date)
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
    return date <=> msg.date;
};
QString Message::getContent() const
{
    return content;
}
QString Message::toString() const
{
    std::time_t date_c = std::chrono::system_clock::to_time_t(date);
    std::tm date_tm = *std::localtime(&date_c);

    char buff[10];
    QString date_str;
    if (!strftime(buff, sizeof buff, "%d:%m:%y", &date_tm))
    {
        date_str = QString("Unknown date");
    }
    else
    {
        date_str = QString(buff);
    }

    return date_str + QString(": ") + content;
}
Participant const *Message::getSender() const
{
    return sender;
}
std::chrono::time_point<std::chrono::system_clock> Message::getTimestamp() const
{
    return date;
}
QDateTime Message::getQTimestamp() const
{
    return QDateTime::fromSecsSinceEpoch(std::chrono::system_clock::to_time_t(date));
}
QString Message::getSenderString() const
{
    if (sender == nullptr)
    {
        return QString("Unknown");
    }
    return sender->getNickname();
}