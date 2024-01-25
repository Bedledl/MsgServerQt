#ifndef SERVER_CHAT_H
#define SERVER_CHAT_H

#include "../common/include/chat.h"
#include "message.h"
#include "uniqueKey.h"
#include <QReadWriteLock>

class ServerChat : public Chat
{
    const UniqueKey<ServerChat> key;
    QReadWriteLock lock;

public:
    void foreach_do(std::function<void(const Message &)> func);
    void add_message(Message msg);
    const UniqueKey<ServerChat> &get_key() const { return key; };
};

/// @brief Global Chat as threadsafe Singleton, because we want to have exactly one instance available over the lifetime of the server.
class GlobalChat : public ServerChat
{
public:
    // static GlobalChat &get_instance()
    // {
    //     static GlobalChat instance;
    //     volatile int8_t dummy;
    //     dummy; // just for removeing the unused warning. This variable is for preventing the optimizing away the instance
    //     return instance;
    // }
    static GlobalChat *get_instance()
    {
        if (!instance)
        {
            instance = new GlobalChat();
        }

        return instance;
    }

private:
    GlobalChat() = default;
    ~GlobalChat() = default;
    GlobalChat(const GlobalChat &) = delete;
    GlobalChat &operator=(const GlobalChat &) = delete;

    static GlobalChat *instance;
};

GlobalChat *GlobalChat::instance = nullptr;

#endif