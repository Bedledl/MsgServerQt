#ifndef BCA63783_3916_45EC_BB00_2B73D19903E5
#define BCA63783_3916_45EC_BB00_2B73D19903E5
#include <QDateTime>
#include <QObject>

#include "message.h"

class MessageModelItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString content READ getContent CONSTANT);
    Q_PROPERTY(QDateTime timestamp READ getTimestamp CONSTANT);
    Q_PROPERTY(QString sender READ getSender CONSTANT);

public:
    MessageModelItem(const Message *message, QObject *parent = nullptr) : QObject(parent), message(message){};

private:
    QString getContent() const;
    QDateTime getTimestamp() const;
    QString getSender() const;
    const Message *message;
};
#endif /* BCA63783_3916_45EC_BB00_2B73D19903E5 */
