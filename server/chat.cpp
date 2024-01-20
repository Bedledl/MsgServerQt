#include "include/chat.h"
#include <functional>

void ServerChat::add_message(Message msg)
{
    lock.lockForWrite();
    Chat::add_message(std::move(msg));
    lock.unlock();
}

void ServerChat::foreach_do(std::function<void(const Message &)> func)
{
    lock.lockForRead();
    Chat::foreach_do(std::move(func));
    lock.unlock();
}
