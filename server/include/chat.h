#ifndef SERVER_CHAT_H
#define SERVER_CHAT_H

#include "../common/include/chat.h"
#include "message.h"
#include "uniqueKey.h"
#include <QReadWriteLock>

class ServerChat : public Chat
{
    const ChatKey key;
    QReadWriteLock lock;

public:
    ServerChat() : key(UniqueKeyGenerator<ChatKey>::get_instance()->requestKey()){};
    ~ServerChat()
    {
        UniqueKeyGenerator<ChatKey>::get_instance()->releaseKey(key);
    };
    void foreach_do(std::function<void(const Message &)> func);
    void add_message(Message msg);
    ChatKey get_key() const { return key; };
};

#endif