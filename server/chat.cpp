#include "include/chat.h"

void Chat::add_message(Message msg) {
    lock.lockForWrite();
    messages.emplace(msg);
    lock.unlock();
}


void Chat::foreach_do(std::function<void( const Message&)> func) {
    lock.lockForRead();
    std::for_each(messages.begin(), messages.end(), func);
    lock.unlock();
}
