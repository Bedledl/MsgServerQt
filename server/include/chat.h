#ifndef CHAT_H
#define CHAT_H

#include <QReadWriteLock>
#include <functional>
#include <set>

#include "clientOrganizer.h"
#include "message.h"
#include "uniqueKey.h"

class Chat
{
    const UniqueKey<Chat> key;
    std::set<Message> messages = {};
    QReadWriteLock lock;

public:
    Chat(){};
    void foreach_do(std::function<void(const Message &)> func);
    void add_message(Message msg);
    const UniqueKey<Chat> &get_key() const { return key; };
};

#endif