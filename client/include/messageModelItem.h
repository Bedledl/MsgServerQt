#ifndef BCA63783_3916_45EC_BB00_2B73D19903E5
#define BCA63783_3916_45EC_BB00_2B73D19903E5
#include <QDateTime>
#include <QObject>

#include "message.h"

class MessageModelItem
{
    Q_OBJECT
    Q_PROPERTY(string content READ getContent CONSTANT);
    Q_PROPERTY(QDateTime timestamp READ getTimestamp CONSTANT);
    Q_PROPERTY(string sender READ getSender CONSTANT);

public:
    MessageModelItem(const Message *message) : message(message){};

private:
    QString getContent() const
    {
        return message->getContent();
    }
    QDateTime getTimestamp() const
    {
        return message->getQTimestamp();
    }
    QString getSender() const
    {
        return message->getSenderString();
    }
    const Message *message;
};
#endif /* BCA63783_3916_45EC_BB00_2B73D19903E5 */
