#ifndef SERVER_CHAT_H
#define SERVER_CHAT_H

#include "../common/include/chat.h"
#include "message.h"
#include "uniqueKey.h"
#include <QReadWriteLock>

class ServerChat : public Chat
{
    Q_OBJECT
    QReadWriteLock lock;

public:
    ServerChat(QObject *parent=nullptr) : Chat(UniqueKeyGenerator<ChatKey>::get_instance()->requestKey(), parent){};
    ~ServerChat()
    {
        UniqueKeyGenerator<ChatKey>::get_instance()->releaseKey(key);
    };
    void foreach_do(std::function<void(const Message &)> func);
    void add_message(Message msg);
};

#endif
