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

/// @brief Global Chat as Singleton, because we want to have exactly one instance available over the lifetime of the server.
class GlobalChat : public Chat
{
public:
    static GlobalChat &get_instance()
    {
        static GlobalChat instance;
        volatile int8_t dummy;
        dummy; // just for removeing the unused warning. This variable is for preventing the optimizing away the instance
        return instance;
    }

private:
    GlobalChat() = default;
    ~GlobalChat() = default;
    GlobalChat(const GlobalChat &) = delete;
    GlobalChat &operator=(const GlobalChat &) = delete;
};

#endif