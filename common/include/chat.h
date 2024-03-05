#ifndef CHAT_H
#define CHAT_H

#include <QObject>
#include <QReadWriteLock>
#include <functional>
#include <set>

#include "message.h"
#include "uniqueKeysTypedef.h"


class Chat : public QObject
{
    Q_OBJECT

    std::set<Message> messages = {};

public:
    void foreach_do(std::function<void(const Message &)> func);

public slots:
    void add_message(Message msg);

signals:
    void messagesChanged();
};

#endif